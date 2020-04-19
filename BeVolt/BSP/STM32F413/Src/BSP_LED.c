#include "BSP_LED.h"
#include "stm32f4xx.h"

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_LEDs_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	
	// PC0 : Over current
	// PC1 : Over temp
	// PC2 : Over volt
	// PC3 : Under volt
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// PA0 : Run
	// PA5 : Extra
	// PA6 : CAN
	// PA7 : WDog
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	// GPIO_InitStruct haven't changed so only pins have to be updated
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// PB12 : Fault
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	// GPIO_InitStruct haven't changed so only pins have to be updated
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief   Toggles a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_Toggle(Led signal) {
    switch(signal){
		// PB12
		case ERROR:
			GPIOB->ODR ^= GPIO_Pin_12;
			break;
			
		// PA0
		case RUN:
			GPIOA->ODR ^= GPIO_Pin_0;
			break;
			
		// PC0
		case OCURR:
			GPIOC->ODR ^= GPIO_Pin_0;
			break;
			
		// PC1
		case OTEMP:
			GPIOC->ODR ^= GPIO_Pin_1;
			break;
			
		// PC2
		case OVOLT:
			GPIOC->ODR ^= GPIO_Pin_2;
			break;
			
		// PC3;
		case UVOLT:
			GPIOC->ODR ^= GPIO_Pin_3;
			break;
			
		// PA7
		case WDOG:
			GPIOA->ODR ^= GPIO_Pin_7;
			break;
			
		// PA6
		case CAN:
			GPIOA->ODR ^= GPIO_Pin_6;
			break;
			
		// PA5
		case EXTRA:
			GPIOA->ODR ^= GPIO_Pin_5;
			break;
	}
}

/**
 * @brief   Turns on a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_On(Led signal) {
    switch(signal){
		// PB12
		case ERROR:
			GPIOB->ODR |= GPIO_Pin_12;
			break;
			
		// PA0
		case RUN:
			GPIOA->ODR |= GPIO_Pin_0;
			break;
			
		// PC0
		case OCURR:
			GPIOC->ODR |= GPIO_Pin_0;
			break;
			
		// PC1
		case OTEMP:
			GPIOC->ODR |= GPIO_Pin_1;
			break;
			
		// PC2
		case OVOLT:
			GPIOC->ODR |= GPIO_Pin_2;
			break;
			
		// PC3;
		case UVOLT:
			GPIOC->ODR |= GPIO_Pin_3;
			break;
			
		// PA7
		case WDOG:
			GPIOA->ODR |= GPIO_Pin_7;
			break;
			
		// PA6
		case CAN:
			GPIOA->ODR |= GPIO_Pin_6;
			break;
			
		// PA5
		case EXTRA:
			GPIOA->ODR |= GPIO_Pin_5;
			break;
	}
}

/**
 * @brief   Turns off a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_Off(Led signal) {
    switch(signal){
		// PB12
		case ERROR:
			GPIOB->ODR &= ~GPIO_Pin_12;
			break;
			
		// PA0
		case RUN:
			GPIOA->ODR &= ~GPIO_Pin_0;
			break;
			
		// PC0
		case OCURR:
			GPIOC->ODR &= ~GPIO_Pin_0;
			break;
			
		// PC1
		case OTEMP:
			GPIOC->ODR &= ~GPIO_Pin_1;
			break;
			
		// PC2
		case OVOLT:
			GPIOC->ODR &= ~GPIO_Pin_2;
			break;
			
		// PC3;
		case UVOLT:
			GPIOC->ODR &= ~GPIO_Pin_3;
			break;
			
		// PA7
		case WDOG:
			GPIOA->ODR &= ~GPIO_Pin_7;
			break;
			
		// PA6
		case CAN:
			GPIOA->ODR &= ~GPIO_Pin_6;
			break;
			
		// PA5
		case EXTRA:
			GPIOA->ODR &= ~GPIO_Pin_5;
			break;
	}
}
