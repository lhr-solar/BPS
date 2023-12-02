/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#include "Print_Queue.h"
#include "RTOS_BPS.h"
#include "stdio.h"
#include <stdarg.h>
#include "BSP_UART.h"

uint32_t size = 1024;

#define FIFO_TYPE char
#define FIFO_SIZE 1024
#define FIFO_NAME Print_Fifo
#include "fifo.h"

static Print_Fifo_t printFifo;
static OS_MUTEX printFifo_ready;
static OS_MUTEX printCall_Mutex;

uint32_t fifo_space();


/**
 * @brief Initializes the print queue
 * @param none
 * @return none
 */
void Print_Queue_Init() {
    Print_Fifo_renew(&printFifo);
    RTOS_BPS_MutexCreate(&printFifo_ready, "readyPrint");
    RTOS_BPS_MutexCreate(&printCall_Mutex, "printCall mutex");
}

/**
 * @brief Initializes the print queue
 * @return Returns if the string was added into the buffer
 * @param buffer String of formatted text to be added to the buffer
 * @return none
 */
bool Print_Queue_Append(char *buffer) {
    //Check if theres room
    if(strlen(buffer) > fifo_space()){
        RTOS_BPS_MutexPost(&printFifo_ready, OS_OPT_POST_1);
        return false;
    }

    while(*buffer != '\0') {

        //Add characters to the fifo one by one
        if(!(*buffer == '\r' || *buffer == '\n')){
            Print_Fifo_put(&printFifo, *buffer);
            buffer++;
        }else{
            //Endline check
            while((*buffer == '\r' || *buffer == '\n')){
                Print_Fifo_put(&printFifo, *buffer);
                buffer++;
            }

            RTOS_BPS_MutexPost(&printFifo_ready, OS_OPT_POST_1);
        }

        
    }
    return true;
}


 /**
 * @brief Blocks until the queue is ready to be dumped
 * @param message String to be dumped from the buffer
 * @param len Length of string to be dumped
 * @return none
 */
void Print_Queue_Pend(char *message, uint32_t *len) {
    uint32_t max_Size = 1024;
    (*len) = 0;
    RTOS_BPS_MutexPend(&printFifo_ready, OS_OPT_PEND_BLOCKING);
    while(!Print_Fifo_is_empty(&printFifo)){
        Print_Fifo_get(&printFifo, message);
        (*len)++;
        message++;
        if(*len >= max_Size){
            return;
        }
    }
}

/**
 * @brief Performs a "non-blocking" printf by dumping into a buffer for Task_Print.c
 * @param string String of formatted text to be added to the buffer
 * @return none
 */
void RTOS_BPS_NonBlocking_Printf(const char *format, ...){
    
    RTOS_BPS_MutexPend(&printCall_Mutex, OS_OPT_PEND_BLOCKING);
    va_list args;
    va_start(args, format);
    char buffer[size];
    
    int printLen = strlen(format); 
    vsnprintf(buffer, printLen, format, args);
    Print_Queue_Append(buffer);

    va_end(args);
    RTOS_BPS_MutexPost(&printCall_Mutex, OS_OPT_POST_1);
}

/**
 * @brief Performs a "blocking" printf by dumping into a buffer for Task_Print.c until success
 * @param string String of formatted text to be added to the buffer
 * @return none
 */
void RTOS_BPS_Blocking_Printf(const char *format, ...){
    
    RTOS_BPS_MutexPend(&printCall_Mutex, OS_OPT_PEND_BLOCKING);
    va_list args;
    va_start(args, format);
    char buffer[size];
    
    int printLen = strlen(format); 
    vsnprintf(buffer, printLen, format, args);
    while(!Print_Queue_Append(buffer));

    va_end(args);
    RTOS_BPS_MutexPost(&printCall_Mutex, OS_OPT_POST_1);
}

//Function will get removed when merged with my FIFO PR
uint32_t fifo_space(){
    if(!Print_Fifo_is_empty(&printFifo)){
        return ((printFifo.get - printFifo.put) + size) % size;
    }

    return size; 
}
