/* Copyright (c) 2022 UT Longhorn Racing Solar 
RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"

void RTOS_BPS_SemPend(void) {

} 
void RTOS_BPS_SemPost(void) {

}
void RTOS_BPS_MutexPend(void){

}
BPS_OS_ERR RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options) {
    // For future reference, Post is the same as Give
    BPS_OS_ERR err;
    OSMutexPost(mutex, options, &err); // calls MutexPost with all specified params
    assertOSError(err);
    return err; // returns OS error, even though we don't *really* use it right now.
}
void RTOS_BPS_TaskCreate(void){

}
void RTOS_BPS_TimeDelay(void){

}