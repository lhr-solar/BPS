/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"

int main(void){
    uint32_t test;
    uint32_t delay = 10000000;
    uint32_t time = 1;        
    
    BSP_Timer_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    BSP_Timer_Start();
    uint32_t freq = BSP_Timer_GetRunFreq();
    printf("New version \n\r");
    
    while(1){
        test = BSP_Timer_GetTicksElapsed();
        printf("Ticks elapsed : %ld\n\r", test);
        freq = BSP_Timer_GetRunFreq();
        printf("Timer frequency: %ld\n\r", freq);  
        while(time){                        //this is a delay to prevent the elapsed time being negligible
            delay = 10000000;
            while(delay){
                delay--;
            }
            time--;
            printf("Decrement \n\r");
        }
        time = 1;
    }
   
}
