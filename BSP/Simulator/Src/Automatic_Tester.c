/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Automatic_Tester.h"
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
#include <stdlib.h> // for calloc/free
#include <errno.h>
#include <assert.h>

/**
 * @brief   Generates random numbers between a range
 * @param   lower The lower bound, inclusive
 * @param   upper The upper bound, exclusive
 * @return  Random number between lower and upper
 */
#define randomRange(lower, upper) ((rand() % (upper - lower)) + lower)

static struct State states[jsonLength];

char* fp = "/BSP/Simulator/Data/AutomatedTests/";

/**
 * @brief The output of this file automates the generation of test files and runs them using the simulator
 * @param argv CLI inputs that specify which test(s) to run. 
 * @note Example: ./Test_Generator 123 will generate and run tests 1, 2, 3
 * @note If no command line arguments are supplied, you will have to type them after the program starts running
 * 
*/
int main(int argc, char **argv){
    //seed the RNG
    srand(time(NULL));
    int inputsize;
    char* inputargs;

    //make the directory for the automated tests if it doesn't exist
    char *cwd;
    char *automatedTestFolder;
    char *chmod;
    cwd = getcwd(NULL, 0);
    asprintf(&automatedTestFolder, "%s%s", cwd, fp);
    mkdir(automatedTestFolder, 0777);
    asprintf(&chmod,"sudo chmod -R 777 %s", automatedTestFolder);
    system(chmod);
    free(chmod);
    free(cwd);
    free(automatedTestFolder);


    //input string parsing
    if(argc > 1){
    //argc more than 1 means the user has passed arguments that will be processed
            //allocate space for all the arguments that the user typed
        char* strptr = *(argv + argc - 1); //starting address of the last argument
        //search for null terminator of the last argument
        while(*strptr != '\0'){
            strptr++;
        }

        inputsize =(int) (strptr - *(argv + 1) - (argc - 2)); //don't wanna count null terminators so we subtract out number of command line arguments
        inputargs = (char *)malloc(inputsize);        

        //add all tests to our inputargs array
        int inputcounter = 0;//inputcounter keeps track of the index we are storing the inputarg
        for(int i = 1; i < argc; i++){
            for(int j = 0; argv[i][j] != '\0'; j++){
                if(argv[i][j] == ALL_ERR || argv[i][j] == (ALL_ERR + 32)){
                    printf("Running all tests\n");
                    generateData(ALL_ERR);
                    runTest(ALL_ERR);
                    return 0;
                }
                inputargs[inputcounter] = toupper(argv[i][j]);
                inputcounter++;
            }
        }


    }else{

        //if there are no CLI parameters, ask the user for which test to run
        printf("\n\r%c-Critical Overcurrent \
                \n\r%c-Critical Overtemp while discharging \
                \n\r%c-Critical Overvolt \
                \n\r%c-Critical Undervolt \
                \n\r%c-Overcurrent \
                \n\r%c-Overtemp while charging \
                \n\r%c-Overtemp while discharging \
                \n\r%c-Overvolt \
                \n\r%c-Undervolt \
                \n\r%c-Generate All Error Tests \
                \n\r%c-No Error Test \
                \n\r%c-All Edge Cases Error Test \
                \n\rSelect a Test to run by typing the corresponding character: ", \
                CRIT_OCURR, CRIT_OTEMP_DSCHRGE, CRIT_OVOLT, CRIT_UVOLT, OCURR, \
                OTEMP_CHRG, OTEMP_DSCHRG, OVOLT, UVOLT, ALL_ERR, NONE, ALL_EDGE);
        char* userinput;
        scanf("%m[^\n]", &userinput); //dynamically allocate memory for user input
        int inputlen = strlen(userinput);
        inputargs = (char *) malloc(sizeof(char) * inputlen); //allocate space for the array
        inputsize = 0;
        for(int i = 0; i < inputlen; i++){
            if(isalnum(userinput[i])){
                if(userinput[i] == ALL_ERR || userinput[i] == (ALL_ERR + 32)){
                    printf("Running all tests\n");
                    generateData(ALL_ERR);
                    runTest(ALL_ERR);
                    return 0;
                }
                inputargs[inputsize] = toupper(userinput[i]);

                inputsize++;
            }
        }
    }

    //Iterate through the collected command line args and run all the specified tests
    for(int i = 0; i < inputsize; i++){
        generateData(inputargs[i]);
        runTest(inputargs[i]);
    }

    return 0;
}


/**
 * @brief   Generates the test json, outputs it to file test.json
 * @param   selection Selects which test to generate
 * @return  None
 */
