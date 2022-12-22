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

static struct State state[jsonLength];


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
void generateTest(char selection){
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

    switch (selection){
        case '0':
            state[0].current = MAX_CURRENT_LIMIT + 1;
            writeOut("Test-Critical-OverCurrent.json");
            break;
        case '1':
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            state[0].temperatures[victim] = MAX_CHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("Test-Critical-ChargingOverTemperature.json");
            break;
        case '2':
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            state[0].temperatures[victim] = MAX_DISCHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("Test-Critical-DischargingOverTemperature.json");
            break;
        case '3':
            victim = randomRange(0, NUM_BATTERY_MODULES);
            state[0].voltages[victim] = MAX_VOLTAGE_LIMIT + 1;
            writeOut("Test-Critical-OverVoltage");
            break;
        case '4':
            victim = randomRange(0, NUM_BATTERY_MODULES);
            state[0].voltages[victim] = MIN_VOLTAGE_CHARGING_LIMIT - 1;
            writeOut("Test-Critical-UnderVoltage");
            break;
        case '5':
            index = randomRange(1,jsonLength);
            state[index].current = MAX_CURRENT_LIMIT + 1;
            writeOut("Test-OverCurrent");
            break;
        case '6':
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            state[0].temperatures[victim] = MAX_CHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("Test-ChargingOverTemperature");
            break;
        case '7':
            index = randomRange(1,jsonLength - 1);
            victim = randomRange(0, NUM_TEMPERATURE_SENSORS);
            state[0].temperatures[victim] = MAX_DISCHARGE_TEMPERATURE_LIMIT + 1;
            writeOut("Test-DischargingOverTemperature");
            break;
        case '8':
            index = randomRange(1,jsonLength - 1);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            state[0].voltages[victim] = MAX_VOLTAGE_LIMIT + 1;
            writeOut("Test-OverVoltage");
            break;
        case '9':
            index = randomRange(1,jsonLength);
            victim = randomRange(0, NUM_BATTERY_MODULES);
            state[index].voltages[victim] = MIN_VOLTAGE_CHARGING_LIMIT - 1;
            writeOut("Test-UnderVoltage");
            break;
        case 'A':
        case 'a':
            for(char i='0'; i<='9'; i++){
                generateTest(i);
            }
            break;
        case 'B':
        case 'b':
            writeOut("Test-NoError.json");
            break;
        case 'C':
        case 'c':
            //to do
            break;
        default:
            break;
    }
}

void initializeVariables(bool charging){
    int flipcurrent = charging ? -1 : 1; //we need to set the curreent to a negative value if the car is charging
    for(int i=0; i<jsonLength; i++) {    
        state[i].time = 10+2*i; //first state time is 10, increase by 2 for each state

        state[i].adcHigh = 0;

        state[i].adcLow = 0;

        for(int j=0; j<NUM_BATTERY_MODULES; j++) {
            state[i].voltages[j] = randomRange(MIN_VOLTAGE_LIMIT, MAX_VOLTAGE_LIMIT);
        }

        for(int j=0; j<NUM_TEMPERATURE_SENSORS; j++){
            state[i].temperatures[j] = randomRange(0, MAX_CHARGE_TEMPERATURE_LIMIT);
        }

        state[i].current = flipcurrent * randomRange(0, MAX_CURRENT_LIMIT);

        state[i].charge = 50000000;
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

bool writeOut(char* filename){
    FILE *f;
    f = fopen(filename, "w");
    fprintf(f, "[\n");
    for (int i=0; i<jsonLength; i++) {
        fprintf(f, "    {\n");
        fprintf(f, "        \"time\": %d,\n", state[i].time);
        fprintf(f, "        \"adcHigh\": %d,\n", state[i].adcHigh);
        fprintf(f, "        \"adcLow\": %d,\n", state[i].adcLow);

        //print voltages
        fprintf(f, "        \"voltages\": [\n            ");
        for(int j=0; j<NUM_BATTERY_MODULES-1; j++){
            fprintf(f, "%d, ", state[i].voltages[j]);
        }
        fprintf(f, "%d\n]        ],\n", state[i].voltages[NUM_BATTERY_MODULES]);

        //print temperatures
        fprintf(f, "        \"temperatures\": [\n            ");
        for(int j=0; j<NUM_TEMPERATURE_SENSORS-1; j++){
            fprintf(f, "%d, ", state[i].temperatures[j]);
        }
        fprintf(f, "%d\n]        ],\n", state[i].temperatures[NUM_TEMPERATURE_SENSORS]);

        fprintf(f,"        \"current\": %d,\n", state[i].current);
        fprintf(f, "        \"charge\": %d\n", state[i].charge);
        fprintf(f,"    }\n");
    }
    fprintf(f, "]\n");

    fclose(f);
    return false;
}

void runTests(int input){
    switch (input){
        case 0:
            // run all tests that are generated using this script using bash
            system("values=($(ls BSP/Simulator/Data/Test-*)) && for i in \"${values[@]}\"; do echo -e \"\\e[0;32mNow running simulation with $i\\e[0m\";./bps-simulator.out $i; done");
            break;
        default:
            // run the most recently edited test, which will be the one that was generated earlier
            system("./bps-simulator.out BSP/Simulator/Data/`ls -t BSP/Simulator/Data/ | head -n 1`") ;//run the most recently edited file
    }
}