/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "BSP_UART.h"
#include "BSP_PLL.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#endif

void foo(void){
    return;
}

int main() {
    BSP_UART_Init(foo, foo, UART_USB);
    char str[128];
    while(1) {
        int num = BSP_UART_ReadLine(str, UART_USB);
        if(num > 0) {
            printf("%s\n\r", str);
        }
    }
}
