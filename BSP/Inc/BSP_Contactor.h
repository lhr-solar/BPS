/* Copyright (c) 2022 UT Longhorn Racing Solar */

#ifndef BSP_CONTACTOR_H
#define BSP_CONTACTOR_H

#include "common.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

// implement me later
typedef enum {
    ARRAY_CONTACTOR,
    LOAD_CONTACTOR,
    HVLOW_CONTACTOR
} Contactor_e;

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
 * @brief Initializes GPIO ports per Contactor requirements
 * 
 * @param gpioPort Specific port to initialize on
 * @param gpioStruct Specific struct to initialize
 * @param pinOutput pin number for output
 * @param pinInput pin number for input
 * @param mode pin mode
 * @param speed 
 * @param otype 
 * @param pupd 
 */
inline void Setup(GPIO_TypeDef* port, GPIO_InitTypeDef* gpioStruct, uint32_t pinOutput, uint32_t pinInput, GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed, 
               GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pupd);

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
void BSP_Contactor_On(Contactor_e c);

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(Contactor_e c);

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(Contactor_e c);

#endif
