/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "stm32f4xx_tim.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"

void TIM4_IRQHandler();

int main(void) {
    TIM4_IRQHandler();
    BSP_Lights_Init();
    // create an interrupt with default handler
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_TimeBaseInitTypeDef t;
    t.TIM_Period = 0;
    TIM_TimeBaseInit(TIM2, &t);
    while(1){
        BSP_Light_Toggle(RUN);
    }
}

