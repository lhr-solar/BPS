/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "common.h"

/**
 * @brief   Initialize the timer for time measurements.
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void);

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start(void);

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void);

/**
 * @brief   Gets the running frequency of the timer (time per tick)
 * @param   None
 * @return  frequency in Hz
 */
uint32_t BSP_Timer_GetRunFreq(void);

#endif
