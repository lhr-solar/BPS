/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_Timer.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

static const int PRESCALER = 1999;
static const int MICROSEC_CON = 1000000;

/**
 * @brief   Initialize the timer for time measurements.
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void) {
    /* Timer is used to find the timeDelta */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef Init_TIM2;
	
	Init_TIM2.TIM_Prescaler = PRESCALER; // In every tick, there are 2000 clock cycles. (A 0 prescaler is actually 1, so 1999 = 2000)
	Init_TIM2.TIM_CounterMode = TIM_CounterMode_Up; // Count from 0 to Period value below
	Init_TIM2.TIM_Period = 0xFFFF;  // Max value of the counter before resetting to 0
	Init_TIM2.TIM_ClockDivision = TIM_CKD_DIV1;  // Clock divide by 1
	Init_TIM2.TIM_RepetitionCounter = 0;
	// With this configuration, it would take 3.27 seconds for the counter to overflow and reset to 0

	TIM_TimeBaseInit(TIM2, &Init_TIM2);
}

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start(void) {
    TIM_Cmd(TIM2,ENABLE);
}

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void) {
    /* Get system clocks */
	uint32_t counter = TIM2->CNT;       // find current value of up counter
	TIM2->CNT = 0;

    return counter;
}

/**
 * @brief   Gets the running frequency of the timer (time per tick)
 * @param   None
 * @return  frequency in Hz
 */
uint32_t BSP_Timer_GetRunFreq(void) {
    RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	// Return value is 40,000,000
    return RCC_Clocks.PCLK2_Frequency; // Tested using UART and this appears to be the proper clock used by the timer.
}

/**
 * @brief   Gives a standard unit for time elapsed in microseconds since calling BSP_Timer_GetTicksElapsed()
 * @param   None
 * @return  Microseconds 
 */
uint32_t BSP_Timer_GetMicrosElapsed(void) {
	
	uint32_t ticks = BSP_Timer_GetTicksElapsed();
	uint32_t freq = BSP_Timer_GetRunFreq();
	uint32_t micros_elap = ticks * (PRESCALER + 1) / (freq / MICROSEC_CON); // Math to ensure that we do not overflow (16Mhz or 80Mhz)
	return micros_elap;
} 
