/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_PLL.h"


int main(void){
    uint32_t test;
    uint32_t delay = 150000000;
    uint32_t time = 200;       
    while(time){
        delay = 15000000;
        while(delay){
            delay--;
        }
        time--;
    }                           //delay is to show the clock going from 16MHz to 80MHz once BSP_PLL_Init() is called

    if(BSP_PLL_Init() == ERROR){
        // if initializing PLL fails, something is very wrong
        while(1){
            printf("PLL init has failed");
            volatile uint16_t horse = -1;
            while(horse!=0){
                --horse; // stall some time before printing out
            }
        }
    }

    while(1){
        test = BSP_PLL_GetSystemClock();
        printf("\r%ld", test);
    }
    return 0;
}
