/** main.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization/output
 * @authors Russell Sutton, Garrett Wong, Sijin Woo
 * @lastRevised 10/26/2019
 */

#include <stdint.h>
#include "stm32f4xx.h"

void ADC_InitHilo(void)	{
	
	//****		Structures for GPIO & ADC Initialization Parameters
	GPIO_InitTypeDef			GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	//****		GPIO & ADC Clock Enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//****		GPIOA configured as analog input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//****		ADC1 configuration
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_7Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);	
}

uint16_t ADC_ReadHigh(void){
	
	//****		Channel Configuration (Connecting the proper GPIO/ADC Channel to the ADC configuration)
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,1,ADC_SampleTime_15Cycles);
	
	ADC_Cmd(ADC1,ENABLE);                               //****		ADC ON
	ADC_SoftwareStartConv(ADC1);                        //****		RUN
	
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));	    //****		Wait for Reading
	ADC_Cmd(ADC1,DISABLE);                              //****		ADC OFF
	return ADC_GetConversionValue(ADC1);
}
uint16_t ADC_ReadLow(void){
	
	//****		Channel Configuration (Connecting the proper GPIO/ADC Channel to the ADC configuration)
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1,ADC_SampleTime_15Cycles);
	
	ADC_Cmd(ADC1,ENABLE);                               //****		ADC ON
	ADC_SoftwareStartConv(ADC1);                        //****		RUN
	
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));     //****		Wait for Reading
	ADC_Cmd(ADC1,DISABLE);                              //****		ADC OFF
	return ADC_GetConversionValue(ADC1);
}
