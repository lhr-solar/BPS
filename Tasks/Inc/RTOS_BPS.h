/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files 
#include "os.h" // for RTOS stuff
#include "Tasks.h" // for OS errors
#include <stdlib.h> 

#ifndef RTOS_BPS_H
#define RTOS_BPS_H

// Custom typedefs and options for mutex/semaphore functionality
// ONLY change the first column in the TypeDef depending on the RTOS used

// Currently using Micrium
typedef OS_MUTEX    BPS_OS_MUTEX;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_OPT      BPS_OS_OPT;
typedef OS_ERR      BPS_OS_ERR;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_TCB      BPS_OS_TCB;
typedef OS_SEM_CTR  BPS_OS_SEM_CTR;
typedef OS_TICK     BPS_OS_TICK;

//Custom typedefs for BPS_TaskCreate function currently using micirium
typedef OS_TASK_PTR  BPS_OS_TASK_PTR;  //pointer to the task
typedef char         BPS_OS_TASK_NAME; //name of the task
typedef OS_PRIO      BPS_OS_TASK_PRIO; //priority for the task
typedef CPU_STK_SIZE BPS_OS_TASK_STK_SIZE;  //size of the stack in WORDS not bytes


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

/**
 * @brief Initializes a mutex object.
 * @param *mut - pointer to a mutex to initialize
 * @param name - char* of the name of the mutex
 * @return none
 */
void RTOS_BPS_MutexCreate(BPS_OS_MUTEX *mut, char* name);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_MutexPend(void);

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
    OS_TCB              *p_tcb,
    BPS_OS_TASK_NAME    *p_name,
    BPS_OS_TASK_PTR      p_task,
    void                *p_arg,
    BPS_OS_TASK_PRIO     prio,
    CPU_STK             *p_stk_base,
    BPS_OS_TASK_STK_SIZE stk_size
);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_TimeDelay(void);


#endif 