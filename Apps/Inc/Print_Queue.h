/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#ifndef PRINT_QUEUE_H
#define PRINT_QUEUE_H

#include <stdbool.h>
#include <stdint.h> 
#include "stddef.h"


/**
 * @brief Initializes the print queue
 * @param none
 * @return none
 */
void Print_Queue_Init(void);

/**
 * @brief Initializes the print queue
 * @param buffer String of formatted text to be added to the buffer
 * @return none
 */
bool Print_Queue_Append(char *buffer);

 /**
 * @brief Blocks until the queue is ready to be dumped
 * @param message String to be dumped from the buffer
 * @param len Length of string to be dumped
 * @return none
 */
void Print_Queue_Pend(char *buffer, uint32_t *len);

/**
 * @brief Performs a "non-blocking" printf by dumping into a buffer for Task_Print.c
 * @param string String of formatted text to be added to the buffer
 * @return none
 */
void RTOS_BPS_NonBlocking_Printf(const char *format, ...);

/**
 * @brief Performs a "blocking" printf by attempting to dump into the buffer until success
 * @param string String of formatted text to be added to the buffer
 * @return none
 */
void RTOS_BPS_Blocking_Printf(const char *format, ...);

#endif