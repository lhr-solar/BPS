/* Fans.c
Controls speed of fans which cool down the battery pack.
Uses Pins PC6,7 and PB14,15
*/

#include <stdint.h>
#include "Fans.h"

GPIO_InitTypeDef GPIO_INIT_STRUCT; //struct used to initialize pins
TIM_OCInitTypeDef TIMER_STRUCT; //struct used to configure timers
TIM_TimeBaseInitTypeDef TIMER_INIT_STRUCT; //struct used to initialize PWM timers

void FansInit(void){
    //Enable Port Clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
    //Configure Pins to Timers
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM8);
    //All pins will be initialized postivie logic, pull up/pull down, 2Mhz, and alternate function
    GPIO_INIT_STRUCT.GPIO_Mode = GPIO_Mode_AF;
    GPIO_INIT_STRUCT.GPIO_OType = GPIO_OType_PP; 
    GPIO_INIT_STRUCT.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_INIT_STRUCT.GPIO_Speed = GPIO_Speed_2MHz;
    //Initialize Pin PC6, PC7
    GPIO_INIT_STRUCT.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_INIT_STRUCT);
    //Initialize Pin PB14, PB15
    GPIO_INIT_STRUCT.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_INIT_STRUCT); 
    //enable Timer 8 peripheral
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    //TIM8CLK = 2 * PLCK2(40MHz) = 80MHz
    //Initialize at 20 KHz, means 20KHz = TIM8CLK(80MHz)/((Prescaler(0)+1) * Period)
    //Duty Cycle = Pulse/Period * 100, Period = 4000
    TIMER_INIT_STRUCT.TIM_Prescaler = 0x0000; //clock frequency not prescaled for timer
    TIMER_INIT_STRUCT.TIM_ClockDivision = TIM_CKD_DIV1; //don't divide timer
    TIMER_INIT_STRUCT.TIM_CounterMode = TIM_CounterMode_Up; //count up
    TIMER_INIT_STRUCT.TIM_Period = 4000;
    TIM_TimeBaseInit(TIM8, &TIMER_INIT_STRUCT); //Initialize Timer 8
    //Initialize all channels to 50% duty cycle
    //Polarity is high: duty cycle is amount of time output is high
    TIMER_STRUCT.TIM_OCMode = TIM_OCMode_PWM1; //enable PWM with Timer 8
    TIMER_STRUCT.TIM_OutputState = TIM_OutputState_Enable;
    TIMER_STRUCT.TIM_Pulse = 2000;
    TIMER_STRUCT.TIM_OCPolarity = TIM_OCPolarity_High;
    //Initalize all channels
    TIM_OC1Init(TIM8, &TIMER_STRUCT);
    TIM_OC2Init(TIM8, &TIMER_STRUCT);
    TIM_OC3Init(TIM8, &TIMER_STRUCT);
    TIM_OC4Init(TIM8, &TIMER_STRUCT);
    //Enable Preload register for timer (Pulse)
    TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
    //Enable Automatic Reload Register (Period)
    TIM_ARRPreloadConfig(TIM8, ENABLE);
    }

void FanSpeedChange(uint8_t fan, int8_t accel){
    //Range of pulse is 0-4000, change in increments of 500
    switch (fan)
    {
    case 1:
        if ((TIM8->CCR1 + (accel * 500)) > 0 || (TIM8->CCR1 + (accel * 500)) < 4000)
        TIM8->CCR1 += (accel * 500);
        break;
    case2:
        if ((TIM8->CCR2 + (accel * 500)) > 0 || (TIM8->CCR2 + (accel * 500)) < 4000)
        TIM8->CCR2 += (accel * 500);
        break;
    case3:
        if ((TIM8->CCR3 + (accel * 500)) > 0 || (TIM8->CCR3 + (accel * 500)) < 4000)
        TIM8->CCR3 += (accel * 500);
        break;
    case3:
        if ((TIM8->CCR4 + (accel * 500)) > 0 || (TIM8->CCR4 + (accel * 500)) < 4000)
        TIM8->CCR4 += (accel * 500);
        break;
    default:
        break;
    }
}