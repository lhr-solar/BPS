/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** CAN_Queue.c
 * Queue that holds all CAN messages that Task_CANBusConsumer needs to send.
 */

#include "CAN_Queue.h"
#include "os.h"
#include "CANbus.h"
#include "RTOS_BPS.h"
#include "Tasks.h"

// fifo
#define FIFO_TYPE CANMSG_t
#define FIFO_SIZE 256
#define FIFO_NAME CAN_fifo
#include "fifo.h"

static CAN_fifo_t canFifo;
static OS_SEM canFifo_Sem4;
static OS_MUTEX canFifo_Mutex;

void CAN_Queue_Init(void) {
    RTOS_BPS_MutexCreate(&canFifo_Mutex, "CAN queue mutex");
    RTOS_BPS_SemCreate(&canFifo_Sem4,
                "CAN queue semaphore",
                0);
    RTOS_BPS_MutexPend(&canFifo_Mutex, OS_OPT_PEND_BLOCKING);
    CAN_fifo_renew(&canFifo);
    RTOS_BPS_MutexPost(&canFifo_Mutex, OS_OPT_POST_NONE);
}

ErrorStatus CAN_Queue_Post(CANMSG_t message) {
    RTOS_BPS_MutexPend(&canFifo_Mutex, OS_OPT_PEND_BLOCKING);
    bool success = CAN_fifo_put(&canFifo, message);
    RTOS_BPS_MutexPost(&canFifo_Mutex, OS_OPT_POST_NONE);

    if (success) {
        RTOS_BPS_SemPost(&canFifo_Sem4, OS_OPT_POST_1);
    }

    return success ? SUCCESS : ERROR;
}

ErrorStatus CAN_Queue_Pend(CANMSG_t *message) {
    RTOS_BPS_SemPend(&canFifo_Sem4, OS_OPT_PEND_BLOCKING);
    RTOS_BPS_MutexPend(&canFifo_Mutex, OS_OPT_PEND_BLOCKING);
    bool result = CAN_fifo_get(&canFifo, message);
    RTOS_BPS_MutexPost(&canFifo_Mutex, OS_OPT_POST_NONE);
    return result ? SUCCESS : ERROR;
}

