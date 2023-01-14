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
    BSP_PLL_Init();
    while(1){
        test = BSP_PLL_GetSystemClock();
        printf("\r%d", test);
    }
    
    
    exit(0);
}
