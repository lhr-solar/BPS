/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * BSP_ADC.c - Simulates ADC of BPS 
 */

#include "BSP_ADC.h"
#include "Simulator.h"

static bool initialized = false;

/**
 * @brief   Initializes the ADC module. This is to measure the hall effect sensors
 *          on the Current Monitor Board.
 * @param   None
 * @return  None
 */
void BSP_ADC_Init(void) {
    initialized = true;
    Simulator_Log(LOG_INFO, "ADC initialized\n");
}

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_High_GetMilliVoltage(void) {
    // TODO
    return 42;
}

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_Low_GetMilliVoltage(void) {
    // TODO
    return 42;
}
