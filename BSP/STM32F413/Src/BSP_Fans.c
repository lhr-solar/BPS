/* Copyright (c) 2020 UT Longhorn Racing Solar */

/* Fans.c
Controls speed of fans which cool down the battery pack.
Uses Pins PC6,7 and PB14,15
*/

#include "BSP_Fans.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

GPIO_InitTypeDef GPIO_INIT_STRUCT; //struct used to initialize pins
TIM_OCInitTypeDef TIMER_OC_STRUCT; //struct used to configure output compare for timers
TIM_TimeBaseInitTypeDef TIMER_INIT_STRUCT; //struct used to initialize PWM timers

void BSP_Fans_Init(void){
    //Enable TIM8 clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);

    //Enable Port Clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //Configure TIM pins by configuring corresponding GPIO pins
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM12);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);
    //All pins will be initialized for alternate function
    GPIO_StructInit(&GPIO_INIT_STRUCT);
    GPIO_INIT_STRUCT.GPIO_Mode = GPIO_Mode_AF;
    GPIO_INIT_STRUCT.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_INIT_STRUCT.GPIO_Speed = GPIO_Speed_50MHz;
    //Initialize Pin PC6, PC7
    GPIO_INIT_STRUCT.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_INIT_STRUCT);
    //Initialize Pin PB14, PB15
    GPIO_INIT_STRUCT.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_INIT_STRUCT); 

    //Configure Time base unit
    TIM_TimeBaseStructInit(&TIMER_INIT_STRUCT);
    TIMER_INIT_STRUCT.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_INIT_STRUCT.TIM_Period = PWM_PERIOD;
    TIM_TimeBaseInit(TIM8, &TIMER_INIT_STRUCT);
    TIM_TimeBaseInit(TIM12, &TIMER_INIT_STRUCT);

    //Fill in TIM_OCInitStruct with desired parameters
    TIM_OCStructInit(&TIMER_OC_STRUCT);
    TIMER_OC_STRUCT.TIM_OCMode = TIM_OCMode_PWM1;
    TIMER_OC_STRUCT.TIM_OutputState = TIM_OutputState_Enable;
    TIMER_OC_STRUCT.TIM_Pulse = PWM_PERIOD / 2;
    TIMER_OC_STRUCT.TIM_OCPolarity = TIM_OCPolarity_High;

    //Configure all channels
    TIM_OC1Init(TIM8, &TIMER_OC_STRUCT);
    TIM_OC2Init(TIM8, &TIMER_OC_STRUCT);
    TIM_OC1Init(TIM12, &TIMER_OC_STRUCT);
    TIM_OC2Init(TIM12, &TIMER_OC_STRUCT);

    //Enable the output compare preload on all channels
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM12, ENABLE);

    TIM_CtrlPWMOutputs(TIM8, ENABLE);

    //Enable the TIM8 counter
    TIM_Cmd(TIM8, ENABLE);
    TIM_Cmd(TIM12, ENABLE);
}


/*This function will change the speed of the fans
Inputs: Number of fan to change speed (1-4)
        Speed of Fan(0-8)
*/
ErrorStatus BSP_Fans_Set(uint8_t fan, uint32_t speed){
    //Range of pulse is 0-4000
    //First check to make sure that change is within range of values
    //Load new value into Compare and Capture Register
    if  (speed < 0 || speed > TOPSPEED) return ERROR; ; //if not in range, leave function with error code
    switch (fan)
    {
    case 1:
        TIM_SetCompare1(TIM8, (speed * DIVIDER));
        return SUCCESS;
        break;
    case 2:
        TIM_SetCompare2(TIM8, (speed * DIVIDER));
        return SUCCESS;
        break;
    case 3:
        TIM_SetCompare1(TIM12, (speed * DIVIDER));
        return SUCCESS;
        break;
    case 4:
        TIM_SetCompare2(TIM12, (speed * DIVIDER));
        return SUCCESS;
        break;
    default:
        return ERROR; //invalid fan value
        break;
    }
}

int BSP_Fans_GetSpeed(uint8_t fan){
    switch (fan)
    {
    case 1:
        return TIM8-> CCR1 / DIVIDER;
        break;
    case 2:
        return TIM8-> CCR2 / DIVIDER;
        break;
    case 3:
        return TIM12-> CCR1 / DIVIDER;
        break;
    case 4:
        return TIM12-> CCR2 / DIVIDER;
        break;
    }

    return -1;
}