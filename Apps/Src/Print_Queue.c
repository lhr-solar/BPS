/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#include "Print_Queue.h"
#include "RTOS_BPS.h"

#define FIFO_TYPE char
#define FIFO_SIZE (128)
#define FIFO_NAME Print_Fifo
#include "fifo.h"

static Print_Fifo_t printFifo;
static OS_SEM printFifo_Sem;
static OS_MUTEX printFifo_Mutex;


/**
 * @brief Initializes the print queue
 * @param none
 * @return none
 */
void Print_Queue_Init() {
    Print_Fifo_renew(&printFifo);
    RTOS_BPS_SemCreate(&printFifo_Sem, "printSem", 0);
    RTOS_BPS_MutexCreate(&printFifo_Mutex, "PRINT queue mutex");
}

/**
 * @brief Initializes the print queue
 * @param buffer String of formatted text to be added to the buffer
 * @param len Length of formatted string
 * @return If the write was successful (there was room in the buffer)
 */
bool Print_Queue_Append(char *buffer, uint32_t len) {
    uint32_t i = 0;
    RTOS_BPS_MutexPend(&printFifo_Mutex, OS_OPT_PEND_BLOCKING);
    while(i < len){
        Print_Fifo_put(&printFifo, buffer[i]);
        i++;
        RTOS_BPS_SemPost(&printFifo_Sem, OS_OPT_POST_1);
    }
    RTOS_BPS_MutexPost(&printFifo_Mutex, OS_OPT_POST_NONE);
    return true;
}


 /**
 * @brief Blocks until the queue is ready to be dumped
 * @param message String to be dumped from the buffer
 * @param len Length of string to be dumped
 * @return none
 */
void Print_Queue_Pend(char *message, uint32_t *len) {
    (*len) = 0;
    while(1){
        RTOS_BPS_SemPend(&printFifo_Sem, OS_OPT_PEND_BLOCKING);
        RTOS_BPS_MutexPend(&printFifo_Mutex, OS_OPT_PEND_BLOCKING);
        Print_Fifo_get(&printFifo, message);
        (*len)++;
        if ((*message == '\r') || (*message == '\n')){
            return;
        }
        message++;
        RTOS_BPS_MutexPost(&printFifo_Mutex, OS_OPT_POST_NONE);
    }
}

