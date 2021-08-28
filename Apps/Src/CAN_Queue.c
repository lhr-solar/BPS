/* Copyright (c) 2020 UT Longhorn Racing Solar */
/** CAN_Queue.c
 * Queue that holds all CAN messages that Task_CANBusConsumer needs to send.
 */

#include "CAN_Queue.h"
#include "os.h"
#include "CANbus.h"
#include "Tasks.h"

// fifo
#define FIFO_TYPE CANMSG_t
#define FIFO_SIZE 256
#define FIFO_NAME CAN_fifo
#include "fifo.h"

static CAN_fifo_t canFifo;
static OS_SEM canFifoSem;
static OS_MUTEX canFifoMutex;

void CAN_Queue_Init(void) {
    OS_ERR err;
    CPU_TS ticks;
    OSMutexCreate(&canFifoMutex, "CAN queue mutex", &err);
    assertOSError(err);
    OSSemCreate(&canFifoSem,
                "CAN queue semaphore",
                0,
                &err);
    assertOSError(err);
    OSMutexPend(&canFifoMutex, 0, OS_OPT_POST_NONE, &ticks, &err);
    assertOSError(err);
    CAN_fifo_renew(&canFifo);
    OSMutexPost(&canFifoMutex, OS_OPT_POST_NONE, &err);
    assertOSError(err);
}

void CAN_Queue_Post(CANMSG_t message) {
    OS_ERR err;
    CPU_TS ticks;
    OSMutexPend(&canFifoMutex, 0, OS_OPT_POST_NONE, &ticks, &err);
    assertOSError(err);
    CAN_fifo_put(&canFifo, message);
    OSMutexPost(&canFifoMutex, OS_OPT_POST_NONE, &err);
    assertOSError(err);
    OSSemPost(&canFifoSem, OS_OPT_POST_1, &err);
    assertOSError(err);
}

ErrorStatus CAN_Queue_Pend(CANMSG_t *message) {
    OS_ERR err;
	CPU_TS ticks;
    
    OSSemPend(&canFifoSem, 0, OS_OPT_PEND_BLOCKING, &ticks, &err);
    assertOSError(err);
    OSMutexPend(&canFifoMutex, 0, OS_OPT_POST_NONE, &ticks, &err);
    assertOSError(err);
    bool result = CAN_fifo_get(&canFifo, message);
    OSMutexPost(&canFifoMutex, OS_OPT_POST_NONE, &err);
    assertOSError(err);
    return result ? SUCCESS : ERROR;
}

