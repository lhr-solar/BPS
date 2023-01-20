/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/* Fans.h
Controls speed of fans which cool down the battery pack. Uses Pins PC6, 7 and PB14, 15
*/
#ifndef FANS_H
#define FANS_H

#include "BSP_PWM.h" 
#include "common.h"
#include "config.h"
#include "Simulator.h"

#define DIVIDER 500 //Speed increments TODO: change divider to 100 and have more speeds
#define TOPSPEED PWM_PERIOD/DIVIDER //top speed achievable

/**
 * @brief   Initialize all the GPIO pins connected to each Fan
 * @param   None
 * @return  None
 */
void Fans_Init(void);

/**
 * @brief This function will change the speed of the fans
 * @param fan Number of fan to change speed (1-4)
 * @param speed Speed of Fan(0-TOPSPEED)
 * @return ErrorStatus will return 1 if successful, 0 if error occurred
 */
ErrorStatus Fans_Set(uint8_t fan, uint32_t speed);

/**
 * @brief   Reads the duty cycle for a specified fan
 * @param   fan Gets the duty cycle for this fan
 * @return  int representation of the fan speed from 0-8, -1 if an error occurred
 */
int Fans_GetSpeed(uint8_t fan);

/**
 * @brief   Sets fan duty cycle for all fans
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 * @return  ErrorStatus
 */
ErrorStatus Fans_SetAll(uint32_t speed);

#endif
