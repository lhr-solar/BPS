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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	// Initialize PA6 as output
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
	
/**
 * 	Closes contactor, GPIO_Pin_6 = 1
 */
void Contactor_On(void){
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);					// Set pin 6 high
}

/**
 *	Opens contactor, GPIO_Pin_6 = 0
 */
void Contactor_Off(void){
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);				// Set pin 6 low
}

/**
 *	 Outputs: flag status (0 or 1)
 */
uint32_t Contactor_Flag(void){
		
}
	