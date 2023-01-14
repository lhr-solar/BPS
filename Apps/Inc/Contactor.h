<<<<<<< HEAD:Apps/Inc/Contactor.h
 
=======
>>>>>>> master:BSP/Inc/BSP_Contactor.h
/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef CONTACTOR_H
#define CONTACTOR_H

#include "BSP_PWM.h" //PWM_Period is defined in here
#include "Simulator.h"

//Parameters for PWM
#define PWM_ON_TIME 2000

// Contactor 1
#define C1 4 // contactor 1 on pwm output 4
// Contactor 2 - this is for the contactor connected to the fan board
#define CFAN 3 // right now we are using fan pin 3 as the contactor output

//These are defining how the contactors are wired, the array contactor is assumed to be wired to the fan board
#define ARRAY_CONTACTOR_OUT CFAN
#define HVHIGH_CONTACTOR_OUT C1
#define HVLOW_CONTACTOR_OUT C1

typedef uint8_t CONT_CHOICE;
typedef enum {ARRAY_CONTACTOR = 1, HVHIGH_CONTACTOR = 2, HVLOW_CONTACTOR = 4, ALL_CONTACTORS = 7} Contactors_t;

<<<<<<< HEAD:Apps/Inc/Contactor.h
=======
#include "common.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

typedef uint8_t CONT_CHOICE;
typedef enum {ARRAY_CONTACTOR = 1, HVHIGH_CONTACTOR = 2, HVLOW_CONTACTOR = 4, ALL_CONTACTORS = 7} Contactors_t;

// Contactor 1
#define C1_PORT		GPIOB
// Contactor 2
#define C2_PORT		GPIOA
// Contactor 3
#define C3_PORT		GPIOC
>>>>>>> master:BSP/Inc/BSP_Contactor.h

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
<<<<<<< HEAD:Apps/Inc/Contactor.h
void Contactor_On(Contactors_t contactorChoice);
=======
void BSP_Contactor_On(Contactors_t contactorChoice);
>>>>>>> master:BSP/Inc/BSP_Contactor.h

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
<<<<<<< HEAD:Apps/Inc/Contactor.h
void Contactor_Off(Contactors_t contactorChoice);
=======
void BSP_Contactor_Off(Contactors_t contactorChoice);
>>>>>>> master:BSP/Inc/BSP_Contactor.h

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
<<<<<<< HEAD:Apps/Inc/Contactor.h
bool Contactor_GetState(Contactors_t contactorChoice);
=======
bool BSP_Contactor_GetState(Contactors_t contactorChoice);
>>>>>>> master:BSP/Inc/BSP_Contactor.h

#endif
