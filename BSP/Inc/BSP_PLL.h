/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __BSP_PLL_H
#define __BSP_PLL_H

#include "common.h"

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void);

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void);

#endif
/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __BSP_PLL_H
#define __BSP_PLL_H

#include "common.h"

/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void);

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void);

#endif
