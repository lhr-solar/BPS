/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef BSP_TIMER_H
#define BSP_TIMER_H

#include "common.h"

/**
 * @brief   Initialize timers
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void);

/**
 * @brief   Starts a one shot timer to execute a callback after a certain time
 * 
 * @param delay_us one shot time in microseconds
 * @param callback callback to execute after `delay_us` time
 */
void BSP_Timer_Start_OneShot(uint32_t delay_us, callback_t callback);

/**
 * @brief   Starts the tick counter timer
 * @param   None
 * @return  None
 */
void BSP_Timer_Start_TickCounter(void);

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
uint32_t BSP_Timer_GetMicrosElapsed(void);

#endif
