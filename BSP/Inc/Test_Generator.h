#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H
#define jsonLength 10 //length of the generated jsonfiles

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

void generateTest(char selection);

void initializeVariables(bool charging);

/**
 * @brief   Generates random numbers between a range
 * @param   lower The lower bound, inclusive
 * @param   upper The upper bound, exclusive
 * @return  Random number between lower and upper
 */
int randomRange(int lower, int upper);

bool writeOut(char* filename);

void runTests(int input);

#endif