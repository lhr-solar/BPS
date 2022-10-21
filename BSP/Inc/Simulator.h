// TODO: uncomment this before pushing
//#ifdef SIMULATION

#ifndef SIMULTOR_H
#define SIMULATOR_H

#include "config.h"

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

typedef enum {LOG, INFO, WARN, ERROR, MAXLEVEL} LoggingType;

static const struct LoggingLvl LoggingLUT[MAXLEVEL] = {
    [LOG] = "",
    [INFO] = "INFO: ",
    [WARN] = "WARNING: ",
    [ERROR] = "ERROR: "
}
// intialize the simulator
void Simulator_init(char *jsonPath);

// log something to the simualtor's log file
void Simulator_log(char *str);

// log an error to the simulator's log file
void Simulator_error(char *str);

// log an info message to the simulator's log file
void Simualtor_info(char *str);

// log a warning to the simulator's log file
void Simulator_warning(char *str);

// shut down the simulator
void Simulator_shutdown(int status);

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

//#endif
