/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"

void RTOS_BPS_SemPend() {

} 
void RTOS_BPS_SemPost() {

}
void RTOS_BPS_MutexPend(void){

}
// Connor Leu
// Post == Give
BPS_OS_ERR RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options) {
    BPS_OS_ERR err;
    OSMutexPost(mutex, options, &err); // calls MutexPost with all specified params
    assertOSError(err);
    return err;
    /**
     * FreeRTOS only uses ONE parameter for its MutexPost, a SemaphoreHandle_t object (no ptr)
     * CANNOT be used in an ISR.
     * xSemaphoreGive() returns pdTRUE if the semaphore is released, pdFALSE if there is some error.
     * 
     * Blocking vs Nonblocking appears to come from https://www.freertos.org/xQueueSelectFromSet.html, and
     * https://www.freertos.org/xQueueCreateSet.html
     * which is a separate structure used and not just an option passed into the pend/post (take/give) functions
    */ 
}
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}