void generateData(char selection){
    int index, victim; //temperary indexing variables
    //initialization
    switch(selection){
        case OTEMP_CHRG:
            initializeVariables(true);
            break;
        case CRIT_OTEMP_DSCHRGE:
        case OTEMP_DSCHRG:
            initializeVariables(false);
            break;
        default:
            //default case we randomly choose whether charging or discharging
            rand()%2==0 ? initializeVariables(true) : initializeVariables(false);
            break;
    }
    char* filename;
    //now we throw an error into the data so that simulator will trip
    switch (selection){
        case CRIT_OCURR:
            states[0].current = MAX_CURRENT_LIMIT + 1;
            asprintf(&filename, "%c-Critical-OverCurrent", selection);
            break;
        case CRIT_OTEMP_DSCHRGE:
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[0].temperatures[victim] = MAX_DISCHARGE_TEMPERATURE_LIMIT + 1;
            asprintf(&filename, "%c-Critical-DischargingOverTemperature", selection);
            break;
        case CRIT_OVOLT:
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[0].voltages[victim] = (MAX_VOLTAGE_LIMIT + 1)*10;
            asprintf(&filename, "%c-Critical-OverVoltage", selection);
            break;
        case CRIT_UVOLT:
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[0].voltages[victim] = (MIN_VOLTAGE_CHARGING_LIMIT - 1)*10;
            asprintf(&filename, "%c-Critical-UnderVoltage", selection);
            break;
        case OCURR:
            index = randomRange(1,jsonLength);
            states[index].current = MAX_CURRENT_LIMIT + 1;
            asprintf(&filename, "%c-OverCurrent", selection);
            break;
        case OTEMP_CHRG:
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[index].temperatures[victim] = MAX_CHARGE_TEMPERATURE_LIMIT + 1;
            asprintf(&filename, "%c-ChargingOverTemperature", selection);
            break;
        case OTEMP_DSCHRG:
            index = randomRange(1,jsonLength - 1);
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[index].temperatures[victim] = MAX_DISCHARGE_TEMPERATURE_LIMIT + 1;
            asprintf(&filename, "%c-DishargingOverTemperature", selection);
            break;
        case OVOLT:
            index = randomRange(1,jsonLength - 1);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[index].voltages[victim] = (MAX_VOLTAGE_LIMIT + 1)*10;
            asprintf(&filename, "%c-DishargingOverTemperature", selection);
            break;
        case UVOLT:
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[index].voltages[victim] = (MIN_VOLTAGE_CHARGING_LIMIT - 1)*10;
            asprintf(&filename, "%c-Undervoltage", selection);
            break;
        case ALL_ERR:
            for(char i = '0'; i < ALL_ERR; i++){
                generateData(i);
            }
            generateData(NONE);
            break;
        case NONE:
            asprintf(&filename, "%c-NoError", selection);
            break;
        case ALL_EDGE:
            //to do
            break;
        default:
            break;
    }
    if (selection != ALL_ERR) {
        assert(writeOut(filename));
    }
}

/**
 * @brief Initializes the variables needed to create a json file
 * @param charging Whether or not the car is in a charging state
*/
void initializeVariables(bool charging){
    for(int i = 0; i < jsonLength; i++) {    
        states[i].time = 1; //all state times to 1

        states[i].adcHigh = 0;

        states[i].adcLow = 0;

        int voltagecenter = randomRange((MIN_VOLTAGE_LIMIT + BALANCING_TOLERANCE_START) * 10, (MAX_VOLTAGE_LIMIT - BALANCING_TOLERANCE_START) * 10);
        for(int j=0; j<NUM_BATTERY_MODULES; j++) {
            int lowerlimit = voltagecenter - (BALANCING_TOLERANCE_STOP * 10);
            int upperlimit = voltagecenter + (BALANCING_TOLERANCE_STOP * 10);
            states[i].voltages[j] = randomRange(lowerlimit, upperlimit);
        }

        for(int j=0; j<NUM_TEMPERATURE_SENSORS; j++){
            states[i].temperatures[j] = randomRange(0, MAX_CHARGE_TEMPERATURE_LIMIT);
        }

        states[i].current = charging ? randomRange(MAX_CHARGING_CURRENT, 0) : randomRange(0, MAX_CURRENT_LIMIT);

        states[i].charge = 50000000;
    }
}

