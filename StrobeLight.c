/* Turns Strobe light on and off */
#include <stdio.h>
#include <Current.h>
#include <Temperature.h>
#include <Voltage.h>
#include <Contactor.h>
#include "stm32f4xx.h"

void StrobeLight_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
	//Initializes Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
//Checks if Error Light pin 12 is sending signal = 1
void StrobeLight_On(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);			
}
//Checks if Error Light pin 12 is sending signal = 0
void StrobeLight_Off(void){
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);		
}
//returns value of pin, either 0 or 1
uint32_t StrobeLight(void){
	int num = GPIO_Pin_12;
	printf("%d", num);
	return GPIO_Pin_12;
}