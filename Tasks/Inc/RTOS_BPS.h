/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files

#include "os.h" // for RTOS stuff
#include "Tasks.h" // for OS errors
#include <stdint.h> 

#ifndef RTOS_BPS_H
#define RTOS_BPS_H


// Currently using Micrium
typedef OS_MUTEX    BPS_OS_MUTEX;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_OPT      BPS_OS_OPT;
typedef OS_ERR      BPS_OS_ERR;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_TCB      BPS_OS_TCB;
typedef CPU_TS      BPS_CPU_TS;
typedef OS_SEM_CTR  BPS_OS_SEM_CTR;
typedef OS_TICK     BPS_OS_TICK;

//Custom typedefs for BPS_TaskCreate function currently using micirium
typedef OS_TCB       BPS_OS_TCB;
typedef CPU_STK      BPS_CPU_STK;

/**
 * @brief Pends a BPS_OS_Semaphore.
 * @param *sem - pointer to a sempaphore to pend
 * @param tick - time in clock ticks to timeout for
 * @param opt - pend option
 * @return the semaphore count, or 0 if not available
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPend(BPS_OS_SEM* sem, BPS_OS_TICK tick, BPS_OS_OPT opt);

/**
 * @brief Posts a semaphore
 * @param sem4 this is a semaphore pointer to post to
 * @param opt determines the type of POST performed
 * @return The current value of the semaphore counter or 0 upon error 
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPost(BPS_OS_SEM *sem4, BPS_OS_OPT opt);

/*
 * @brief Initializes a mutex object.
 * @param *mut - pointer to a mutex to initialize
 * @param name - char* of the name of the mutex
 * @return none
 */
void RTOS_BPS_MutexCreate(BPS_OS_MUTEX *mut, char* name);

/**
 * @brief   Function waits for Mutex
 * @param   *mutex - pointer to mutex
 * @param   options - determines what the mutex will do, ie: block or not block
 */
void RTOS_BPS_MutexPend(BPS_OS_MUTEX* mutex, BPS_OS_OPT opt);

/**
 * @brief   Posts the specified Mutex
 * @param   *mutex - pointer to the specified RTOS Mutex object
 * @param   options - a parameter which determines what kind of Post MutexPost performs
 * @return  none
 */
void RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options);

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
    char         *p_name,
    void         *p_task,
    void         *p_arg,
    uint8_t       prio,
    BPS_CPU_STK  *p_stk_base,
    uint64_t      stk_size
);

/**
 * @brief Creates a Second-based Time Delay.
 * @param dly Defines how many seconds to delay for.
 * @return none 
 */
void RTOS_BPS_DelaySecs(uint16_t dly);

/**
 * @brief Creates a Millisecond-based Time Delay.
 * @param dly Defines how many milliseconds to delay for.
 * @return none 
 */
void RTOS_BPS_DelayMs(uint16_t dly);

/**
 * @brief Creates a Tick-based Time Delay.
 * @param dly Defines how many ticks to delay for.
 * @return none 
 */
void RTOS_BPS_DelayTick(BPS_OS_TICK dly);

#endif 
