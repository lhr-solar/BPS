/* Copyright (c) 2022 UT Longhorn Racing Solar */

#ifndef BSP_CONTACTOR_H
#define BSP_CONTACTOR_H

#include "common.h"

// implement me later
typedef enum {
    ARRAY_CONTACTOR,
    LOAD_CONTACTOR,
    HVLOW_CONTACTOR
} Contactor;

#define CONTACTORS_ALL (ARRAY_CONTACTOR | LOAD_CONTACTOR | HVLOW_CONTACTOR)

// Contactor 1
#define C1_PORT		GPIOB
#define C1_PERIPH 	RCC_AHB1Periph_GPIOB
// Contactor 2
#define C2_PORT		GPIOA
#define C2_PERIPH 	RCC_AHB1Periph_GPIOA
// Contactor 3
#define C3_PORT		GPIOC
#define C3_PERIPH 	RCC_AHB1Periph_GPIOC

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
void BSP_Contactor_On(Contactor c);

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(Contactor c);

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(Contactor c);

#endif
