/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"
#include <stdint.h>

/**
 * @brief Pends a BPS_OS_Semaphore.
 * @param *sem - pointer to a sempaphore to pend
 * @param tick - time in clock ticks to timeout for
 * @param opt - pend option
 * @return the semaphore count, or 0 if not available
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPend(BPS_OS_SEM* sem, BPS_OS_TICK tick, BPS_OS_OPT opt) {
    BPS_OS_ERR err;
    BPS_OS_SEM_CTR count = OSSemPend(sem, tick, opt, 0, &err); // we don't need timestamp
    assertOSError(err);
    return count;
}

void RTOS_BPS_SemPost(void) {

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

/**
 * @brief: Creates a Second-Based Time Delay.
 * @param dly Defines how many seconds to delay for.
 * @return none
 */
void RTOS_BPS_DelaySecs(uint16_t dly){
    BPS_OS_ERR err;
    OSTimeDlyHMSM(0, 0, dly, 0, OS_OPT_TIME_HMSM_NON_STRICT, &err);
    assertOSError(err);
}

/**
 * @brief: Creates a Millisecond-Based Time Delay.
 * @param dly Defines how many milliseconds to delay for.
 * @return none
 */
void RTOS_BPS_DelayMs(uint16_t dly){
    BPS_OS_ERR err;
    OSTimeDlyHMSM(0, 0, 0, dly, OS_OPT_TIME_HMSM_NON_STRICT, &err);
    assertOSError(err);
}

/**
 * @brief: Creates a Tick-Based Time Delay.
 * @param dly Defines how many ticks to delay for.
 * @return none
 */
void RTOS_BPS_DelayTick(BPS_OS_TICK dly){
    BPS_OS_ERR err;
    OSTimeDly(dly, OS_OPT_TIME_DLY, &err);
    assertOSError(err);
}
