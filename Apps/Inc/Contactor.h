 
/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef CONTACTOR_H
#define CONTACTOR_H

#include "BSP_PWM.h" //PWM_Period is defined in here
#ifdef SIMULATION
#include "Simulator.h"
#endif

//Parameters for PWM
#define PWM_ON_TIME 3500

#define CFAN 3
//These are defining how the contactors are wired, the array contactor is assumed to be wired to the fan board
#define ARRAY_CONTACTOR CFAN // right now we are using fan pin 3 as the contactor output
#define HVHIGH_CONTACTOR 4 // contactor 1 on pwm output 4
#define HVLOW_CONTACTOR 4 // contactor 2 on pwm output 4

/**
 * @brief   A Contactor is a high power switch similar to what a relay is. The Contactor
 *          essentially "turns on and off" the whole car.
 */

/**
 * @brief   Initializes the GPIO pins that interfaces with the Contactor.
 *          Two GPIO pins are initialized. One as an output and one as an input.
 *          The output pin controls the state and the input pin views what state the
 *          contactor is through the Aux pins.
 * @param   None
 * @return  None
 */
void Contactor_Init(void);

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 */
void Contactor_On(uint8_t contactorChoice);

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void Contactor_Off(uint8_t contactorChoice);

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool Contactor_GetState(uint8_t contactorChoice);

void Contactor_SetArrayEnable(bool en);

#endif
