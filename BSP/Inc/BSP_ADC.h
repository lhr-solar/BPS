/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "common.h"

//return this if the ADC is not available
#define BAD_ADC 0xffff

/**
 * @brief   Initializes the ADC module. This is to measure the hall effect sensors
 *          on the Current Monitor Board.
 * @param   None
 * @return  None
 */
void BSP_ADC_Init(void);

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_High_GetMilliVoltage(void);

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_Low_GetMilliVoltage(void);

#endif
/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "common.h"

//return this if the ADC is not available
#define BAD_ADC 0xffff

/**
 * @brief   Initializes the ADC module. This is to measure the hall effect sensors
 *          on the Current Monitor Board.
 * @param   None
 * @return  None
 */
void BSP_ADC_Init(void);

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_High_GetMilliVoltage(void);

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_Low_GetMilliVoltage(void);

#endif
