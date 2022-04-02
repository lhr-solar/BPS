/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "M24128.h"
#include "BSP_UART.h"

int main(void) {

    static char input[] = "Hello World!\n";
    static char output[sizeof(input)];

    BSP_UART_Init(NULL, NULL, UART_USB);

    M24128_Init();

    M24128_Write(0, sizeof(input), input);
    M24128_Read(0, sizeof(input), output);

    BSP_UART_Write(output, sizeof(input) - 1, UART_USB);

    while (1);

    return 0;
}

