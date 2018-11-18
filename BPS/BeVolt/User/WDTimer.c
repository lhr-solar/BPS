// WDTimer.c
/**
 * Driver for the watch dog timer. Whenever the Watch Dog Timer reaches 0,
 * all contactors will be opened (the battery pack will be disconnected from the car).
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/4/2018
 */

#include "WDTimer.h"
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "Contactor.h"


void WDTimer_Init(void)
{
 // Enable clock for TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // TIM3 initialization overflow every 5s
    // TIM3 by default has clock of 50MHz
    // Here, we must set value of prescaler and period,
    // so update event is 0.2Hz or 5s
    // Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) *
    // (TIM_Period + 1))
    // Update Event (Hz) = 50MHz / ((24999 + 1) * (9999 + 1)) = .2 Hz

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 24999;
    TIM_TimeBaseInitStruct.TIM_Period = 9999;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;

    // TIM3 initialize
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // Start TIM3
    TIM_Cmd(TIM3, ENABLE);

    // Enable TIM3 interrupt
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}


/** EnableTimerInterrupt
 * Enables Interrupts
 */
void EnableTimerInterrupt(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // Nested vectored interrupt settings
    // TIM3 interrupt is most important (PreemptionPriority and
    // SubPriority = 0)
}


/** WDTimer_Start
 * Starts the Watch Dog Timer count down
 */
void WDTimer_Start(void){
	TIM_Cmd(TIM3, ENABLE);
}


/** WDTimer_Reset
 * Resets the Watch Dog Timer. Does not stop the counter.
 */
void WDTimer_Reset(void){
	   // Set the counter back to 0
    TIM_SetCounter (TIM3, 0);
	
}

/** WDTimer_Stop
 * Stops the Watch Dog Timer. Does not reset the counter.
 */
void WDTimer_Stop(void){
	TIM_Cmd(TIM3, DISABLE);
}


/** WDTimer_FaultDetection
 * Problem -> Disconnects the contactor
 */
void TIM3_IRQHandler(void)
{
//  Contactor_Off();
}

