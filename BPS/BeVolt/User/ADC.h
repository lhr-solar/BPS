#ifndef ADC_H
#define ADC_H
#include <stdint.h>

void ADC_InitHilo(void);

uint32_t ADC_ReadHigh(void);

uint32_t ADC_ReadLow(void);

uint32_t ADC_Conversion(uint32_t ADC_Reading);

#endif
