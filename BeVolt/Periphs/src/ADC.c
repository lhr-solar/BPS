/** ADC.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "ADC.h"

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
	
}
	
/** ADC_ReadLow
 * Returns the adc value of PA3 (low prec)
 * @return adc value of low prec adc
 */
uint32_t ADC_ReadLow(void){
	
}

/** ADC_Conversion
 * Returns the converted current value given the ADC readings.
 * @returns converted current - fixed point notation of 0.001
 */
uint32_t ADC_Conversion (uint32_t ADC_Reading, CurrentSensor s){
	const int maxReading = 4095;	// The maximum reading the ADC is capable of returning
																// based on the precision of the ADC.
	const int operationMilliVoltage = 3300;	// The operating voltage in millivolts
	const int opAmpGain = 3;	// The gain for the op-amp stage
	const int opAmpOffset = 4096;	// The offset in millivolts for the op-amp stage
	
	// The actual reading on the ADC pin
	uint32_t milliVolts = ADC_Reading * operationMilliVoltage / maxReading;
	
	// The output of the hall sensor (prior to the op-amp stage)
	uint32_t sensorOutput = milliVolts * opAmpGain - opAmpOffset;
	
	// The actual current measured by the sensors
	switch(s) {
		case HIGH_PRECISION:	// Rated for currents between -50 and 50 A.
			return sensorOutput * 50 / 4;	// In mA.
		case LOW_PRECISION:		// Rated for currents between -100 and 100 A.
			return sensorOutput * 100 / 4;	// In mA.
	}
}


