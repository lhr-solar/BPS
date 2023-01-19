/* Copyright (c) 2022 UT Longhorn Racing Solar */

/** 
 * Simulator.h - Simulator Functionality
*/

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "config.h"

// Used for simulator logging and fault states
// See Fault State files
#include "Tasks.h"
extern uint32_t Fault_BitMap;
extern void EnterFaultState();

// Funny macros from Tianda
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') + 1)

// struct for managing what the simulator should read if it does I/O
typedef struct simulator_state {
    uint32_t time;          // how long to stay in this state
    // the readings when we are in this state
    uint16_t adcHigh;
    uint16_t adcLow;
    uint16_t voltages[NUM_BATTERY_MODULES];
    int32_t temperatures[NUM_TEMP_SENSORS_PER_MOD * NUM_BATTERY_MODULES];
    int32_t current;
    uint32_t charge; // in units of 1 million = 1%
    struct simulator_state *next;
} simulator_state;

typedef enum {LOG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_OUTPUT, LOG_MISC, LOG_NUM_LEVELS} LoggingType_t;

// initialize the simulator
void Simulator_Init(char *jsonPath);

// log something to the simualtor's log file
void Simulator_Log(LoggingType_t lvl, char *str);


#define Simulator_Log_Location(lvl, str)    \
    do {                                    \
    Simulator_Log(lvl, __FILE__);           \
    Simulator_Log(LOG, "\t");               \
    Simulator_Log(LOG, str);                \
    } while (0)

// shut down the simulator
void Simulator_Shutdown(int status);

// get the adcHigh
uint16_t Simulator_getAdcHigh(void);

// get the adcLow
uint16_t Simulator_getAdcLow(void);

// get the voltages
uint16_t Simulator_getVoltage(int i);

// get a single temperature sensor
int32_t Simulator_getTemperature(int i);

// get the current
int32_t Simulator_getCurrent(void);

// get the charge (stored in the EEPROM)
uint32_t Simulator_getCharge(void);

#endif
