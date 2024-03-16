/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/*RTOS_BPS library includes all the wrapper functions for a functioning RTOS - needed for cleaner code and portability
*/

#include "RTOS_BPS.h"
#include "os_cfg_app.h"
#include "Tasks.h"      // for OS errors
#include "BSP_Timer.h"  // for fine-grained RTOS delays

// Helper structures for RTOS_BPS_DelayUs() -------------------

static volatile uint32_t DelayUsContinue = 0;

static void DelayUsCallback(void) {
    CPU_SR_ALLOC();
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    DelayUsContinue = 1;

    OSIntExit();
}

// End Helper structures for RTOS_BPS_DelayUs() ---------------

/**
 * @brief   Creates a task that will be handled by the OS
 * @param   *p_tcb - pointer to the tcb
 * @param   *p_name - pointer to task name
 * @param   *p_task - pointer to the task
 * @param   *p_args - pointer to task function arguments
 * @param   prio - task priority
 * @param   *p_stk_base - the stack
 * @param   stk_size - size of the stack
 * @param   *p_err - return error code
 * @return  nothing to see here
 */
void RTOS_BPS_TaskCreate(BPS_OS_TCB *p_tcb, char *p_name, void *p_task, void *p_arg, uint8_t prio, BPS_CPU_STK *p_stk_base, uint64_t stk_size) {
    BPS_OS_ERR err;
    OSTaskCreate(p_tcb, p_name, p_task, p_arg, prio, p_stk_base, stk_size, stk_size, 0, 10,(void *)0, OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP, &err);
    assertOSError(err);
}

/**
 * @brief Creates a semaphore with the initially specified count
 * 
 * @param sem - pointer to a semaphore object to create and initialize
 * @param name - name of the semaphore
 * @param count - initial count for the semaphore
 */
void RTOS_BPS_SemCreate(BPS_OS_SEM* sem, char* name, uint32_t count) {
    BPS_OS_ERR err;
    OSSemCreate(sem, name, count, &err);
    assertOSError(err);
}

/**
 * @brief Pends a BPS_OS_Semaphore.
 * @param sem - pointer to a sempaphore to pend
 * @param opt - pend option
 * @return the semaphore count, or -1 if not available (will be 0xFFFFFFFF as it is unsigned)
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPend(BPS_OS_SEM* sem, BPS_OS_OPT opt) {
    BPS_OS_ERR err;
    BPS_OS_SEM_CTR count = OSSemPend(sem, 0, opt, 0, &err); // we don't need timestamp
    if (err == OS_ERR_PEND_WOULD_BLOCK) {
        return BPS_OS_SEM_WOULD_BLOCK;
    }
    else {
        assertOSError(err);
        return count;
    }
}

/**
 * @brief Posts a semaphore
 * @param sem4 this is a semaphore pointer to post to
 * @param opt determines the type of POST performed
 * @return The current value of the semaphore counter or 0 upon error 
 */
BPS_OS_SEM_CTR RTOS_BPS_SemPost(BPS_OS_SEM *sem4, BPS_OS_OPT opt) {
    BPS_OS_ERR err;
    BPS_OS_SEM_CTR counter = OSSemPost(sem4, opt, &err);
    assertOSError(err);
    return counter;
}

/**
 * @brief Initializes a mutex object.
 * @param *mut - pointer to a mutex to initialize
 * @param name - char* of the name of the mutex
 * @return none
 */
void RTOS_BPS_MutexCreate(BPS_OS_MUTEX *mut, char* name) {
    BPS_OS_ERR err;
    OSMutexCreate(mut, name, &err);
    assertOSError(err);
}

/**
 * @brief   Waits for Mutex, assigns timestamp and any error to err and ticks
 * @param   *mutex - pointer to mutex
 * @param   options - determines what the mutex will do, ie: block or not block
 * @return  none
 */
void RTOS_BPS_MutexPend(BPS_OS_MUTEX* mutex, BPS_OS_OPT opt) {
    BPS_OS_ERR err;
    OSMutexPend(mutex, 0, opt, (void*) 0, &err); 
    assertOSError(err);
}

/**
 * @brief   Posts the specified Mutex. (For future reference, Post is the same as Give)
 * @param   *mutex - pointer to the specified RTOS Mutex object
 * @param   options - a parameter which determines what kind of Post MutexPost performs
 * @return  none
 */
void RTOS_BPS_MutexPost(BPS_OS_MUTEX* mutex, BPS_OS_OPT options) {
    BPS_OS_ERR err;
    OSMutexPost(mutex, options, &err);
    assertOSError(err);
}

/**
 * @brief: Creates a Second-Based Time Delay.
 * @param delay_s Defines how many seconds to delay for.
 * @return none
 */
void RTOS_BPS_DelaySecs(uint16_t delay_s){
    BPS_OS_ERR err;
    OSTimeDlyHMSM(0, 0, (CPU_INT16U)delay_s, 0, OS_OPT_TIME_HMSM_NON_STRICT, &err);
    assertOSError(err);
}

/**
 * @brief: Creates a Millisecond-Based Time Delay.
 * @param dly Defines how many milliseconds to delay for.
 * @note if a dly value is passed that is less time than the resolution of 1 tick (100ms), 
 *       this code will error out
 * @return none
 */
void RTOS_BPS_DelayMs(uint16_t delay_ms){
    BPS_OS_ERR err;
    OSTimeDlyHMSM(0, 0, 0, (CPU_INT32U)delay_ms, OS_OPT_TIME_HMSM_NON_STRICT, &err);
    assertOSError(err);
}

/**
 * @brief Creates an accurate Microsecond-based Time Delay.
 *        As anything less than 100ms will not work with an RTOS delay, 
 *        this delay suspends the scheduler and uses a hardware timer to implement a 
 *        microsecond-accurate delay.
 * @param dly Defines how many milliseconds to delay for.
 * @note !! Blocks the Scheduler !! Do not use for extended delays!
 *       Use RTOS_BPS_DelayMs() or RTOS_BPS_DelaySecs() if possible.
 *       Delays longer than 1 tick (10ms) cause undefined behavior.
 * 
 *       Due to overhead from suspending the scheduler and setting up the delay, the 
 *       actual delay time will be a few cycles slower than the requested delay time.
 * @return none 
 */
void RTOS_BPS_DelayUs(uint32_t delay_us) {
    BPS_OS_ERR err;
    // only errors possible are OS_ERR_NONE and OS_ERR_LOCK_NESTING_OVF
    // OS_ERR_LOCK_NESTING_OVF occurs when scheduler is already locked
    // we can ignore this error (OSSchedLock then does nothing)
    
    // lock the scheduler
    OSSchedLock(&err);

    // delay
    BSP_Timer_Start_OneShot(delay_us, DelayUsCallback);
    while (!DelayUsContinue);
    DelayUsContinue = 0;

    // unlock the scheduler
    if (err == OS_ERR_NONE) {
        OSSchedUnlock(&err);
    }
}

/**
 * @brief: Creates a Tick-Based Time Delay.
 * @param delay_ticks Defines how many ticks to delay for.
 * @return none
 */
void RTOS_BPS_DelayTick(BPS_OS_TICK delay_ticks){
    BPS_OS_ERR err;
    OSTimeDly(delay_ticks, OS_OPT_TIME_DLY, &err);
    assertOSError(err);
}

