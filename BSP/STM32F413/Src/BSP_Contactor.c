/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_Contactor.h"
#include "stm32f4xx.h"

/**
 * @brief   Initializes the GPIO pins that interfaces with the Contactor.
 *          Two GPIO pins are initialized. One as an output and one as an input.
 *          The output pin controls the state and the input pin views what state the contactor is through the
 *          Aux pins.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	
	// Initialize clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// Initialize PB0 as output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;			// Pin 0 is output
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;			// Pin 1 is input
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(void) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);      // Set pin 0 high
}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(void) {
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);	// Set pin 0 low
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(void) {
    // The AUX pin we have connected to is negative logic
    if((GPIOB->IDR & GPIO_Pin_1) >> 1) {
		return 0;
	} else {
		return 1;
	}
}
