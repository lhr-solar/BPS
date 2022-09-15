/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"

void RTOS_BPS_SemPend(void) {

} 
void RTOS_BPS_SemPost(void) {

}

/**
 * @brief Creates and returns a mutex. Creates and returns a mutex. Mutex objects are used in Pend and Post, and for Micrium, require no 
 *        initialization besides declaring a variable of BPS_OS_MUTEX type
 * @param none
 * @return an BPS_OS_MUTEX object
 */
BPS_OS_MUTEX RTOS_BPS_MutexCreate(void) {
    BPS_OS_MUTEX mut;
    return mut;
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
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}