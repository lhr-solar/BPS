/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

//need to add the proper #include files 

#ifndef RTOS_BPS_H
#define RTOS_BPS_H

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
void RTOS_BPS_TaskCreate(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_BPS_TimeDelay(void);


#endif 