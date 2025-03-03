/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"
#include "BSP_PLL.h"

volatile uint32_t timer_delay_continue = 0;

void oneshot(void) {
    timer_delay_continue = 1;
}

int main(void){
    uint32_t test;   
    
    BSP_PLL_Init();
    BSP_Timer_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    BSP_Timer_Start_TickCounter();
    
    while(1){
        test = BSP_Timer_GetTicksElapsed();
        printf("Ticks elapsed : %ld\n\r", test);
        BSP_Timer_Start_OneShot(2e6, oneshot);
        while (!timer_delay_continue);
        timer_delay_continue = 0;
    }
   
}
