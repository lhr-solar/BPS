/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_Timer.h"
#include <time.h>
#include "Simulator.h"
#include <stdint.h>
#include <stdlib.h>

#define MICROS_IN_SEC 1000000

static bool initialized = false;
static clock_t t;
/**
 * @brief   Initialize the timer for time measurements.
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void) {
    initialized = true;
    Simulator_Log(LOG_INFO,"Initialized Timer\n");
}

/**
 * @brief   Starts a one shot timer to execute a callback after a certain time
 * 
 * @param delay_us one shot time in microseconds
 * @param callback callback to execute after `delay_us` time
 */
void BSP_Timer_Start_OneShot(uint32_t delay_us, callback_t callback) {
    if (initialized) {
        t = clock();
        Simulator_Log(LOG_INFO, "One-shot timer started (pretend this triggers after some time)\n");
    }
    else {
        Simulator_Log_Location(LOG_INFO,"Hard Fault: Initialize Timer before start\n");
    }
}

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start_TickCounter(void) {
    if (initialized) {
        t = clock();
        Simulator_Log(LOG_INFO,"Timer started\n");
    }
    else {
        Simulator_Log_Location(LOG_INFO,"Hard Fault: Initialize Timer before start\n");
    }
}

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void) {
    if(!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used PWM without initialization!\n");
        exit(-1);
    }

    uint32_t currentTime = clock();
    uint32_t TicksElasped = currentTime - t;
    char TimeMessage[100] = {0};
    sprintf(TimeMessage, "Ticks Elasped: %d ticks\n", TicksElasped);
    Simulator_Log(LOG_INFO, TimeMessage);
    t = currentTime; // reassign so we track time between calls, not since program start
    return TicksElasped;
}

/**
 * @note    Function not implemented so returns 0
 * @brief   Returns 0
 * @param   None
 * @return  0
 */
uint32_t BSP_Timer_GetRunFreq(void) {
    if(!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used PWM without initialization!\n");
        exit(-1);
    }
    return 0;
}

/**
 * @brief   Gives a standard unit for time elapsed in microseconds since calling BSP_Timer_GetTicksElapsed()
 * @param   None
 * @return  Microseconds 
 */
uint32_t BSP_Timer_GetMicrosElapsed(void) {
    if(!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used PWM without initialization!\n");
        exit(-1);
    }
    uint32_t MicrosElasped = ((uint64_t) BSP_Timer_GetTicksElapsed() * MICROS_IN_SEC) / CLOCKS_PER_SEC;
    char MicrosMessage[100] = {0};
    sprintf(MicrosMessage, "Microseconds elasped: %d us\n", MicrosElasped);
    Simulator_Log(LOG_INFO, MicrosMessage);
    return (uint32_t) MicrosElasped;
}
