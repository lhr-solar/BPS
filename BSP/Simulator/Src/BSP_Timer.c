/* Copyright (c) 2022 UT Longhorn Racing Solar */

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
    Simulator_log("Initialized Timer\n");
}

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start(void) {
    if (initialized) {
        t = clock();
        Simulator_log("Timer started\n");
    }
    else {
        Simulator_log("Hard Fault: Initialize Timer before start\n");
    }
}

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void) {
    return clock() - t;
    
}

/**
 * @note    Function not implemented so returns 0
 * @brief   Returns 0
 * @param   None
 * @return  0
 */
uint32_t BSP_Timer_GetRunFreq(void) {
    return 0;
}

/**
 * @brief   Gives a standard unit for time elapsed in microseconds since calling BSP_Timer_GetTicksElapsed()
 * @param   None
 * @return  Microseconds 
 */
uint32_t BSP_Timer_GetMicrosElapsed(void) {
    return (uint32_t) (BSP_Timer_GetTicksElasped() * MICROS_IN_SEC) / CLOCKS_PER_SEC;
}
