/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files 

#ifndef RTOS_BPS_H
#define RTOS_BPS_H

//#include


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
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_MutexPost(void);

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