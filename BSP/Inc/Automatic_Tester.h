/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef AUTOMATIC_TESTER_H
#define AUTOMATIC_TESTER_H
#define jsonLength 5 //length of the generated jsonfiles

#include "config.h"

struct State {
    int time;
    int adcHigh;
    int adcLow;
    int voltages[NUM_BATTERY_MODULES];
    int temperatures[NUM_TEMPERATURE_SENSORS];
    int current;
    int charge;
};

typedef enum {
    CRIT_OCURR = '0',
    CRIT_OTEMP_DSCHRGE = '1',
    CRIT_OVOLT = '2',
    CRIT_UVOLT = '3',
    OCURR = '4',
    OTEMP_CHRG = '5',
    OTEMP_DSCHRG = '6',
    OVOLT = '7',
    UVOLT = '8',
    ALL_ERR = '9',
    NONE = 'A',
    ALL_EDGE = 'B'
} ERR_t;

/**
 * @brief The output of this file automates the generation of test files and running them using the simulator
 * @param argv CLI inputs that specify which test(s) to run. 
 * @note Example: ./Test_Generator 123 will generate and run tests 1, 2, 3
 * @note If no command line arguments are supplied, you will have to type them after the program starts running
 * 
*/
int main(int argc, char **argv);

/**
 * @brief   Generates the test json, outputs it to file test.json
 * @param   selection Selects which test to generate
 * @return  None
 */
void generateData(char selection);

/**
 * @brief Initializes the variables needed to create a json file
 * @param charging Whether or not the car is in a charging state
*/
void initializeVariables(bool charging);

/**
 * @brief   Writes out a json file using all the variables set earlier
 * @param   filename The name of the json to write out to, filepath and .json extension will be added automatically
 * @return  Whether the creation of json was successful
*/
bool writeOut(char* filename);

/**
 * @brief Run the simulator on a specified json file
 * @param input The specific test to run, refer to generateData() to see what each char means
*/
void runTest(char input);

/**
 * @brief Writes the expected output of the simulator
 * @param filename The filename that the output file will have
 * @note The method will automatically which test generate the expected output by the filename
*/
bool writeVerification(char* filename);

#endif