/**
 * @brief   Writes out a json file using all the variables set earlier
 * @param   filename The name of the json to write out to, filepath and .json extension will be added automatically
 * @return  Whether the creation of json was successful
*/
bool writeOut(char* filename){
    char* filepath;
    char *cwd;
    cwd = getcwd(NULL, 0);
    asprintf(&filepath, "%s%s%s.json", cwd, fp, filename);
    free(cwd);
    printf("Writing out %s\n", filepath);
    FILE *f;
    f = fopen(filepath, "w");
    if (f == NULL) {
        fprintf(stderr, "can't open %s: %s\n", filepath, strerror(errno));
        return false;
    }
    fprintf(f, "[\n");
    for (int i = 0; i < jsonLength; i++) {
        fprintf(f, "    {\n");
        fprintf(f, "        \"time\": %d,\n", states[i].time);
        fprintf(f, "        \"adcHigh\": %d,\n", states[i].adcHigh);
        fprintf(f, "        \"adcLow\": %d,\n", states[i].adcLow);

        //print voltages
        fprintf(f, "        \"voltages\": [\n            ");
        for(int j=0; j<NUM_BATTERY_MODULES-1; j++){
            fprintf(f, "%d, ", states[i].voltages[j]);
        }
        fprintf(f, "%d\n        ],\n", states[i].voltages[NUM_BATTERY_MODULES-1]);

        //print temperatures
        fprintf(f, "        \"temperatures\": [\n            ");
        for(int j=0; j<NUM_TEMPERATURE_SENSORS-1; j++){
            fprintf(f, "%d, ", states[i].temperatures[j]);
        }
        fprintf(f, "%d\n        ],\n", states[i].temperatures[NUM_TEMPERATURE_SENSORS-1]);

        fprintf(f,"        \"current\": %d,\n", states[i].current);
        fprintf(f, "        \"charge\": %d\n", states[i].charge);
        if(i==jsonLength-1){
            fprintf(f,"    }\n"); //don't print a comma on the last iteration
        }else{
            fprintf(f,"    },\n");
        }
    }
    fprintf(f, "]\n");
    fclose(f);

    return writeVerification(filename);
}

bool writeVerification(char* filename){
    char selection = filename[0];

    char* filepath;
    char *cwd;
    cwd = getcwd(NULL, 0);
    asprintf(&filepath, "%s%s%s-out.json", cwd, fp, filename);
    free(cwd);
    printf("Writing out %s\n", filepath);
    FILE *f;
    f = fopen(filepath, "w");
    if (f == NULL) {
        fprintf(stderr, "can't open %s: %s\n", filepath, strerror(errno));
        return false;
    }

    fprintf(f, "{\n    \"forbidden_states\": {\n");
    //if we are in a critical state, forbidden states exist
    if(selection <='4' && selection >= '0'){
        fprintf(f, "        \"Contactor\": [\n            [\"ContactorC1\", \"enabled\"],\n            [\"ContactorCFAN\", \"enabled\"]\n        ]\n");
    }
    fprintf(f, "    },\n");
    fprintf(f, "    \"end_state\": {\n");
    //end state for all problematic states should have contactors open
    if(isdigit(selection)){
        fprintf(f,"        \"Contactor\": [\n            [\"ContactorC1\", \"disabled\"],\n            [\"ContactorCFAN\", \"disabled\"]\n        ],\n");
    }else{
        fprintf(f,"        \"Contactor\": [\n            [\"ContactorC1\", \"enabled\"],\n            [\"ContactorCFAN\", \"enabled\"]\n        ],\n");
    }

    //specific fault state for each test case
    fprintf(f, "        \"Light\": [\n");
    switch(selection){
        case CRIT_OCURR:
        case OCURR:
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"OCURR\", \"1\"]\n");
            break;
        case CRIT_OTEMP_DSCHRGE:
        case OTEMP_CHRG:
        case OTEMP_DSCHRG:
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"OTEMP\", \"1\"]\n");
            break;
        case CRIT_OVOLT:
        case OVOLT:
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"OVOLT\", \"1\"]\n");
            break;
        case CRIT_UVOLT:
        case UVOLT:
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"UVOLT\", \"1\"]\n");
            break;
        case NONE:
            fprintf(f,"            [\"FAULT\", \"0\"]\n");
            break;
    }
    fprintf(f, "        ],\n");

    //fan end state
    fprintf(f, "        \"Fan\": [\n");
    if(isdigit(selection)){ //only the tests with errors will max out the fans
        fprintf(f,"            [\"0\", \"4000\"],\n            [\"1\", \"4000\"],\n            [\"2\", \"4000\"]\n");
    }
    fprintf(f,"        ]\n");

    fprintf(f,"    }\n}");
    fclose(f);

    return true;
}

/**
 * @brief Run the simulator on a specified json file
 * @param input The specific test to run, refer to generateData() to see what each char means
*/
void runTest(char input){
    if (input == ALL_ERR || input == (ALL_ERR + 32)){
        for(char i = '0'; i < ALL_ERR; i++){
            runTest(i);
        }
        runTest(NONE);
    }else{
        // run the specified test
        char* command;
        asprintf(&command, "python3 Validation/verify_test.py `find BSP/Simulator/Data/AutomatedTests/ -name \"%c-*\" ! -name \"*-out.json\"`", input); //generate command
        printf("%s\n", command);
        if(system(command) != 0){
           //check if test was successful, if not we need to throw error and exit
            asprintf(&command, "cat BSP/Simulator/Simulator-Out/%c-*", input);
            system(command);
            asprintf(&command, "echo -e \"The json file used was \n\"; cat BSP/Simulator/Data/AutomatedTests/%c-*", input);
            system(command);
            printf("Error occurred when simulating test %c \n", input);
            exit(-1); 
        } 
    }
}
