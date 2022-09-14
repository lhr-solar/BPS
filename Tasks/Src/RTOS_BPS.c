/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"

void RTOS_BPS_SemPend(void) {

} 
void RTOS_BPS_SemPost(void) {

}
void RTOS_BPS_MutexPend(void){

}

/**
 * @brief   Posts the specified Mutex. (For future reference, Post is the same as Give)
 * @param   *mutex - pointer to the specified RTOS Mutex object
 * @param   options - a parameter which determines what kind of Post MutexPost performs
 * @return  none
 */
void RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options) {
    BPS_OS_ERR err;
    OSMutexPost(mutex, options, &err);
    assertOSError(err);
}

/**
 * Note that the incoming BPS_OS_QUEUE queue !!MUST!! be created already.
 * @brief   Puts a message into the RTOS Queue following FIFO order (i.e., POST something into the Queue). https://docs.silabs.com/micrium/latest/micrium-kernel-api/03-kernel-message-queue-api
 * @param   *queue - pointer to the Queue to write a message to
 * @param   *msg - void pointer to the message you are going to send
 * @param   size - payload size of *msg (this changes in FreeRTOS)
 * @return  none
 */
void RTOS_BPS_FifoPut(BPS_OS_QUEUE* queue, void* msg, BPS_OS_MSG_SIZE size) {
    BPS_OS_ERR err;
    OSQPost(queue, msg, size, OS_OPT_POST_FIFO, &err);
    assertOSError(err);
}
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}