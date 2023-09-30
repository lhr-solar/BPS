/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "Print_Queue.h"
#include "BSP_UART.h"
//#include "CANbus.h"
//#include "CAN_Queue.h"


void Task_Print(void *p_arg) {
    (void)p_arg;

    char message;
    unsigned int len;

    while(1) {
        // BLOCKING =====================
        // Wait for Print Queue to Dump
        Print_Queue_Pend(&message, &len);
        
        //Port 2 for now
        BSP_UART_Write(&message, len, UART_USB);
    }
}