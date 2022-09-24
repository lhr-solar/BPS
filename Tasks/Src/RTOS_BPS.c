/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"

void RTOS_BPS_SemPend(void) {

} 
void RTOS_BPS_SemPost(void) {

}

/**
 * @brief   Waits for Mutex, assigns timestamp and any error to err and ticks
 * @param   *mutex - pointer to mutex
 * @param   timeout - timeout period, if 0 will wait forever until resource available
 * @param   options - determines what the mutex will do, ie: block or not block
 * @return  none
 */
void RTOS_BPS_MutexPend(BPS_OS_MUTEX* mutex, BPS_OS_TICK timeout, BPS_OS_OPT opt) {
    BPS_OS_ERR err;
    BPS_CPU_TS ticks;
    OSMutexPend(mutex, timeout, opt, &ticks, &err);
    assertOSError(err);
}

/**
 * @brief Initializes a mutex object.
 * @param *mut - pointer to a mutex to initialize
 * @param name - char* of the name of the mutex
 * @return none
 */
void RTOS_BPS_MutexCreate(BPS_OS_MUTEX *mut, char* name) {
    BPS_OS_ERR err;
    OSMutexCreate(mut, name, &err);
    assertOSError(err);
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
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}