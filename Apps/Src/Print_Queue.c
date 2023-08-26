/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#include "Print_Queue.h"
#include "RTOS_UART.h"
#include "RTOS_BPS.h"

#define FIFO_TYPE char*
#define FIFO_SIZE (128)
#define FIFO_NAME printFifo
#include "fifo.h"

static OS_MUTEX printFifo_Mutex;

/**
 * @brief Initializes the print queue
 * @param none
 * @return none
 */
void Print_Queue_Init() {
    printFifo_new();
}

bool New_Line_Check(){}

/**
 * @brief Initializes the print queue
 * @param buffer String of formatted text to be added to the buffer
 * @param len Length of formatted string
 * @return If the write was successful (there was room in the buffer)
 */
bool Print_Queue_Append(char *buffer, unsigned int len) {
    bool result = printFifo_put(&printFifo, buffer);

    if(printFifo_is_full(&printFifo) || New_Line_Check()) {
        RTOS_BPS_MutexPost(%printFifo_Mutex, OS_OPT_POST_1);
    }

    return result;
}


 /**
 * @brief Returns if the queue is ready to be dumped
 * @param message String to be dumped from the buffer
 * @param len Length of string to be dumped
 * @return none
 */

void Print_Queue_Pend(char *message, unsigned int *len) {
    RTOS_BPS_MutexPend(&printFifo_Mutex, OS_OPT_PEND_NON_BLOCKING);
    bool result = printFifo_get(&printFifo, message);
    RTOS_BPS_MutexPost(&printFifo_Mutex, OS_OPT_POST_NONE);
}

bool New_Line_Check(){
    char* topChar;
    printFifo_peek(&printFifo, topChar);
    return (topChar == '\n');
}
