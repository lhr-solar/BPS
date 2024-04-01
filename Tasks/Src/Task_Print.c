/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "Print_Queue.h"
#include "BSP_UART.h"


void Task_Print(void *p_arg) {
    (void)p_arg;

    uint32_t pending;
    char c;

    while (1) {
        PQ_WaitForFlush();

        pending = PQ_GetNumWaiting();   // could be outdated, but will always be less than actual
        for (uint32_t i = 0; i < pending; i++) {
            PQ_Read(&c, 1);
            BSP_UART_Write(&c, 1, UART_USB);
        }
    }
}