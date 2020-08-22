/* Fans.h
Controls speed of fans which cool down the battery pack.
Uses Pins PC6,7 and PB14,15
*/
#ifndef FANS_H__
#define FANS_H__

#include "common.h"
#include "config.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

//This function will initialize the pins and timer used to control the fans
void BSP_Fans_Init(void);

/*This function will change the speed of the fans
Inputs: Number of fan to change speed (1-4)
        Speed of fan (0-8)
Output: Error Status
        0: Gave invalid fan value
        1: Gave invalid velocity value
*/
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t speed);

/* Get current speed of specific fan
Input: Fan number
*/
int BSP_Fans_GetSpeed(uint8_t fan);

#endif