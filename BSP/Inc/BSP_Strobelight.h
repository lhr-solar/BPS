/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef BSP_STROBELIGHT_H
#define BSP_STROBELIGHT_H

#include "common.h"
#include "config.h"
#include <sys/file.h>

/**
 * @brief   Create text file if none exists (Simulator)
 * @param   None
 * @return  None
 */
void BSP_Strobe_Init(void);

/**
 * @brief   Turns on Strobe (pulse once per second)
 * @param   None
 * @return  None
 */
void BSP_Strobe_On(void);

/**
 * @brief   Turns off Strobe
 * @param   None
 * @return  None
 */
void BSP_Strobe_Off(void);

#endif
