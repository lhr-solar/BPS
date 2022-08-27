/* Copyright (c) 2022 UT Longhorn Racing Solar */

//need to add the proper #include files 

#ifndef Task_RTOS_BPS_H
#define Task_RTOS_BPS_H

//#include


/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_SemPend(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_SemPost(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_MutexPend(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_MutexPost(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_TaskCreate(void);

/**
 * @brief   
 * @param   
 * @return  
 */
void RTOS_TimeDelay(void);


#endif 