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
void FansInit(void);

/*This function will change the speed of the fans
Inputs: Number of fan to change speed (1-4)
        Increase or decrease speed (1 or -1)
*/
void FanChangeSpeed(uint8_t fan, int8_t accel);

#endif