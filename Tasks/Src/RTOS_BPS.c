/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"

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

/**
 * @brief Posts a semaphore
 * @param sem4 this is a semaphore pointer to post to
 * @param opt determines the type of POST performed
 * @return The current value of the semaphore counter or 0 upon error 
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPost(BPS_OS_SEM *sem4, BPS_OS_OPT opt) {
    BPS_OS_ERR err;
    BPS_OS_SEM_CTR counter = OSSemPost(sem4, opt, &err);
    assertOSError(err);
    return counter;
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

/**
 * @brief   Creates a task that will be handled by the OS
 * @param   *p_tcb - pointer to the tcb
 * @param   *p_name - pointer to task name
 * @param   p_task - pointer to the task
 * @param   *p_args - pointer to task function arguments
 * @param   *prio - task priority
 * @param   *p_stk_base - the stack
 * @param   stk_size - size of the stack
 * @param   *p_err - return error code
 * @return  nothing to see here
 */
void RTOS_BPS_TaskCreate(
    BPS_OS_TCB      *p_tcb,
    char            *p_name,
    void            *p_task,
    void            *p_arg,
    uint8_t          prio,
    BPS_CPU_STK     *p_stk_base,
    uint64_t         stk_size
    )
    {
        BPS_OS_ERR err;
        OSTaskCreate(p_tcb, p_name, p_task, p_arg, prio, p_stk_base, stk_size, stk_size, 0, 10,(void *)0, OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP, &err);
        assertOSError(err);
    }

/*
void RTOS_BPS_TaskCreate(
    BPS_OS_TASK_PTR taskptr, 
    BPS_OS_TASK_NAME *taskname, 
    void *args, 
    BPS_OS_TASK_PRIO priority, 
    BPS_OS_TASK_STK_SIZE stack_size
    )
{
    see if I can implement this one later
}
*/

void RTOS_BPS_TimeDelay(void){

}
