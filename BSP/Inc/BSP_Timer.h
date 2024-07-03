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
 * 
 * @note Calling this multiple times concurrently results in undefined behavior.
 *       Define more one shot timers if >1 are needed concurrently. 
 *       Look at RTOS_BPS_DelayUs() for an example use case
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
 * @brief   Convert ticks to micros. Call in conjunction with BSP_Timer_GetTicksElapsed().
 * @param   ticks Timer tick value
 * @return  Microseconds 
 */
uint32_t BSP_Timer_TicksToMicros(uint32_t ticks);

#endif
