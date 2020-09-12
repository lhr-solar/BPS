#include "BSP_Lights.h"
#include "stm32f4xx.h"

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	
	// PC0 : Over current
	// PC1 : Over temp
	// PC2 : Over volt
	// PC3 : Under volt
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2| GPIO_Pin_3 | GPIO_Pin_4; 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7; 
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStruct.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal) {
    switch(signal){

		// PC0
		case EXTRA:
			GPIOC->ODR ^= GPIO_Pin_0;
			break;
		// PC1
		case CAN:
			GPIOC->ODR ^= GPIO_Pin_1;
			break;
		// PC2
		case WDERROR:
			GPIOC->ODR ^= GPIO_Pin_2;
			break;
		// PC3
		case WIRE:
			GPIOC->ODR ^= GPIO_Pin_3;
			break;
		// PC4
		case HEARTBEAT:
			GPIOC->ODR ^= GPIO_Pin_4;
			break;
		// PA4
		case OCURR:
			GPIOA->ODR ^= GPIO_Pin_4;
			break;
		// PA5
		case OTEMP:
			GPIOA->ODR ^= GPIO_Pin_5;
			break;
		// PA6
		case OVOLT:
			GPIOA->ODR ^= GPIO_Pin_6;
			break;
		// PA7
		case UVOLT:
			GPIOA->ODR ^= GPIO_Pin_7;
			break;
		
        default:
            break;
	}
}

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal) {
    switch(signal){
		// PB12
		case FAULT:
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

        default:
            break;
	}
}

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal) {
    switch(signal){
		// PB12
		case FAULT:
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

        default:
            break;
	}
}

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of the LED/Light (ON/OFF)
 */
State BSP_Light_GetState(Light signal) {
    State state = OFF;
    switch(signal){
		// PB12
		case FAULT:
			state = (GPIOB->ODR & GPIO_Pin_12) ? ON : OFF;
			
		// PA0
		case RUN:
			state = (GPIOA->ODR & GPIO_Pin_0) ? ON : OFF;
			break;
			
		// PC0
		case OCURR:
			state = (GPIOC->ODR & GPIO_Pin_0) ? ON : OFF;
			break;
			
		// PC1
		case OTEMP:
			state = (GPIOC->ODR & GPIO_Pin_1) ? ON : OFF;
			break;
			
		// PC2
		case OVOLT:
			state = (GPIOC->ODR & GPIO_Pin_2) ? ON : OFF;
			break;
			
		// PC3;
		case UVOLT:
			state = (GPIOC->ODR & GPIO_Pin_3) ? ON : OFF;
			break;
			
		// PA7
		case WDOG:
			state = (GPIOA->ODR & GPIO_Pin_7) ? ON : OFF;
			break;
			
		// PA6
		case CAN:
			state = (GPIOA->ODR & GPIO_Pin_6) ? ON : OFF;
			break;
			
		// PA5
		case EXTRA:
			state = (GPIOA->ODR & GPIO_Pin_5) ? ON : OFF;
			break;
	}

    return state;
}
