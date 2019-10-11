/** main.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization/output
 * @authors Garrett Wong, Sijin Woo, Russell Sutton
 * @lastRevised 09/28/2019
 */

#include <stdint.h>
#include "stm32f4xx.h"

/** ADC_InitHilo
 * Initializes PA2 (high prec) and PA3 (low prec) as ADC input pins.
 */
void ADC_InitHilo(void){
	//uint16_t digitalValue;
	//channelSelect = 0 = High Precision
	//channelSelect = 1 = Low Precision
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;									// Analog In, used to be =AIN

	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;							//was =DOWN
	GPIO_Init(GPIOA,&GPIO_InitStruct);


	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 1;

	ADC_Init(ADC2, &ADC_InitStruct);
	// i think this is the right place for this ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE); Sets endOfConversion flag usage, may be unnecessary
	//enable and begin

}
/** ADC_ReadLow
 * PA3 (low prec) ADC return value
 */
uint16_t ADC_ReadLow(void){
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_56Cycles); // Low precision sensor, rank=1, idk about sample time just set lowest val
	ADC_Cmd(ADC1,ENABLE);
	ADC_SoftwareStartConv(ADC1);
	//wait for conversion
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	ADC_Cmd(ADC1,DISABLE);
	return ADC_GetConversionValue(ADC1);

	/*
	digitalValue = ADC_GetConversionValue(ADC1);
	ADC_Cmd(ADC1,DISABLE);
	return digitalValue;  */
}

/** ADC_ReadHigh
 * PA2 (high prec) ADC return value
 */
uint16_t ADC_ReadHigh(void){
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_56Cycles); // High precision sensor, rank=2, idk about sample time just set lowest val
	ADC_Cmd(ADC1,ENABLE);
	ADC_SoftwareStartConv(ADC1);
	//wait for conversion
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	ADC_Cmd(ADC1,DISABLE);
	return ADC_GetConversionValue(ADC1);

	/*
	digitalValue = ADC_GetConversionValue(ADC1);
	ADC_Cmd(ADC1,DISABLE);
	return digitalValue;  */
}

/**~~~~~ LEGACY REFRENCE ~~~~~~~~~~

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

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





 * ADC_ReadHigh
 * Returns the adc value of PA2 (high prec)
 * @return adc value in high prec adc
uint32_t ADC_ReadHigh(void){
	// Start ADC conversion
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1,ADC_SampleTime_84Cycles);

	// Wait until conversion is finish
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC1->DR & 0x00000FFF;
}

 * ADC_ReadLow
 * Returns the adc value of PA3 (low prec)
 * @return adc value of low prec adc

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

 * ADC_Conversion
 * Returns the converted current value given the ADC readings
 * @returns converted current

uint32_t ADC_Conversion (uint32_t ADC_Reading){
 // note Fred's board already converts the -4 to 4 volts to 0-4
 //uint32_t convertedVoltage = ((ADC_Reading*4.096)/2047)*1000; // converts ADC reading to a voltage
 uint32_t convertedVoltage = (((ADC_Reading*3300)/4096));
 int32_t convertedCurrent = (convertedVoltage-2048)*(100/4); // takes voltage and converts to a current reading, given linear slope
 return convertedCurrent;
 //return convertedVoltage;
}

 * ADC_ChooseHiLo
 * Chooses which value of ADC readings to choose from
 * High or Low current Tamura Sensor
 * @returns converted value

uint32_t ADC_ChooseHiLo (uint32_t highReading, uint32_t lowReading){
	return ADC_Conversion(lowReading);
	if (ADC_Conversion(highReading) < 50){
		return ADC_Conversion(lowReading);
	}
	return ADC_Conversion(highReading);
}*/
