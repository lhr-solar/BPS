#include "BSP_ADC.h"

/**
 * @brief   Initializes the ADC module. This is to measure the hall effect sensors
 *          on the Current Monitor Board.
 * @param   None
 * @return  None
 */
void BSP_ADC_Init(void) {
    // TODO: Initialize the ADC port here
}

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_High_GetMilliVoltage(void) {

    // TODO: Get ADC value and convert
    // Get ADC raw data
    // uint16_t data = ADC_Data();
    
    // Convert to millivoltage
    // uint16_t mV = (data * 3300) >> 12;   // For 12-bit ADCs

    return 0;
}

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_Low_GetMilliVoltage(void) {

    // TODO: Get ADC value and convert
    // Get ADC raw data
    // uint16_t data = ADC_Data();
    
    // Convert to millivoltage
    // uint16_t mV = (data * 3300) >> 12;   // For 12-bit ADCs

    return 0;
}
