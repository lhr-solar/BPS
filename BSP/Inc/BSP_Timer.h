/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef BSP_TIMER_H
#define BSP_TIMER_H

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

/**
 * @brief   Calculates the microseconds passed since last calling BSP_TimerGetTicksElapsed()
 * @param   None
 * @return  Microseconds
 */
uint32_t BSP_Timer_GetMicrosElapsed();

#endif
