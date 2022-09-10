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
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_MutexPend(void);

/**
 * @brief   Posts the specified Mutex
 * @param   *mutex - pointer to the specified RTOS Mutex object
 * @param   options - a parameter which determines what kind of Post MutexPost performs
 * @return  BPS_OS_ERR value
 */
BPS_OS_ERR RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_TaskCreate(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_TimeDelay(void);


#endif 