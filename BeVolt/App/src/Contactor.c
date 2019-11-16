// Contactor.c
/**
 * Sets Contactor on or off
 * Checks if flag is high or low. If high, return 1, if low, return 0.
 * @author Manolo Alvarez
 * @lastRevised 11/21/2018
 */

#include "Contactor.h"
#include "stm32f4xx.h"

/** Contactor_Init
 * Initialize PA6 as output pin.
 */
void Contactor_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Initialize clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	// Initialize PB0 as output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;			// Pin 0 is output
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;			// Pin 1 is input
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
	
/**
 * 	Closes contactor, GPIO_Pin_6 = 1
 */
void Contactor_On(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);					// Set pin 6 high
}

/**
 *	Opens contactor, GPIO_Pin_6 = 0
 */
void Contactor_Off(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);				// Set pin 6 low
}

/**
 *	 Outputs: flag status
 */
FunctionalState Contactor_Flag(void){
	if((GPIOB->IDR & GPIO_Pin_1) >> 1) {
		return DISABLE;
	} else {
		return ENABLE;
	}
}

