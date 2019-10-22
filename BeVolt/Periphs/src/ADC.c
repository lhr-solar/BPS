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
	// Apparently channel 2 has priority, or is at least read first.
	// If you change the priorities, be prepared to have the order in the array change.
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_480Cycles);

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
uint16_t ADC_ReadHigh(void){
	return ADCresults[0];
}
