/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_PWM.h"

GPIO_InitTypeDef GPIO_INIT_STRUCT; //struct used to initialize pins
TIM_OCInitTypeDef TIMER_OC_STRUCT; //struct used to configure output compare for timers
TIM_TimeBaseInitTypeDef TIMER_INIT_STRUCT; //struct used to initialize PWM timers

void BSP_PWM_Init(void){
    //we will initialize all pins pertaining to fans and contactors for PWM here

    //Enable TIM 3,8,12
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

    //Enable Port Clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //HV Contactor
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    //Configure TIM pins by configuring corresponding GPIO pins
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3); //this one will always be contactor
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);

    //All pins will be initialized for alternate function
    GPIO_INIT_STRUCT.GPIO_Mode = GPIO_Mode_AF;
    GPIO_INIT_STRUCT.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_INIT_STRUCT.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_INIT_STRUCT.GPIO_OType = GPIO_OType_PP;
    //Initialize Pin PC6, PC7
    GPIO_INIT_STRUCT.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_INIT_STRUCT);
    //Initialize Pin PB14, PB15
    GPIO_INIT_STRUCT.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_INIT_STRUCT); 

    //Configure Time base unit
	TIMER_INIT_STRUCT.TIM_Period = PWM_PERIOD;
  	TIMER_INIT_STRUCT.TIM_Prescaler = 0x0000;
	TIMER_INIT_STRUCT.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER_INIT_STRUCT.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER_INIT_STRUCT.TIM_RepetitionCounter = 0x0000;
    TIM_TimeBaseInit(TIM3, &TIMER_INIT_STRUCT);
    TIM_TimeBaseInit(TIM8, &TIMER_INIT_STRUCT);
    TIM_TimeBaseInit(TIM12, &TIMER_INIT_STRUCT);

    //Fill in TIM_OCInitStruct with desired parameters
	TIMER_OC_STRUCT.TIM_OCMode = TIM_OCMode_PWM1;
	TIMER_OC_STRUCT.TIM_OutputState = TIM_OutputState_Enable;
	TIMER_OC_STRUCT.TIM_OutputNState = TIM_OutputNState_Disable;
	TIMER_OC_STRUCT.TIM_Pulse = 0; //start with everything off
	TIMER_OC_STRUCT.TIM_OCPolarity = TIM_OCPolarity_High;
	TIMER_OC_STRUCT.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIMER_OC_STRUCT.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIMER_OC_STRUCT.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	//Configure all channels with the output compare
    TIM_OC3Init(TIM3, &TIMER_OC_STRUCT);
    TIM_OC1Init(TIM8, &TIMER_OC_STRUCT);
    TIM_OC2Init(TIM8, &TIMER_OC_STRUCT);
    TIM_OC1Init(TIM12, &TIMER_OC_STRUCT);
    TIM_OC2Init(TIM12, &TIMER_OC_STRUCT);

    //Enable the output compare preload on all channels
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM12, ENABLE);

    TIM_CtrlPWMOutputs(TIM8, ENABLE);

    //Enable the TIM8 counter
    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM8, ENABLE);
    TIM_Cmd(TIM12, ENABLE);
}

ErrorStatus BSP_PWM_Set(uint8_t pin, uint32_t speed){
    //Range of pulse is 0-4000
    //First check to make sure that change is within range of values
    //Load new value into Compare and Capture Register
    switch (pin)
    {
    case 1:
        TIM_SetCompare1(TIM8, speed);
        return SUCCESS;
        break;
    case 2:
        TIM_SetCompare2(TIM8, speed);
        return SUCCESS;
        break;
    case 3:
        TIM_SetCompare1(TIM12, speed);
        return SUCCESS;
        break;
    case 4:
        TIM_SetCompare2(TIM12, speed);
        return SUCCESS;
        break;
    case 5:
        TIM_SetCompare3(TIM3, speed);
        return SUCCESS;
        break;
    default:
        return ERROR; //invalid fan value
        break;
    }
}

int BSP_PWM_Get(uint8_t pin){
    switch (pin)
    {
    case 1:
        return TIM_GetCapture1(TIM8);
        break;
    case 2:
        return TIM_GetCapture2(TIM8);
        break;
    case 3:
        return TIM_GetCapture1(TIM12);
        break;
    case 4:
        return TIM_GetCapture2(TIM12);
        break;
    case 5:
        return TIM_GetCapture3(TIM3);
        break;
    default:
        return -1;
    }
}