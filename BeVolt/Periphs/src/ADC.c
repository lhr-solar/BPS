/** main.c
 * Program for UTSVT BeVolt's Battery Protection System ADC initialization/output
 * @authors Garrett Wong, Sijin Woo, Russell Sutton
 * @lastRevised 09/28/2019
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"

static volatile uint16_t ADCresults[2];

void ADC_InitDMA(void) {
	// Start the clock for the DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	// Create the DMA structure
	DMA_InitTypeDef DMA_InitStruct;

	DMA_InitStruct.DMA_Channel = DMA_Channel_0;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t) &ADCresults;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStruct.DMA_BufferSize = 2;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);

	// Enable DMA2 stream 0
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

/** ADC_InitHilo
 * Initializes PA2 (high prec) and PA3 (low prec) as ADC input pins.
 */
void ADC_InitHilo(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	// Enable the ADC clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	// Enable the PA clock

	ADC_InitDMA();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;	// Using pins PA2 and PA3 for the ADC
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;						// Analog Input
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;				// High impedence
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// ADC Common Init
	ADC_CommonInitTypeDef ADC_CommonStruct;
	ADC_CommonStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonStruct.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonStruct);

	// Set up to use DMA so that multiple channels can be used
	ADC_InitTypeDef ADC_InitStruct;	// Initialization structure
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;	// High resolution
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;						// So we can go through all the channels
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; 		// Cycle the channels
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 2;							// We have two channels that we need to read

	ADC_Init(ADC1, &ADC_InitStruct);

	// Configure the channels
	// TODO: investigate the sample time that is needed for this
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_56Cycles);

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	ADC_DMACmd(ADC1, ENABLE);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);

	ADC_SoftwareStartConv(ADC1);
}

/** ADC_ReadLow
 * PA3 (low prec) ADC return value
 */
uint16_t ADC_ReadLow(void){
	return ADCresults[1];
}

/** ADC_ReadHigh
 * PA2 (high prec) ADC return value
 */
<<<<<<< HEAD
uint16_t ADC_ReadHigh(void){
	return ADCresults[0];
=======
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
>>>>>>> 2b9455ca47e4fbea0cb1dd095e25de26d858d1f3
}
