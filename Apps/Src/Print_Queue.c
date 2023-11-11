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
static OS_MUTEX printFifo_ready;
static OS_MUTEX printCall_Mutex;
static OS_MUTEX printIsFull_Mutex;

bool waitingToClear = false;


/**
 * @brief Initializes the print queue
 * @param none
 * @return none
 */
void Print_Queue_Init() {
    Print_Fifo_renew(&printFifo);
    RTOS_BPS_MutexCreate(&printFifo_ready, "readyPrint");
    RTOS_BPS_MutexCreate(&printCall_Mutex, "printCall mutex");
    RTOS_BPS_MutexCreate(&printIsFull_Mutex, "printIsFull mutex");
}

/**
 * @brief Initializes the print queue
 * @param buffer String of formatted text to be added to the buffer
 * @return none
 */


void Print_Queue_Append(char *buffer) {
    int a = 0;
    while(*buffer != 0 && a != 128) {
        if(Print_Fifo_is_full(&printFifo)){
            // If we are full, we need to wait
            // If we can remove stuff, lets remove from the length (a)
            // Flush printFifo
            // Continue
            while(a > 0){
                Print_Fifo_popback(&printFifo, NULL);
                buffer--;
                a--;
            }
            RTOS_BPS_MutexPost(&printFifo_ready, OS_OPT_POST_1);
            waitingToClear = true;
            RTOS_BPS_MutexPend(&printIsFull_Mutex, OS_OPT_PEND_BLOCKING);
        }
        Print_Fifo_put(&printFifo, *buffer);
        buffer++;
        a++;
        if((*buffer == '\r') || (*buffer == '\n')){
            RTOS_BPS_MutexPost(&printFifo_ready, OS_OPT_POST_1);
        }
    }
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
    RTOS_BPS_MutexPend(&printFifo_ready, OS_OPT_PEND_BLOCKING);
    while(!Print_Fifo_is_empty(&printFifo)){
        Print_Fifo_get(&printFifo, curr);
        (*len)++;
        curr++;
    }
    if(waitingToClear){
        waitingToClear = false;
        RTOS_BPS_MutexPost(&printIsFull_Mutex, OS_OPT_POST_1);
    }
}

/**
 * @brief Performs a "non-blocking" printf by dumping into a buffer for Task_Print.c
 * @param string String of formatted text to be added to the buffer
 * @return none
 */
void RTOS_BPS_snPrintf(const char *format, ...){
    
    RTOS_BPS_MutexPend(&printCall_Mutex, OS_OPT_PEND_BLOCKING);
    va_list args;
    va_start(args, format);
    char buffer[128];
    
    int a = strlen(format);                             // Per every group of 128 characters, it appends
    while(a > 128){
        vsnprintf(buffer, 128, format, args);
        Print_Queue_Append(buffer);
        format = format + 128;
        a = a - 128;
    }

    vsnprintf(buffer, a, format, args);
    Print_Queue_Append(buffer);
    va_end(args);
    

    RTOS_BPS_MutexPost(&printCall_Mutex, OS_OPT_POST_1);
}