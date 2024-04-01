/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** Print_Queue.c
 * Queue that holds all characters to be printed that Task_Print needs to send to UART.
 */

#include "Print_Queue.h"
#include "RTOS_BPS.h"
#include "stdio.h"
#include <stdarg.h>
#include "BSP_UART.h"
#include "Tasks.h"  // for assertOSError()

#define FFIFO_TYPE char
#define FFIFO_SIZE PQ_PRINT_FIFO_SIZE
#define FFIFO_NAME PQFifo
#include "fastfifo.h"
static PQFifo_t PQFifo;

static OS_MEM  PQPrintfPool;
static char    PQPrintfBuffers[PQ_PRINTF_BUFFER_COUNT][PQ_PRINTF_BUFFER_SIZE];

static BPS_OS_SEM PQ_SignalFlush_Sem4;
static BPS_OS_MUTEX PQ_Mutex;

void PQ_Init(void) {
    // setup fifo
    PQFifo = PQFifo_new();

    // setup memory pool
    BPS_OS_ERR err;
    OSMemCreate(&PQPrintfPool,
                "Print Queue Printf Pool",
                PQPrintfBuffers,
                PQ_PRINTF_BUFFER_COUNT,
                PQ_PRINTF_BUFFER_SIZE,
                &err);
    assertOSError(err);

    RTOS_BPS_SemCreate(&PQ_SignalFlush_Sem4, "PQ_SignalFlush", 0);
    RTOS_BPS_MutexCreate(&PQ_Mutex, "PQ_Mutex");
}

bool PQ_Write(char *data, uint32_t len) {
    if (OSIntNestingCtr == 0) { // not in ISR -- ok to pend
        RTOS_BPS_MutexPend(&PQ_Mutex, OS_OPT_PEND_BLOCKING);
    }
    bool status = PQFifo_put(&PQFifo, data, (int)len);
    if (OSIntNestingCtr == 0) {
        RTOS_BPS_MutexPost(&PQ_Mutex, OS_OPT_POST_NONE);
    }
    return status;
}

bool PQ_Read(char *data, uint32_t len) {
    if (OSIntNestingCtr == 0) { // not in ISR -- ok to pend
        RTOS_BPS_MutexPend(&PQ_Mutex, OS_OPT_PEND_BLOCKING);
    }
    bool status = PQFifo_get(&PQFifo, data, (int)len);
    if (OSIntNestingCtr == 0) {
        RTOS_BPS_MutexPost(&PQ_Mutex, OS_OPT_POST_NONE);
    }
    return status;
}

uint32_t PQ_GetNumWaiting(void) {
    return (uint32_t)PQFifo_len(&PQFifo);
}

void PQ_Flush(void) {
    RTOS_BPS_SemPost(&PQ_SignalFlush_Sem4, OS_OPT_POST_ALL | OS_OPT_POST_NO_SCHED);
}

void PQ_WaitForFlush(void) {
    RTOS_BPS_SemPend(&PQ_SignalFlush_Sem4, OS_OPT_PEND_BLOCKING);
}

int _printf_internal(const char *format, ...) {
    BPS_OS_ERR err;

    va_list args;
    va_start(args, format);

    // get a buffer from the pool
    char *buffer = (char *)OSMemGet(&PQPrintfPool, &err);
    assertOSError(err);

    // write to buffer
    int len = vsnprintf(buffer, PQ_PRINTF_BUFFER_SIZE, format, args);

    // copy from buffer to fifo
    bool status = PQ_Write(buffer, len);

    // release buffer
    OSMemPut(&PQPrintfPool, buffer, &err);
    assertOSError(err);

    // signal flush
    if (memchr(buffer, '\n', len)) {
        PQ_Flush();
    }

    va_end(args);
    return status ? len : 0;
}


