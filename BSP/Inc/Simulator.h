// TODO: uncomment this before pushing
//#ifdef SIMULATION

#ifndef SIMULTOR_H
#define SIMULATOR_H

#include "config.h"

// struct for managing what the simulator should read if it does I/O
struct simulator_state {
    uint32_t time;          // how long to stay in this state
    // the readings when we are in this state
    uint16_t adcHigh;
    uint16_t adcLow;
    uint16_t voltages[NUM_BATTERY_MODULES];
    int32_t temperatures[NUM_TEMP_SENSORS_PER_MOD * NUM_BATTERY_MODULES];
    int32_t current;
};

// intialize the simulator
void Simulator_init(void);

// log something to the simualtor's log file
void Simulator_log(char *str);

#endif

//#endif
