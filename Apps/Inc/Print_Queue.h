/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Asynchronous printf output via UART
 */

#ifndef PRINT_QUEUE_H
#define PRINT_QUEUE_H

#include <stdbool.h>
#include <stdint.h> 
#include "stddef.h"
#include "config.h"

// fifo size used by print queue task
#define PQ_PRINT_FIFO_SIZE          1024

// buffers are dynamically allocated from this pool whenever printf() is called.
// total space required is PQ_PRINTF_BUFFER_SIZE * PQ_PRINTF_BUFFER_COUNT bytes
#define PQ_PRINTF_BUFFER_SIZE       256 // in bytes
#define PQ_PRINTF_BUFFER_COUNT      8


/**
 * @brief Initializes memory pool for print queue
 * @note  must be called before OSStart()
 */
void PQ_InitMemPool(void);

/**
 * @brief Initializes print queue internal structures
 */
void PQ_Init(void);

/**
 * @brief Write to the global print queue. This is threadsafe.
 * 
 * @param data  pointer to data
 * @param len   length of data in bytes
 * @return true on success, false on fail (queue full)
 */
bool PQ_Write(char *data, uint32_t len);

/**
 * @brief Read from the global print queue. This is threadsafe.
 * 
 * @param data  pointer to read buffer
 * @param len   requested read length
 * @return true on success, false on fail (data in queue less than requested)
 */
bool PQ_Read(char *data, uint32_t len);

/**
 * @brief Essentially queue->length() function.
 * 
 * @return number of elements in print queue
 */
uint32_t PQ_GetNumWaiting(void);

/**
 * @brief Signal Print Task (or someone waiting on PQ_WaitForFlush())
 */
void PQ_Flush(void);

/**
 * @brief As the name says. This is a blocking call. Pairs with PQ_Flush().
 */
void PQ_WaitForFlush(void);

/**
 * @brief threadsafe wrapper for printf(). DO NOT USE DIRECTLY -- just use printf
 * 
 * @param printf format string
 * @note to enable, BPS_ENABLE_PRINT_OUTPUT must be set to true
 */
int _printf_internal(const char *format, ...);



#ifndef SIMULATION

#if BPS_ENABLE_PRINT_OUTPUT
#define printf(...) _printf_internal(__VA_ARGS__)
#else   // BPS_ENABLE_PRINT_OUTPUT disabled
#define printf(...)
#endif  // BPS_ENABLE_PRINT_OUTPUT

#endif // SIMULATION

#endif  // PRINT_QUEUE_H

