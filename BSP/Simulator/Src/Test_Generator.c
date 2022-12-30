#include "Test_Generator.h"
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

static struct State states[jsonLength];

/**
 * @brief The output of this file automates the generation of test files and running them using the simulator
 * @param argv CLI inputs that specify which test(s) to run. 
 * @note Example: ./Test_Generator 123 will generate and run tests 1, 2, 3
 * @note If no command line arguments are supplied, you will have to type them after the program starts running
 * 
*/
int main(int argc, char **argv){
    //seed the RNG
    srand(time(NULL));

    //argc more than 1 means the user has passed arguments that will be processed
    if(argc > 1){
        //allocate space for all the arguments that the user typed
        char* strptr = *(argv + argc - 1); //starting address of the last argument
        //search for null terminator of the last argument
        while(*strptr != '\0'){
            strptr++;
        }

        int inputsize =(int) (strptr - *(argv + 1) - (argc - 2)); //don't wanna count null terminators so we subtract out number of command line arguments
        char* inputargs = (char *)malloc(inputsize);
        //printf("Allocataing %d locations", inputsize);
        

        //add all tests to our inputargs array
        int* j = (int *)malloc(sizeof(int));
        *j = 0; //j will keep track of the number of while loop iterations
        for(int i=1; i<argc; i++){
            strptr = *(argv + i);
            while (*strptr != '\0'){
                //iterating through all the command line arguments to see if a was typed, because then we just run all the tests
                if(*strptr == 'A' || *strptr == 'a'){
                    printf("Running all tests\n");
                    generateData('a');
                    runTest('a');
                    return 0;
                }
                *(inputargs + *j) = *strptr;
                strptr++;
                (*j)++;
            }
        }
        free(j);

        //Iterate through the collected command line args and run all the specified tests
        for(int i=0; i<inputsize; i++){
            generateData(*(inputargs + i));
            runTest(*(inputargs + i));
        }
    }else{
        //if there are no CLI parameters, ask the user for which test to run
        printf("Select a Test to run by typing the corresponding character \n0-Critical Overcurrent\n1-Critical Overtemp while charging\n2-Critical Overtemp while discharging\n3-Critical Overvolt\n4-Critical Undervolt\n5-Overcurrent\n6-Overtemp while charging\n7-Overtemp while discharging\n8-Overvolt\n9-Undervolt\nA-Generate All Error Tests\nB-No Error Test\nC-All Edge Cases Error Test\n");

        char userinput = getchar();
        generateData(userinput);
        runTest(userinput);
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
            states[0].voltages[victim] = MAX_VOLTAGE_LIMIT + 1;
            writeOut("3-Critical-OverVoltage");
            break;
        case '4':
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[0].voltages[victim] = MIN_VOLTAGE_CHARGING_LIMIT - 1;
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
            states[index].voltages[victim] = MAX_VOLTAGE_LIMIT + 1;
            writeOut("8-OverVoltage");
            break;
        case '9':
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            states[index].voltages[victim] = MIN_VOLTAGE_CHARGING_LIMIT - 1;
            writeOut("9-UnderVoltage");
            break;
        case 'A':
        case 'a':
            for(char i='0'; i<='9'; i++){
                generateData(i);
            }
            break;
        case 'B':
        case 'b':
            writeOut("NoError.json");
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
    int flipcurrent = charging ? -1 : 1; //we need to set the current to a negative value if the car is charging
    for(int i=0; i<jsonLength; i++) {    
        states[i].time = 10; //all state times to 10

        states[i].adcHigh = 0;

        states[i].adcLow = 0;

        for(int j=0; j<NUM_BATTERY_MODULES; j++) {
            states[i].voltages[j] = randomRange(MIN_VOLTAGE_LIMIT, MAX_VOLTAGE_LIMIT);
        }

        for(int j=0; j<NUM_TEMPERATURE_SENSORS; j++){
            states[i].temperatures[j] = randomRange(0, MAX_CHARGE_TEMPERATURE_LIMIT);
        }

        states[i].current = flipcurrent * randomRange(0, MAX_CURRENT_LIMIT);

        states[i].charge = 50000000;
    }
}

/**
 * @brief   Generates random numbers between a range
 * @param   lower The lower bound, inclusive
 * @param   upper The upper bound, exclusive
 * @return  Random number between lower and upper
 */
int randomRange(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

/**
 * @brief   Writes out a json file using all the variables set earlier
 * @param   filename The name of the json to write out to, filepath and .json extension will be added automatically
 * @return  Whether the creation of json was successful
*/
bool writeOut(char* filename){
    char* filepath;
    asprintf(&filepath, "BSP/Simulator/Data/%s.json", filename);
    printf("Writing out %s\n", filepath);
    FILE *f;
    f = fopen(filepath, "w");
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
        fprintf(f, "%d\n        ],\n", states[i].voltages[NUM_BATTERY_MODULES]);

        //print temperatures
        fprintf(f, "        \"temperatures\": [\n            ");
        for(int j=0; j<NUM_TEMPERATURE_SENSORS-1; j++){
            fprintf(f, "%d, ", states[i].temperatures[j]);
        }
        fprintf(f, "%d\n        ],\n", states[i].temperatures[NUM_TEMPERATURE_SENSORS]);

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
    }else{
        // run the specified test
        char* command;
        asprintf(&command, "./bps-simulator.out `ls BSP/Simulator/Data/%c-*`", input); //generate command
        system(command); //run command
    }
}