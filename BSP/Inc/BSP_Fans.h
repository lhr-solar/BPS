/* Fans.h
Controls speed of fans which cool down the battery pack.
Uses Pins PC6,7 and PB14,15
*/
#ifndef __BSP_FANS_H
#define __BSP_FANS_H

#include "common.h"
#include "config.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

/**
 * @brief   Initialize all the GPIO pins connected to each Fan
 * @param   None
 * @return  None
 */
void BSP_Fans_Init(void);

/**
 * @brief   Sets fan duty cycle
 * @param   dutyCycle: int for duty cycle amount in range 0-8
 *          fan: fan number whose speed should be changed
 * @return  None
 */
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t speed);

/**
 * @brief   Get current speed of specific fan
 * @param   fan Number
 * @return  Current PWM duty cycle if fans
 */
int BSP_Fans_GetSpeed(uint8_t fan);

#endif
