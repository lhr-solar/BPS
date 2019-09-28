/* Turns Strobe light on and off */
#include <Current.h>
#include <Temperature.h>
#include <Voltage.h>
#include <Contactor.h>
#include "stm32f4xx.h"

void StrobeLight_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void StrobeLight_On(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);			
}

void StrobeLight_Off(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);		
}

uint32_t StrobeLight(void){
	return GPIO_Pin_12;
}