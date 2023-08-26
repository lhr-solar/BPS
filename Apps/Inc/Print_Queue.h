/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#ifndef PRINT_QUEUE_H
#define PRINT_QUEUE_H

#include <stdbool.h>

void Print_Queue_Init(void);

bool Print_Queue_Append(char *buffer, unsigned int len);

void Print_Queue_Pend(char *buffer, unsigned int *len);

#endif