/* Copyright (c) 2022 UT Longhorn Racing Solar */

#ifndef BSP_CONTACTOR_H
#define BSP_CONTACTOR_H

#include "common.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

typedef uint8_t CONT_CHOICE;
typedef enum {ARRAY_CONTACTOR = 1, HVHIGH_CONTACTOR = 2, HVLOW_CONTACTOR = 4, ALL_CONTACTORS = 7} Contactors_t;

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
void BSP_Contactor_Init(void);

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.

 */
void BSP_Contactor_On(Contactors_t contactorChoice);

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(Contactors_t contactorChoice);

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(Contactors_t contactorChoice);

#endif
