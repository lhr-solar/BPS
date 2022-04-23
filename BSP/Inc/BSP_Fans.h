/* Copyright (c) 2020 UT Longhorn Racing Solar */

/* Fans.h
Controls speed of fans which cool down the battery pack. Uses Pins PC6, 7 and PB14, 15
*/
#ifndef BSP_FANS_H
#define BSP_FANS_H

#include "common.h"
#include "config.h"

#define PWM_PERIOD 4000
#define DIVIDER 500 //Speed increments
#define TOPSPEED PWM_PERIOD/DIVIDER //top speed achievable

/**
 * @brief   Initialize all the GPIO pins connected to each Fan
 * @param   None
 * @return  None
 */
void BSP_Fans_Init(void);

/**
 * @brief   Sets fan duty cycle
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 * @param   fan: fan number whose speed should be changed
 * @return  ErrorStatus
 */
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t speed);

/**
 * @brief   Get current speed of specific fan
 * @param   fan Number
 * @return  Current PWM duty cycle if fans
 */
int BSP_Fans_GetSpeed(uint8_t fan);

/**
 * @brief   Sets fan duty cycle for all fans
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 * @return  ErrorStatus
 */
ErrorStatus BSP_Fans_SetAll(uint32_t speed);

#endif
