/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"
#include "Tasks.h"
void RTOS_BPS_SemPend(void){ 

} 
void RTOS_BPS_SemPost(void){

}
void RTOS_BPS_MutexPend(void){

}
void RTOS_BPS_MutexPost(void){

}
void RTOS_BPS_TaskCreate(
    OS_TCB        *p_tcb,
    CPU_CHAR      *p_name,
    OS_TASK_PTR    p_task,
    void          *p_arg,
    OS_PRIO        prio,
    CPU_STK       *p_stk_base,
    CPU_STK_SIZE   stk_size,
    OS_ERR        *p_err)
    {
    OSTaskCreate(p_tcb, p_name, p_task, p_arg, prio, p_stk_base, WATERMARK_STACK_LIMIT, stk_size, 0, 10,(void *)0, OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP, p_err);
    assertOSError(*p_err);
    }

void RTOS_BPS_TimeDelay(void){

}
