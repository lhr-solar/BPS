/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

/** Print_Queue.c
 * Asynchronous printf output via UART
 * 
 * This module provides a threadsafe and non-blocking printf() wrapper. Can theoretically be 
 * called from interrupts but this is untested and will probably result in errors. If prints 
 * are called too often, MESSAGES WILL BE DROPPED. This is designed to avoid tripping WDog.
 * 
 * Roughly, each printf() call results in the following steps:
 * [1] A buffer is allocated from a pool
 *  2. The formatted string is written to the buffer using sprintf()
 * [3] The buffer is copied to a global FIFO
 * The bracketed steps require mutual exclusion. Note the compute-heavy portion 
 * (string formatting) is not protected by a mutex and is thus nonblocking.
 * 
 * A separate task (Task_Print) then reads from the global FIFO and sends data out to UART.
 */

#include "Print_Queue.h"
#include "os.h"     // use this instead of RTOS_BPS as RTOS_BPS lacks functionality
#include "stdio.h"
#include <stdarg.h>
#include "BSP_UART.h"
#include "Tasks.h"  // for assertOSError()

#ifndef SIMULATION

// Compiler messages go here so they don't get spammed when building
#if BPS_ENABLE_PRINT_OUTPUT
#pragma message("Printf output is enabled (default). To disable, set BPS_ENABLE_PRINT_OUTPUT to 'false' when building: 'make DEFINES=\"BPS_ENABLE_PRINT_OUTPUT=false\"'")
#else   // BPS_ENABLE_PRINT_OUTPUT disabled
#pragma message("Printf output is disabled. To enable, set BPS_ENABLE_PRINT_OUTPUT to 'true' when building: 'make DEFINES=\"BPS_ENABLE_PRINT_OUTPUT=true\"'")
#endif  // BPS_ENABLE_PRINT_OUTPUT

#endif  // SIMULATION

#define FFIFO_TYPE char
#define FFIFO_SIZE PQ_PRINT_FIFO_SIZE
#define FFIFO_NAME PQFifo
#include "fastfifo.h"
static PQFifo_t PQFifo;

static OS_MEM   PQPrintfPool;
static char     PQPrintfBuffers[PQ_PRINTF_BUFFER_COUNT][PQ_PRINTF_BUFFER_SIZE];

static OS_SEM   PQ_SignalFlush_Sem4;
static OS_MUTEX PQ_Mutex;

/**
 * @brief Initializes memory pool for print queue
 * @note  must be called before OSStart()
 */
void PQ_InitMemPool(void) {
    OS_ERR err;
    OSMemCreate(&PQPrintfPool,
                "Print Queue Printf Pool",
                &PQPrintfBuffers[0][0],
                PQ_PRINTF_BUFFER_COUNT,
                PQ_PRINTF_BUFFER_SIZE,
                &err);
    assertOSError(err);
}

/**
 * @brief Initializes print queue internal structures
 */
void PQ_Init(void) {

    // setup fifo
    PQFifo_renew(&PQFifo);

    OS_ERR err;
    OSSemCreate(&PQ_SignalFlush_Sem4, "PQ_SignalFlush", 0, &err);
    assertOSError(err);
    OSMutexCreate(&PQ_Mutex, "PQ_Mutex", &err);
    assertOSError(err);
}

/**
 * @brief Write to the global print queue. This is threadsafe.
 * 
 * @param data  pointer to data
 * @param len   length of data in bytes
 * @return true on success, false on fail (queue full)
 */
bool PQ_Write(char *data, uint32_t len) {
    OS_ERR err;
    bool in_isr = OSIntNestingCtr > 0;

    if (!in_isr) { // not in ISR -- ok to pend
        OSMutexPend(&PQ_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    }
    bool status = PQFifo_put(&PQFifo, data, (int)len);
    if (!in_isr) {
        OSMutexPost(&PQ_Mutex, OS_OPT_POST_NONE, &err);
    }
    return status;
}

/**
 * @brief Read from the global print queue. This is threadsafe.
 * 
 * @param data  pointer to read buffer
 * @param len   requested read length
 * @return true on success, false on fail (data in queue less than requested)
 */
bool PQ_Read(char *data, uint32_t len) {
    OS_ERR err;
    bool in_isr = OSIntNestingCtr > 0;

    if (!in_isr) { // not in ISR -- ok to pend
        OSMutexPend(&PQ_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    }
    bool status = PQFifo_get(&PQFifo, data, (int)len);
    if (!in_isr) {
        OSMutexPost(&PQ_Mutex, OS_OPT_POST_NONE, &err);
    }
    return status;
}

/**
 * @brief Essentially queue->length() function.
 * 
 * @return number of elements in print queue
 */
uint32_t PQ_GetNumWaiting(void) {
    return (uint32_t)PQFifo_len(&PQFifo);
}

/**
 * @brief Signal Print Task (or someone waiting on PQ_WaitForFlush())
 */
void PQ_Flush(void) {
    OS_ERR err;
    OSSemPost(&PQ_SignalFlush_Sem4, OS_OPT_POST_ALL, &err);
}

/**
 * @brief As the name says. Will timeout automatically at PQ_FLUSH_TIMEOUT_OS_TICKS. Pairs with PQ_Flush().
 */
void PQ_WaitForFlush(void) {
    OS_ERR err;
    OSSemPend(&PQ_SignalFlush_Sem4, PQ_FLUSH_TIMEOUT_OS_TICKS, OS_OPT_PEND_BLOCKING, NULL, &err);
}

/**
 * @brief threadsafe wrapper for printf(). DO NOT USE DIRECTLY -- just use printf
 * 
 * @param printf format string
 * @note to enable, BPS_ENABLE_PRINT_OUTPUT must be set to true
 */
int _printf_internal(const char *format, ...) {
    va_list args;
    va_start(args, format);

    OS_ERR err;

    // get a buffer from the pool
    char *buffer = (char *)OSMemGet(&PQPrintfPool, &err);
    if (err != OS_ERR_NONE) return 0;

    // write to buffer
    int len = vsnprintf(buffer, PQ_PRINTF_BUFFER_SIZE, format, args);
    len = MIN(len, PQ_PRINTF_BUFFER_SIZE);

    // copy from buffer to fifo
    bool status = PQ_Write(buffer, len);

    // release buffer
    OSMemPut(&PQPrintfPool, buffer, &err);

    // signal flush
    if ((PQ_GetNumWaiting() > (PQ_PRINT_FIFO_SIZE - PQ_PRINTF_BUFFER_SIZE))
        || memchr(buffer, '\n', len)) {

        PQ_Flush();
    }

    va_end(args);
    return status ? len : EOF;
}


