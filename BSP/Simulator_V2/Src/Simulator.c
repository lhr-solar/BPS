/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * This file creates an interface for the simulator to create a trace for running tests
 */

#include "Simulator.h"
#include <time.h> // for creating unique logfile names
// for open() syscall
#include <sys/types.h>  // for stat() and struct stat as well
#include <sys/stat.h>
#include <fcntl.h>
// for write() and read()
#include <unistd.h>
// for sprintf
#include <stdio.h>
// for signal handler
#include <signal.h>
// for boolean
#include <stdbool.h>
#include "cJSON.h" // for json parser
#include <stdlib.h> // for calloc/free

#include "BSP_CAN.h" // CAN testing

// file descriptor of simulator log file
int simulatorLog;

// linked list of simulator states
static simulator_state *states = NULL;

// timing information to handle the state transitions
// note that this only has a granularity of 1 second, so it's not super precise
static time_t startTime;

/**
 * @brief   Log something to simulator log file
 * @param   lvl - Choose level at which to log at. Levels defined in LoggingType enum.
 * @param   str - string to print
 * @return  None
 */
void Simulator_log(LoggingType lvl, char *str) {
    char* msg = strcat(strcat(LoggingLUT[lvl], str))
    write(simulatorLog, msg, strlen(msg));
    if (0) {
        char *buffer = (char *) malloc(strlen(msg) + 2);
        sprintf(buffer, "%s%s\n", msg);
        free(buffer);
    }

// It knows what the input file is because the Makefile should make a #define for the file path called SIMULATOR_JSON_PATH
#ifndef SIMULATOR_JSON_PATH
#define JSON_PATH "Error: make sure you define the simulator JSON path in the Makefile!"
#else
// #define QUOTE_MARK "
#define STR(X) # X
#define STRINGIFY(X) STR(X)
#define JSON_PATH STRINGIFY(SIMULATOR_JSON_PATH) 
#endif

// units in mV
static uint16_t DUMMY_VOLTAGES[NUM_BATTERY_MODULES] = { 3000 };
// units in mC, but config.h uses 45,000 as 45.00C, so we'll bump these up to 30k for 30C
static uint16_t DUMMY_TEMPS[NUM_TEMPERATURE_SENSORS] = { 30000 };

/**
 * @brief   Shut down the simulator
 * @param   status - Look at man page for C exit() function 
 * @return  None
 */
void Simulator_shutdown(int status) {
    Simulator_log("Shutting down the simulator...\n");
    close(simulatorLog);
    exit(status);
}

/**
 * @brief   Read the input json file and parse it
 * @param   jsonPath - Path to json file
 * @return  None
 */
static void readInputFile(char *jsonPath) {
    // get the length of the input file
    struct stat inFileStats;
    if (stat(jsonPath, &inFileStats) < 0) {
        printf("error getting stats for file %s\n", jsonPath);
        exit(-1);
    }
    off_t length = inFileStats.st_size;

    // read in the input file
    char *inFile = (char *) malloc(length);
    int fd = open(jsonPath, O_RDONLY);
    if (fd < 0) {
        printf("error opening file %s\n", jsonPath);
        exit(-1);
    }
    if (read(fd, inFile, length) < 0) {
        printf("error reading file %s", jsonPath);
        exit(-1);
    }

    // parses the input file as a json
    cJSON *json = cJSON_ParseWithLength(inFile, length);

    // parse the json structure into an array of states
    simulator_state *tail = states;
    uint8_t stateCount = 0;
    // For every state in the JSON (highest level {...})
    for (cJSON *state = json->child; state != NULL; state = state->next) {
        if (states == NULL) {
            tail = (simulator_state *) calloc(1, sizeof(simulator_state));
            states = tail;
        } else {
            simulator_state *temp = (simulator_state *) calloc(1, sizeof(simulator_state));
            tail->next = temp;
            tail = tail->next;
        }
        char buffer[50];
        sprintf(buffer, "\nExecuting state #%d...\n", stateCount);
        Simulator_log(buffer);
        // Try and get every field we should have in each state.
        // If null, it doesn't exist.
        cJSON* time = cJSON_GetObjectItem(state, "time");
        cJSON* adcHigh = cJSON_GetObjectItem(state, "adcHigh");
        cJSON* adcLow = cJSON_GetObjectItem(state, "adcLow");
        // voltageArray should have 31 elements.
        cJSON* voltageArray = cJSON_GetObjectItem(state, "voltages");
        // temperatureArray should have 31*2 elements.
        cJSON* temperatureArray = cJSON_GetObjectItem(state, "temperatures");
        cJSON* current = cJSON_GetObjectItem(state, "current");
        cJSON* charge = cJSON_GetObjectItem(state, "charge");
        cJSON* canList = cJSON_GetObjectItem(state, "can");
    
        // Check which state fields aren't present in the current sim state JSON.
        // Lots of nasty if/else's here, not much way around this.
        if (!time) {
            printf("Current simulator state does not have a specified time value. Using -1...\n");
            tail->time = -1;
        } else tail->time = time->valueint;
        if (!adcHigh) {
            printf("Current simulator state does not have a specified adcHigh value. Using -1...\n");
            tail->adcHigh = -1;
        } else tail->adcHigh = adcHigh->valueint;
        if (!adcLow) {
            printf("Current simulator state does not have a specified adcLow value. Using -1...\n");
            tail->adcLow = -1;
        } else tail->adcLow = adcLow->valueint;
        if (!voltageArray) { // see top of file for dummy array. units in milliVolts
            printf("Current simulator state does not have a specified voltage array. Using default...\n");
            memcpy(tail->voltages, DUMMY_VOLTAGES, sizeof(uint16_t) * NUM_BATTERY_MODULES);
        } else { // otherwise, the voltage array exists; check size, then copy elements over.
            if (cJSON_GetArraySize(voltageArray) != NUM_BATTERY_MODULES) {
                printf("Voltage array in simulator state %d does not have 31 elements in it! Please fix. Exiting...\n", stateCount);
                exit(-1);
            }
            for (int idx = 0; idx < NUM_BATTERY_MODULES; idx++) {
                tail->voltages[idx] = cJSON_GetArrayItem(voltageArray, idx)->valueint;
            }
        }
        if (!temperatureArray) { // see top of file for dummy array. units in milliCelsius
            printf("Current simulator state does not have a specified temperature array. Using default...\n");
            memcpy(tail->temperatures, DUMMY_TEMPS, sizeof(uint16_t) * NUM_TEMPERATURE_SENSORS);
        } else { // otherwise, the temperature array exists; check size, then copy elements over.
            if (cJSON_GetArraySize(temperatureArray) != NUM_TEMPERATURE_SENSORS) {
                printf("Temperature array in simulator state %d does not have 62 elements in it! Please fix. Exiting...\n", stateCount);
                exit(-1);
            }
            for (int idx = 0; idx < NUM_TEMPERATURE_SENSORS; idx++) {
                tail->temperatures[idx] = cJSON_GetArrayItem(temperatureArray, idx)->valueint;
            }
        }
        if (!current) { // current in mA
            printf("Current simulator state does not have a specified current value. Using 30...\n");
            tail->current = 30;
        }
        if (!charge) {
            printf("Current simulator state does not have a specified charge value. Using 25,000,000...\n");
            tail->charge = 25000000;
        }
        if (!canList) {
            char buffer[67];
            sprintf(buffer, "No CAN messages to simulate in this state. (State Count = %d)\n", stateCount);
            Simulator_log(buffer);
        } else { // otherwise, there are some potential CAN messages. 
            // for every CAN message...
            for (cJSON* msg = canList->child; msg != NULL; msg = msg->next) {
                uint32_t id = cJSON_GetObjectItem(msg, "id")->valueint;
                char* read_write = cJSON_PrintUnformatted(cJSON_GetObjectItem(msg, "read_write"));
                uint8_t* data = (uint8_t*)cJSON_PrintUnformatted(cJSON_GetObjectItem(msg, "data"));
                uint8_t len = cJSON_GetObjectItem(msg, "len")->valueint;
                if (strcmp(read_write, "\"read\"") == 0)
                    BSP_CAN_Read(&id, data);
                else if (strcmp(read_write, "\"write\"") == 0)
                    BSP_CAN_Write(id, data, len);
                else {
                    printf("Error: CAN message read/write type invalid! [%s]\n", read_write);
                    exit(-1);
                }
            }
        }
        stateCount++; // keep track of which state we are on.
    }
}

/**
 * @brief   Handler for Ctrl-C to safely shut down the simulator
 * @param   n - unused
 * @return  None
 */
void CtrlCHandler(int n) {
    Simulator_log("simulator received SIGINT!\n");
    Simulator_shutdown(-1);
}

/**
 * @brief   Initialize Simulator
 * @param   jsonPath - path to json file
 * @return  None
 */
void Simulator_init(char *jsonPath) {
    // generate unique name for log file
    startTime = time(NULL);
    char filename[30];
    sprintf(filename, "bps-sim-%ld.log", startTime);

    // create the log file
    simulatorLog = open(filename, O_CREAT | O_WRONLY, 0664);
    if (simulatorLog < 0) {
        printf("error opening file %s\n", jsonPath);
        exit(-1);
    }
    if (write(simulatorLog, "simulator started...\n", 21) < 0) {
        printf("error writing file %s\n", jsonPath);
        exit(-1);
    }
    
    // register the Ctrl-C handler
    sigset_t s;
    sigemptyset(&s);
    const struct sigaction act = {.sa_handler = CtrlCHandler, .sa_mask = s, .sa_flags = 0};
    sigaction(SIGINT, &act, NULL);

    Simulator_log("Simulator intialized\n");    

    // initialize the fake inputs
    readInputFile(jsonPath);

    // log the starting time
    startTime = time(NULL);
}

/**
 * @brief   Check for state transition and transition if necessary
 * @param   None
 * @return  None
 */
static void Simulator_Transition(void) {
    // advance to the current state
    time_t currentTime = time(NULL);
    while (startTime + states->time < currentTime) {
        simulator_state *prev = states;
        states = states->next;
        startTime += prev->time;
        free(prev);

        if (states == NULL) {
            Simulator_log("Finished last state!\n");
            Simulator_shutdown(0);
        }
    }
}


// Functions for accessing simulator state. Each field should have a function associated with it

// get the adcHigh
uint16_t Simulator_getAdcHigh(void) {
    Simulator_Transition();
    return states->adcHigh;
}

// get the adcLow
uint16_t Simulator_getAdcLow(void) {
    Simulator_Transition();
    return states->adcLow;
}

// get the voltages
uint16_t Simulator_getVoltage(int i) {
    Simulator_Transition();
    return states->voltages[i];
}

// get a single temperature sensor
int32_t Simulator_getTemperature(int i) {
    Simulator_Transition();
    return states->temperatures[i];
}

// get the current
int32_t Simulator_getCurrent(void) {
    Simulator_Transition();
    return states->current;
}

// get the charge (stored in the EEPROM)
uint32_t Simulator_getCharge(void) {
    Simulator_Transition();
    return states->charge;
}
