/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include <stdio.h>
#include "common.h"
#include "config.h"
#include "Fans.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"

#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"

int main()
{
    BSP_PLL_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    BSP_Lights_Init();
    Fans_Init();
    
    Fans_Set(1,2);
    Fans_Set(2,4);
    Fans_Set(3,6);
    Fans_Set(4,8);

    while(1){
        BSP_Light_Toggle(FAULT);
        for (volatile int i  = 0; i < 1000000; i++);
    }
}
