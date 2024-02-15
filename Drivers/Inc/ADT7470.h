/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** ADT740.h
 *  Functions for using the ADT740
 */

#ifndef ADT740_H
#define ADT740_H

#include "common.h"
#include "config.h"

/*
* ADT740_Init initializes I2C to communicate with ADT740
*/
void ADT740_Init(void);

/*
* Returns the rpm of the given fan
*/
int16_t getFanSpeed(uint8_t fan);

/*
* Sets the PWM duty cycle of the given fan
*/
ErrorStatus setFanPWM(uint8_t fan, int32_t pwm);

/*
* Puts the ADT740 into shutdown mode
*/
void enterShutdown();

#endif