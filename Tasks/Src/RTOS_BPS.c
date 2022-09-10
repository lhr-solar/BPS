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
 * @return  BPS_OS_ERR value
 */
BPS_OS_ERR RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options) {
    BPS_OS_ERR err;
    OSMutexPost(mutex, options, &err);
    assertOSError(err);
    return err;
}
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}