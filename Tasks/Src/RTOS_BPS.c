/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"
#include "os.h"
#include "Tasks.h"

typedef OS_ERR      BPS_OS_ERR;
typedef OS_SEM      BPS_OS_SEM;

typedef OS_TICK     BPS_OS_TICK;
typedef OS_OPT      BPS_OS_OPT;

void RTOS_BPS_SemPend(BPS_OS_SEM   *p_sem,
                      BPS_OS_TICK   timeout,
                      BPS_OS_OPT    opt){ 
    //look at all the functions that implement OSSemPend and see what inputs are actually needed
    // you then make the function prototype so that it implements all the inputs you actually need 
    // and then you can call the function 
    //we are not using the p_ts part - don't need it 
    // need to typedef cahnge the name from OS_SEM to RTOS_SEM 
    BPS_OS_ERR err;
    OSSemPend(&p_sem,timeout,opt,NULL,&err);
    assertOSError(err);
} 
void RTOS_BPS_SemPost(void){

}
void RTOS_BPS_MutexPend(void){

}
void RTOS_BPS_MutexPost(void){

}
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}