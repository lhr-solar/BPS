/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#include "Print_Queue.h"
#include "RTOS_BPS.h"
#include "stdio.h"
#include <stdarg.h>
#include "BSP_UART.h"


#define FIFO_TYPE char
#define FIFO_SIZE (128)
#define FIFO_NAME Print_Fifo
#include "fifo.h"

static Print_Fifo_t printFifo;
static OS_SEM printFifo_ready;
static OS_MUTEX printCall_Mutex;

/**
 * @brief Initializes the print queue
 * @param none
 * @return none
 */
void Print_Queue_Init() {
    Print_Fifo_renew(&printFifo);
    RTOS_BPS_SemCreate(&printFifo_ready, "readyPrint", 0);
    RTOS_BPS_MutexCreate(&printCall_Mutex, "printCall mutex");
}

/**
 * @brief Initializes the print queue
 * @param buffer String of formatted text to be added to the buffer
 * @return If the write was successful (there was room in the buffer)
 */


bool Print_Queue_Append(char *buffer) {
    int a = 0;
    while(*buffer != 0 && a != 256) {
        Print_Fifo_put(&printFifo, *buffer);
        buffer++;
        a++;
    }
    RTOS_BPS_SemPost(&printFifo_ready, OS_OPT_POST_NONE);
    return true;
}


 /**
 * @brief Blocks until the queue is ready to be dumped
 * @param message String to be dumped from the buffer
 * @param len Length of string to be dumped
 * @return none
 */
void Print_Queue_Pend(char *message, uint32_t *len) {
    char *curr = message;
    (*len) = 0;
    RTOS_BPS_SemPend(&printFifo_ready, OS_OPT_PEND_BLOCKING);
    while(!Print_Fifo_is_empty(&printFifo)){
        Print_Fifo_get(&printFifo, curr);
        (*len)++;
        curr++;
    }
    return;
}


void RTOS_BPS_snPrintf(const char *format, ...){
    RTOS_BPS_MutexPend(&printCall_Mutex, OS_OPT_PEND_BLOCKING);
    va_list args;
    va_start(args, format);

    int result = vsnprintf(NULL, 0, format, args);
    char buffer[256];
    volatile int x = 1;
    while(x < 100){
        x = x + 1;
    }
    if(result<255){
        vsnprintf(buffer, result+1, format, args);
    }else{
        vsnprintf(buffer, 256, format, args);
    }
    va_end(args);

    Print_Queue_Append(buffer);

    RTOS_BPS_MutexPost(&printCall_Mutex, OS_OPT_POST_NONE);
}