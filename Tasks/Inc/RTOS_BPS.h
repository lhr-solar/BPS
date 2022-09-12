/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files 
#include "os.h" // for RTOS stuff
#include "Tasks.h" // for OS errors

#ifndef RTOS_BPS_H
#define RTOS_BPS_H

// Custom typedefs and options for RTOS functionality
// ONLY change the first column in the TypeDef depending on the RTOS used

// Currently using Micrium
typedef OS_MUTEX    BPS_OS_MUTEX;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_OPT      BPS_OS_OPT;
typedef OS_ERR      BPS_OS_ERR;
typedef OS_SEM      BPS_OS_SEM;
typedef OS_TCB      BPS_OS_TCB;
typedef OS_Q        BPS_OS_QUEUE;
typedef OS_MSG_SIZE BPS_OS_MSG_SIZE;


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
 * @return  none
 */
void RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options);

/**
 * Note that the incoming BPS_OS_QUEUE queue !!MUST!! be created already.
 * @brief   Puts a message into the RTOS Queue (i.e., POST something into the Queue). https://docs.silabs.com/micrium/latest/micrium-kernel-api/03-kernel-message-queue-api
 * @param   *queue - pointer to the Queue to write a message to
 * @param   *msg - void pointer to the message you are going to send
 * @param   size - payload size of *msg (this changes in FreeRTOS)
 * @return  none
 */
void RTOS_BPS_QueuePut(BPS_OS_QUEUE* queue, void* msg, BPS_OS_MSG_SIZE size);

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