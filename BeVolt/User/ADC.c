/** main.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization
 * @authors Garrett Wong, Sijin Woo
 * @lastRevised 1/11/2019
 */

#include <stdint.h>
#include "stm32f4xx.h"

/** ADC_InitHilo
 * Initializes PA2 (high prec) and PA3 (low prec) as ADC input pins.
 */
void ADC_InitHilo(void){	
	GPIO_InitTypeDef GPIO_InitStruct;													// make structure
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 		// enable clock portA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;				// setting pin2? and pin 3? high
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;									// Settting mode to ???? where did i find the stuff like AN
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;						// setting clock?
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;								// ^^ ^^ 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA,&GPIO_InitStruct);												// calling GPIO_Init function with PortA and InitStruct
	
	ADC_InitTypeDef ADC_InitStruct;														// Init ADC
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_NbrOfConversion = 2;										// one sample.. fast but not precise
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;				// 12 bit precision
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;								// single or all channels
	ADC_Init(ADC1, &ADC_InitStruct);													// initialize
	ADC_Cmd(ADC1, ENABLE);																		// command
}

/** ADC_ReadHigh
 * Returns the adc value of PA2 (high prec)
 * @return adc value in high prec adc
 */
uint32_t ADC_ReadHigh(void){
	// Start ADC conversion
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1,ADC_SampleTime_84Cycles);
	ADC_SoftwareStartConv(ADC1);
	// Wait until conversion is finish
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC1->DR & 0x00000FFF;
}
	
/** ADC_ReadLow
 * Returns the adc value of PA3 (low prec)
 * @return adc value of low prec adc
 */
uint32_t ADC_ReadLow(void){
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1,ADC_SampleTime_84Cycles);
	ADC_SoftwareStartConv(ADC1);
	// Wait until conversion is finish
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC1->DR & 0x00000FFF;
}
	// INITIALIZE ADC PIN "ALTERNATE FUNCTION"
	// SAME THING FOR OTHER ONES
	// RETURN 

/** ADC_Conversion
 * Returns the converted current value given the ADC readings
 * @returns converted current
 */
uint32_t ADC_Conversion (uint32_t ADC_Reading){
 // note Fred's board already converts the -4 to 4 volts to 0-4 
 //uint32_t convertedVoltage = ((ADC_Reading*4.096)/2047)*1000; // converts ADC reading to a voltage
 uint32_t convertedVoltage = (((ADC_Reading*3300)/4096));
 int32_t convertedCurrent = (convertedVoltage-2048)*(100/4); // takes voltage and converts to a current reading, given linear slope
 return convertedCurrent;
 //return convertedVoltage;	
}

/** ADC_ChooseHiLo
 * Chooses which value of ADC readings to choose from
 * High or Low current Tamura Sensor
 * @returns converted value
 */
uint32_t ADC_ChooseHiLo (uint32_t highReading, uint32_t lowReading){
	return ADC_Conversion(lowReading);
	/*if (ADC_Conversion(highReading) < 50){
		return ADC_Conversion(lowReading);
	}
	return ADC_Conversion(highReading);*/
}

