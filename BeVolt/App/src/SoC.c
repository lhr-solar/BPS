/** main.c
 * Program for UTSVT BeVolt's Battery Protection System SOC
 * @authors Garrett Wong, Sijin Woo
 * @lastRevised 1/13/2019
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "ADC.h"

 uint32_t cumulativeNegativeSoc = 0;
 uint32_t initialSoc = 100;
 uint32_t finalSoc;
 
/** Soc_Init
 * Initializes necessary timer and values to begin state of charge
 * calculation algorithm.
 * Info found on reference sheet stm32f413 page 535
 */
void SoC_Init(void){ 
	// TODO: Initilize timer. 32 bit timer..
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 	//Enable TIM clock	
	TIM_TimeBaseInitTypeDef Init_TIM2;								 		//make struct
	
	Init_TIM2.TIM_Prescaler = 1;
	Init_TIM2.TIM_CounterMode = TIM_CounterMode_Down;
	Init_TIM2.TIM_Period = 0xFFFF;
	Init_TIM2.TIM_ClockDivision = TIM_CKD_DIV1;	
	
	TIM_TimeBaseInit(TIM2, &Init_TIM2);										//call function
	TIM_Cmd(TIM2,ENABLE);																	//enable counter	
	
}

/** SoC_Calculate
 * Calculates new SoC depending on the current reading
 * @param current reading from hall effect sensors. Fixed point of 0.0001 (1.500 Amps = 15000)
 * not a constant samping. Add on however much from the previous
 */
void SoC_Calculate(int32_t amps){uint32_t counter; uint32_t timeElapsed;
	// TODO: Coloumb counting algorithm.
	// Accumulator summation
	// need to find out how to raise a flag or access count register
	counter = TIM2->CNT;							//find current value of up counter
	TIM2->CNT = 0;										//set counter to zero to count up again
	timeElapsed = counter/(80000000); //time passed when you call the counter
	//cumulativeNegativeSoc = cumulativeNegativeSoc + timeElapsed*(ADC_Conversion(ADC_ReadLow()));
	//finalSoc = initialSoc - cumulativeNegativeSoc;
}

/** SoC_Calibrate
 * Calibrates the SoC. Whenever the BPS trips, the SoC should recalibrate. If an undervoltage
 * fault occurs, the SoC calibrates to 0% and whenever there is an overvoltage, the SoC calibrates
 * to 100%.
 * @param voltage fault type. 0 if under voltage, 1 if over voltage
 */
void SoC_Calibrate(int32_t faultType){
	// TODO: depending on fault type, the accumulator should be set t 0% or 100%
}

/** SoC_GetPercent
 * Gets the percentage of charge left in the battery pack
 * @return fixed point percentage. Resolution = 0.01 (45.55% = 4555)
 */
int32_t SoC_GetPercent(void){
	uint32_t socPercent;
	// TODO: returns percentage of charge
	socPercent = (finalSoc/ initialSoc)*100;
	return socPercent;
}

/** SoC_SetAccum 
 * Sets the accumulator of the coloumb counting algorithm
 * @param accumulator new value. Resolution = 0.0001.
 */
void SoC_SetAccum(int32_t accum){
	// TODO: sets accumulator.
}

