/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef BSP_PLL_H
#define BSP_PLL_H

#include "common.h"
#include "config.h"

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
ErrorStatus BSP_PLL_Init(void);

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void);

/**
 * @brief   Delays in param milli seconds
 * @param   # of milliseconds to delay
 * @return  None
*/
void BSP_PLL_DelayM(uint32_t milli);

/**
 * @brief   Delays in param micro seconds
 * @param   # of microseconds to delay
 * @return  None
*/
void BSP_PLL_DelayU(uint32_t micro);

#endif
