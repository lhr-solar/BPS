/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#ifndef PRINT_QUEUE_H
#define PRINT_QUEUE_H

#include <stdbool.h>
#include <stdint.h> 
#include "stddef.h"

// fifo size used by print queue task
// note: double this size is actually used as we use a contiguous block circular buffer
#define PQ_PRINT_FIFO_SIZE          1024

// buffers are dynamically allocated from this pool whenever printf() is called.
// total space required is PQ_PRINTF_BUFFER_SIZE * PQ_PRINTF_BUFFER_COUNT bytes
#define PQ_PRINTF_BUFFER_SIZE       256 // in bytes
#define PQ_PRINTF_BUFFER_COUNT      8

/**
 * @brief Initializes print queue internal structures
 */
void PQ_Init(void);

bool PQ_Write(char *data, uint32_t len);
bool PQ_Read(char *data, uint32_t len);
void PQ_Flush(void);
void PQ_WaitForFlush(void);
uint32_t PQ_GetNumWaiting(void);

/**
 * @brief threadsafe wrapper for printf(). DO NOT USE DIRECTLY -- just use printf
 * @note to enable, BPS_ENABLE_PRINT_OUTPUT must be set to true
 */
int _printf_internal(const char *format, ...);

/**
 * @brief Performs a "blocking" printf by attempting to dump into the buffer until success
 * @param string String of formatted text to be added to the buffer
 * @return none
 */
void RTOS_BPS_Blocking_Printf(const char *format, ...);

#ifdef BPS_ENABLE_PRINT_OUTPUT

#ifndef SIMULATION
#define printf(...) _printf_internal(__VA_ARGS__)
#else // running in SIMULATION mode
#define printf(...) printf(__VA_ARGS__)
#endif

#else   // BPS_ENABLE_PRINT_OUTPUT disabled

#define printf(...)

#endif  // BPS_ENABLE_PRINT_OUTPUT

#endif

/**
 * TODO:
 * Error handling on fifo full
 * Test everything
 * Add back in CAN print mirroring
 */
