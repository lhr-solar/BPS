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

// It knows what the input file is because the Makefile should make a #define for the file path called SIMULATOR_JSON_PATH
#ifndef SIMULATOR_JSON_PATH
#define JSON_PATH "Error: make sure you define the simulator JSON path in the Makefile!"
#else
// #define QUOTE_MARK "
#define STR(X) # X
#define STRINGIFY(X) STR(X)
#define JSON_PATH STRINGIFY(SIMULATOR_JSON_PATH) 
#endif

// log something to the simualtor's log file
void Simulator_log(char *str) {
    write(simulatorLog, str, strlen(str));
}

// shut down the simulator
void Simulator_shutdown(int status) {
    Simulator_log("Shutting down the simulator...\n");
    close(simulatorLog);
    exit(status);
}

// read the input json file and parse it
static void readInputFile(void) {
    // get the length of the input file
    struct stat inFileStats;
    if (stat(JSON_PATH, &inFileStats) < 0) {
        printf("error getting stats for file %s\n", JSON_PATH);
        exit(-1);
    }
    off_t length = inFileStats.st_size;

    // read in the input file
    char *inFile = (char *) malloc(length);
    int fd = open(JSON_PATH, O_RDONLY);
    if (fd < 0) {
        printf("error opening file %s\n", JSON_PATH);
        exit(-1);
    }
    if (read(fd, inFile, length) < 0) {
        printf("error reading file %s", JSON_PATH);
        exit(-1);
    }

    // parses the input file as a json
    cJSON *json = cJSON_ParseWithLength(inFile, length);

    // parse the json structure into an array of states
    simulator_state *tail = states;
    for (cJSON *state = json->child; state != NULL; state = state->next) {
        if (states == NULL) {
            tail = (simulator_state *) calloc(1, sizeof(simulator_state));
            states = tail;
        } else {
            simulator_state *temp = (simulator_state *) calloc(1, sizeof(simulator_state));
            tail->next = temp;
            tail = tail->next;
        }
        // iterate through a single simulator state
        for (cJSON *field = state->child; field != NULL; field = field->next) {
            if (strcmp(field->string, "time") == 0) {
                tail->time = field->valueint;
            } else if (strcmp(field->string, "adcHigh") == 0) {
                tail->adcHigh = field->valueint;
            } else if (strcmp(field->string, "adcLow") == 0) {
                tail->adcLow = field->valueint;
            } else if (strcmp(field->string, "voltages") == 0) {
                // get all of the voltages
                int i = 0;
                for (cJSON *voltage = field->child; voltage != NULL && i < NUM_BATTERY_MODULES; i++, voltage = voltage->next) {
                    tail->voltages[i] = voltage->valueint;
                }
                if (i != NUM_BATTERY_MODULES) {
                    printf("Error: incorrect number of voltages! Expected %d, but got %d\n", NUM_BATTERY_MODULES, i);
                    exit(-1);
                }
            } else if (strcmp(field->string, "temperatures") == 0) {
                // get all of the temperatures
                int i = 0;
                for (cJSON *temperature = field->child; 
                    temperature != NULL && i < (NUM_BATTERY_MODULES * NUM_TEMP_SENSORS_PER_MOD); 
                    i++, temperature = temperature->next) {

                    tail->temperatures[i] = temperature->valueint;
                }
                if (i != (NUM_BATTERY_MODULES * NUM_TEMP_SENSORS_PER_MOD)) {
                    printf("Error: incorrect number of temperatures! Expected %d, but got %d\n", (NUM_BATTERY_MODULES * NUM_TEMP_SENSORS_PER_MOD), i);
                    exit(-1);
                }
            } else if (strcmp(field->string, "current") == 0) {
                tail->current = field->valueint;
            } else if (strcmp(field->string, "charge") == 0) {
                tail->charge = field->valueint;
            } else if (strcmp(field->string, "can") == 0) {
                // for every CAN msg in the list...
                for (cJSON* msg = field->child; msg != NULL; msg = msg->next) {
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
            else {
                printf("Error: unknown field %s in simulator state!\n", field->string);
                exit(-1);
            }
        }
    }
}

// handler for Ctrl-C to safely shut down the simulator
void CtrlCHandler(int n) {
    Simulator_log("simulator received SIGINT!\n");
    Simulator_shutdown(-1);
}

// intialize the simulator
void Simulator_init(void) {
    // generate unique name for log file
    startTime = time(NULL);
    char filename[30];
    sprintf(filename, "bps-sim-%ld.log", startTime);

    // create the log file
    simulatorLog = open(filename, O_CREAT | O_WRONLY, 0664);
    if (simulatorLog < 0) {
        printf("error opening file %s\n", JSON_PATH);
        exit(-1);
    }
    if (write(simulatorLog, "simulator started...\n", 21) < 0) {
        printf("error writing file %s\n", JSON_PATH);
        exit(-1);
    }
    
    // register the Ctrl-C handler
    sigset_t s;
    sigemptyset(&s);
    const struct sigaction act = {.sa_handler = CtrlCHandler, .sa_mask = s, .sa_flags = 0};
    sigaction(SIGINT, &act, NULL);

    Simulator_log("Simulator intialized\n");    

    // initialize the fake inputs
    readInputFile();

    // log the starting time
    startTime = time(NULL);
}

// check for state transition and transition if necessary
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

// Functions for accessing simulator state
// add more of these if you add more fields

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