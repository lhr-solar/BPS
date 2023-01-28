/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "BSP_UART.h"
#include "BSP_PLL.h"

void foo(void){
    return;
}

int main() {
    BSP_PLL_Init();
    BSP_UART_Init(foo, foo, UART_USB);

    while(1) {
        printf("12345678");
    }
}
