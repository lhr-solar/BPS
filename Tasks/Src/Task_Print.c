/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "Print_Queue.h"
#include "BSP_UART.h"

// buffer for blocked copies with BSP_UART_Write
// arbitrarily set to 1/2 of the BSP_UART_TX_SIZE
// note that UART_Write is blocking so too large of a buffer size 
// will hang for awhile
#define PRINT_BUFFER_SIZE (BSP_UART_TX_SIZE / 2)

static char print_buffer[PRINT_BUFFER_SIZE];

void Task_Print(void *p_arg) {
    (void)p_arg;

    uint32_t pending;

    while (1) {
        PQ_WaitForFlush();

        pending = PQ_GetNumWaiting();   // could be outdated, but will always be less than actual

        // copy over block-by-block
        for (uint32_t i = 0; i < pending / PRINT_BUFFER_SIZE; i++) {
            PQ_Read(print_buffer, PRINT_BUFFER_SIZE);
            BSP_UART_Write(print_buffer, PRINT_BUFFER_SIZE, UART_USB);
        }

        // copy remainder
        PQ_Read(print_buffer, pending % PRINT_BUFFER_SIZE);
        BSP_UART_Write(print_buffer, pending % PRINT_BUFFER_SIZE, UART_USB);
    }
}
