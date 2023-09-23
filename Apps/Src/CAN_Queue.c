/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** CAN_Queue.c
 * Queue that holds all CAN messages that Task_CANBusConsumer needs to send.
 */

#include "CAN_Queue.h"
#include "CANbus.h"
#include "RTOS_BPS.h"
#include "Tasks.h"

// fifo
#define FIFO_TYPE CANMSG_t
#define FIFO_SIZE 256
#define FIFO_NAME CAN_fifo
#include "fifo.h"

static CAN_fifo_t canFifo;
static SemaphoreHandle_t canFifo_Sem4;
static SemaphoreHandle_t canFifo_Mutex;

void CAN_Queue_Init(void) {
    canFifo_Mutex = xSemaphoreCreateMutex();
	canFifo_Sem4 = xSemaphoreCreateBinary();
	xSemaphoreTake(canFifo_Mutex, (TickType_t)portMAX_DELAY); 
    CAN_fifo_renew(&canFifo);
	xSemaphoreGive(canFifo_Mutex);
}

ErrorStatus CAN_Queue_Post(CANMSG_t message) {
	xSemaphoreTake(canFifo_Mutex, (TickType_t)portMAX_DELAY); 
    bool success = CAN_fifo_put(&canFifo, message);
	xSemaphoreGive(canFifo_Mutex);

    if (success) {
	xSemaphoreGive(canFifo_Sem4);
    }

    return success ? SUCCESS : ERROR;
}

ErrorStatus CAN_Queue_Pend(CANMSG_t *message) {
	xSemaphoreTake(canFifo_Sem4, (TickType_t)portMAX_DELAY);
	xSemaphoreTake(canFifo_Mutex, (TickType_t)portMAX_DELAY); 
    bool result = CAN_fifo_get(&canFifo, message);
	xSemaphoreGive(canFifo_Mutex);
    return result ? SUCCESS : ERROR;
}

