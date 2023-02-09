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

/**
 * @brief   Generates random numbers between a range
 * @param   lower The lower bound, inclusive
 * @param   upper The upper bound, exclusive
 * @return  Random number between lower and upper
 */
#define randomRange(lower, upper) ((rand() % (upper - lower)) + lower)

static struct State states[jsonLength];

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
        for(int i=1; i<argc; i++){
            for(int j=0; argv[i][j] != '\0'; j++){
                if(argv[i][j] == 'A' || argv[i][j] == 'a'){
                    printf("Running all tests\n");
                    generateData('a');
                    runTest('a');
                    return 0;
                }
                inputargs[inputcounter] = toupper(argv[i][j]);
                inputcounter++;
            }
        }


    }else{

        //if there are no CLI parameters, ask the user for which test to run
        printf("\n0-Critical Overcurrent\n1-Critical Overtemp while charging\n2-Critical Overtemp while discharging\n3-Critical Overvolt\n4-Critical Undervolt\n5-Overcurrent\n6-Overtemp while charging\n7-Overtemp while discharging\n8-Overvolt\n9-Undervolt\nA-Generate All Error Tests\nB-No Error Test\nC-All Edge Cases Error Test\nSelect a Test to run by typing the corresponding character:");
        char* userinput;
        scanf("%m[^\n]", &userinput); //dynamically allocate memory for user input
        int inputlen = strlen(userinput);
        inputargs = (char *) malloc(sizeof(char) * inputlen); //allocate space for the array
        inputsize = 0;
        for(int i=0; i<inputlen; i++){
            if(isalnum(userinput[i])){
                if(userinput[i] == 'A' || userinput[i] == 'a'){
                    printf("Running all tests\n");
                    generateData('a');
                    runTest('a');
                    return 0;
                }
                inputargs[inputsize] = toupper(userinput[i]);

                inputsize++;
            }
        }
    }

    //Iterate through the collected command line args and run all the specified tests
    for(int i=0; i<inputsize; i++){
        generateData(inputargs[i]);
        runTest(inputargs[i]);
    }

    return 0;
}


/**
 * @brief   Generates the test json, outputs it to file test.json
 * @param   selection Selects which test to generate
 *          0-Critical Overcurrent
 *          1-Critical Overtemp while charging
 *          2-Critical Overtemp while discharging
 *          3-Critical Overvolt
 *          4-Critical Undervolt
 *          5-Overcurrent
 *          6-Overtemp while charging
 *          7-Overtemp while discharging
 *          8-Overvolt
 *          9-Undervolt
 *          A-Generate All Error Tests
 *          B-No Error Test
 *          C-All Edge Cases Error Test
 * @return  None
 */
void generateData(char selection){
    int index, victim; //temperary indexing variables
    //initialization
    switch(selection){
        case '1':
        case '6':
            initializeVariables(true);
            break;
        case '2':
        case '7':
            initializeVariables(false);
            break;
        default:
            //default case we randomly choose whether charging or discharging
            rand()%2==0 ? initializeVariables(true) : initializeVariables(false);
            break;
    }

    //now we throw an error into the data so that simulator will trip
    switch (selection){
        case '0':
            states[0].current = MAX_CURRENT_LIMIT + 1;
            writeOut("0-Critical-OverCurrent");
            break;
        case '1':
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[0].temperatures[victim] = MAX_CHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("1-Critical-ChargingOverTemperature");
            break;
        case '2':
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[0].temperatures[victim] = MAX_DISCHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("2-Critical-DischargingOverTemperature");
            break;
        case '3':
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[0].voltages[victim] = (MAX_VOLTAGE_LIMIT + 1)*10;
            writeOut("3-Critical-OverVoltage");
            break;
        case '4':
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[0].voltages[victim] = (MIN_VOLTAGE_CHARGING_LIMIT - 1)*10;
            writeOut("4-Critical-UnderVoltage");
            break;
        case '5':
            index = randomRange(1,jsonLength);
            states[index].current = MAX_CURRENT_LIMIT + 1;
            writeOut("5-OverCurrent");
            break;
        case '6':
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[index].temperatures[victim] = MAX_CHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("6-ChargingOverTemperature");
            break;
        case '7':
            index = randomRange(1,jsonLength - 1);
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            states[index].temperatures[victim] = MAX_DISCHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("7-DischargingOverTemperature");
            break;
        case '8':
            index = randomRange(1,jsonLength - 1);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[index].voltages[victim] = (MAX_VOLTAGE_LIMIT + 1)*10;
            writeOut("8-OverVoltage");
            break;
        case '9':
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[index].voltages[victim] = (MIN_VOLTAGE_CHARGING_LIMIT - 1)*10;
            writeOut("9-UnderVoltage");
            break;
        case 'A':
        case 'a':
            for(char i='0'; i<='9'; i++){
                generateData(i);
            }
            generateData('B');
            break;
        case 'B':
        case 'b':
            writeOut("B-NoError");
            break;
        case 'C':
        case 'c':
            //to do
            break;
        default:
            break;
    }
}

/**
 * @brief Initializes the variables needed to create a json file
 * @param charging Whether or not the car is in a charging state
*/
void initializeVariables(bool charging){
    for(int i=0; i<jsonLength; i++) {    
        states[i].time = 10; //all state times to 10

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
    asprintf(&filepath, "BSP/Simulator/Data/AutomatedTests/%s.json", filename);
    printf("Writing out %s\n", filepath);
    FILE *f;
    f = fopen(filepath, "w");
    if(f == 0){
        return false; //throw error if file is not openable
    }
    fprintf(f, "[\n");
    for (int i=0; i<jsonLength; i++) {
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
    asprintf(&filepath, "BSP/Simulator/Data/AutomatedTests/%s-out.json", filename);
    printf("Writing out %s\n", filepath);
    FILE *f;
    f = fopen(filepath, "w");
    if(f == 0){
        return false; //throw error if file is not openable
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
        case '0':
        case '5':
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"OCURR\", \"1\"]\n");
            break;
        case '1':
        case '2':
        case '6':
        case '7':
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"OTEMP\", \"1\"]\n");
            break;
        case '3':
        case '8':
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"OVOLT\", \"1\"]\n");
            break;
        case '4':
        case '9':
            fprintf(f,"            [\"FAULT\", \"1\"],\n            [\"UVOLT\", \"1\"]\n");
            break;
        case 'B':
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
    if (input == 'a' || input == 'A'){
        for(char i='0'; i<='9'; i++){
            runTest(i);
        }
        runTest('B');
    }else{
        // run the specified test
        char* command;
        asprintf(&command, "python3 Validation/verify_test.py `ls BSP/Simulator/Data/AutomatedTests/%c-* | head -n 1`", input); //generate command
        printf("%s\n", command);
        if(system(command) != 0){
           //check if test was successful, if not we need to throw error and exit
           system("cat BSP/Simulator/Simulator-Out/%c-*");
           system("echo -e \"The json file used was \n\"; cat BSP/Simulator/Data/AutomatedTests/%c-*");
           exit(-1); 
        } 
    }
}