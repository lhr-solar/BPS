/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files 
#include "os.h" // for RTOS stuff
#include "Tasks.h" // for OS errors

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


/**
 * @brief
 * @param
 * @return
 */
void RTOS_BPS_SemPend(void);

/**
 * @brief
 * @param
 * @return  
 */
void RTOS_BPS_SemPost(void);

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
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_TaskCreate(
    OS_TCB        *p_tcb,
    CPU_CHAR      *p_name,
    OS_TASK_PTR    p_task,
    void          *p_arg,
    OS_PRIO        prio,
    CPU_STK       *p_stk_base,
    CPU_STK_SIZE   stk_size,
    OS_ERR        *p_err);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_TimeDelay(void);


#endif 