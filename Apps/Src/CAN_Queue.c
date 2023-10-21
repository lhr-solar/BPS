/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** CAN_Queue.c
 * Holds both queues for Task_CANProducer and Task_CANBusConsumer
 * works by calling respective post/pend to each queue so other tasks can grab from the queues
 * 
 */

#include "CAN_Queue.h"
#include "CANbus.h"
#include "RTOS_BPS.h"
#include "Tasks.h"

// fifo
#define FIFO_TYPE CANMSG_t
#define FIFO_SIZE 512
#define FIFO_NAME CAN_fifo_TRANSMIT
#include "fifo.h"

#define FIFO_TYPE CANMSG_t
#define FIFO_SIZE 512
#define FIFO_NAME CAN_fifo_RECEIVE
#include "fifo.h"

static CAN_fifo_TRANSMIT_t canFifo_TRANSMIT;
static CAN_fifo_RECEIVE_t canFifo_RECEIVE;

static OS_SEM canFifo_Transmit_Sem4;
static OS_MUTEX canFifo_Transmit_Mutex;

static OS_SEM canFifo_Receive_Sem4;
static OS_MUTEX canFifo_Receive_Mutex;


/**
 * @brief: initializes both Receive and Transmit fifo + sema4s and mutexes
 * @param: none
 * @return: none
*/
void CAN_Queue_Init(void) {
    RTOS_BPS_MutexCreate(&canFifo_Transmit_Mutex, "CAN transmit queue mutex");
    RTOS_BPS_SemCreate(&canFifo_Transmit_Sem4, "CAN transmit queue semaphore", 0);
    
    RTOS_BPS_MutexCreate(&canFifo_Receive_Mutex, "CAN receive queue mutex");
    RTOS_BPS_SemCreate(&canFifo_Receive_Sem4, "CAN receive queue semaphore", 0);
    
    //TODO: i dont think these mutexes are necessary honestly, maybe remove??
    RTOS_BPS_MutexPend(&canFifo_Transmit_Mutex, OS_OPT_PEND_BLOCKING);
    CAN_fifo_TRANSMIT_renew(&canFifo_TRANSMIT);
    RTOS_BPS_MutexPost(&canFifo_Transmit_Mutex, OS_OPT_POST_NONE);

    RTOS_BPS_MutexPend(&canFifo_Receive_Mutex, OS_OPT_PEND_BLOCKING);
    CAN_fifo_RECEIVE_renew(&canFifo_RECEIVE);
    RTOS_BPS_MutexPost(&canFifo_Receive_Mutex, OS_OPT_POST_NONE);

}

/**
 * @brief: Places a CAN message into transmit queue 
 * @param: CAN message itself
 * @return: error status
*/
ErrorStatus CAN_TransmitQueue_Post(CANMSG_t message) {
    RTOS_BPS_MutexPend(&canFifo_Transmit_Mutex, OS_OPT_PEND_BLOCKING);
    bool success = CAN_fifo_TRANSMIT_put(&canFifo_TRANSMIT, message);
    RTOS_BPS_MutexPost(&canFifo_Transmit_Mutex, OS_OPT_POST_NONE);

    if (success) {
        RTOS_BPS_SemPost(&canFifo_Transmit_Sem4, OS_OPT_POST_1);
    }

    return success ? SUCCESS : ERROR;
}

/**
 * @brief: (Blocking) grabs a CANMSG from the transmit queue
 * @param: pointer to CANMSG
 * @return: error status
*/
ErrorStatus CAN_TransmitQueue_Pend(CANMSG_t *message) {
    RTOS_BPS_SemPend(&canFifo_Transmit_Sem4, OS_OPT_PEND_BLOCKING);
    RTOS_BPS_MutexPend(&canFifo_Transmit_Mutex, OS_OPT_PEND_BLOCKING);
    bool result = CAN_fifo_TRANSMIT_get(&canFifo_TRANSMIT, message);
    RTOS_BPS_MutexPost(&canFifo_Transmit_Mutex, OS_OPT_POST_NONE);
    return result ? SUCCESS : ERROR;
}

/**
 * @brief puts a CANMSG into the Receive queue
 * @param CANMSG_t itself
 * @return error status
*/
ErrorStatus CAN_ReceiveQueue_Post(CANMSG_t message) {
    RTOS_BPS_MutexPend(&canFifo_Receive_Mutex, OS_OPT_PEND_BLOCKING);
    bool success = CAN_fifo_RECEIVE_put(&canFifo_RECEIVE, message);
    RTOS_BPS_MutexPost(&canFifo_Receive_Mutex, OS_OPT_POST_NONE);

    if (success) {
        RTOS_BPS_SemPost(&canFifo_Receive_Sem4, OS_OPT_POST_1);
    }

    return success ? SUCCESS : ERROR;
}

/**
 * @brief: (NON Blocking) grabs a CANMSG from the receive queue
 * @param: pointer to CANMSG
 * @return: error status
 * @note: SemPend (Micrium) will have err = OS_ERR_PEND_WOULD_BLOCK, which is okay
*/
ErrorStatus CAN_ReceiveQueue_Pend(CANMSG_t *message) {
    RTOS_BPS_SemPend(&canFifo_Receive_Sem4, OS_OPT_PEND_NON_BLOCKING);
    RTOS_BPS_MutexPend(&canFifo_Receive_Mutex, OS_OPT_PEND_BLOCKING);
    bool result = CAN_fifo_RECEIVE_get(&canFifo_RECEIVE, message);
    RTOS_BPS_MutexPost(&canFifo_Receive_Mutex, OS_OPT_POST_NONE);
    return result ? SUCCESS : ERROR;
}

