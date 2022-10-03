/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files 
#include "os.h" // for RTOS stuff
#include "Tasks.h" // for OS errors

#include "os.h" // for RTOS stuff
#include "Tasks.h" // for OS errors

#ifndef RTOS_BPS_H
#define RTOS_BPS_H


// Currently using Micrium
typedef OS_MUTEX    BPS_OS_MUTEX;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_OPT      BPS_OS_OPT;
typedef OS_ERR      BPS_OS_ERR;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_TCB      BPS_OS_TCB;
typedef OS_SEM_CTR  BPS_OS_SEM_CTR;
typedef OS_TICK     BPS_OS_TICK;


/**
 * @brief Pends a BPS_OS_Semaphore.
 * @param *sem - pointer to a sempaphore to pend
 * @param tick - time in clock ticks to timeout for
 * @param opt - pend option
 * @return the semaphore count, or 0 if not available
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPend(BPS_OS_SEM* sem, BPS_OS_TICK tick, BPS_OS_OPT opt);

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
void RTOS_BPS_TaskCreate(void);

/**
 * @brief Creates a Time Delay.
 * @param Defines how many ticks to delay for.
 * @return none  
 */
void RTOS_BPS_TimeDelay(BPS_OS_TICK dly);

#endif 
