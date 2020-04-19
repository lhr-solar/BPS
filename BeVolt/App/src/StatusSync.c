#include "StatusSync.h"
#include <stdbool.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include <CAN.h>

//Timer for the TIM2 timer on STM32 chip

volatile int DashboardMessageFlag;
volatile int TIM2Count = 0;

void TIM2_IRQHandler(){ 
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){ // check if Update bit is set or not
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(TIM2Count > 1000){
			TIM2Count++;
			DashboardMessageFlag = 0;
		}
		else{
			TIM2Count = 0;
			DashboardMessageFlag = 1;
		}
	}


}



void EnableTIM2Interrupt(){

	NVIC_InitTypeDef nvicStruct;

	nvicStruct.NVIC_IRQChannel = TIM2_IRQn;
	nvicStruct.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStruct.NVIC_IRQChannelSubPriority = 1;
	nvicStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvicStruct);


}


void DashMessage(bool overrideFlag){
	if(TIM2Count == 1000){
		if(DashboardMessageFlag == 1){
			if(overrideFlag == false){    //the motor can be running
				CANData_t data = {1};
				CANPayload_t payload = {.idx = 0, .data = data};
				CAN1_Send(CAN_MOTOROFF, payload);

			}
		else{ // danger; the motor should NOT be running
					CANData_t data = {0};
					CANPayload_t payload = {.idx = 0, .data = data};
					CAN1_Send(CAN_MOTOROFF, payload);
				}
		}
	}


}






void TIM2Init(){  

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStruct;

	timerInitStruct.TIM_Prescaler = 800;
	timerInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStruct.TIM_Period = 50000;
	timerInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &timerInitStruct);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	// timer will generate "update" events when period value is reached

	EnableTIM2Interrupt();
}
