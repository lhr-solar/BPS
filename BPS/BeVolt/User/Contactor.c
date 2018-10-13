/* 
Contactor
		Sets Contactor on or off
		Checks if flag is high or low. If high, return 1, if low, return 0.
*/

#include <stdint.h>
#include "stm32f4xx.h"
#include "Contactor.h"

/* 
	 Inputs: 0
	 Outputs: GPIOA
	 Modifies: PA6 and PA7
*/

void Contactor_Init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1_Periph_GPIOA, ENABLE);
	GPIO_InitTypDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);	

}

/* 
	 Inputs: 0
	 Outputs: PA6 = 1
	 Modifies: PA6
*/
uint32_t Contactor_On(void){
	GPIOA->ODR |= 0x40;			// Set pin 6 high
}

/* 
	 Inputs: 0
	 Outputs: PA6 = 0
	 Modifies: PA6
*/
uint32_t Contactor_Off(void){
	GPIOA->ODR &= 0xFFFFFFBF;			// Set pin 6 low
}

/* 
	 Inputs: PA7
	 Outputs: 0 or 1
	 Modifies: nothing
*/
uint32_t Contactor_Flag(void){
		if (ODR&0x080 == 0x080){
//			printf('Normal operation\n');
			return 1;
		} 
		else{
//			printf('Fault\n');
			return 0;
		}
}
	