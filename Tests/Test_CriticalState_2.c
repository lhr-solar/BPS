/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Contactor.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];
OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

/******************************************************************************
 * Critical State Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Flash the BPS (see README.md)
 * 3. Run the code in this file. NOTE: This code includes a heartbeat.
 * 4. Make sure the contactor stays open. Use an ohmmeter to check the resistance across the contactor's terminals to determine the state of the contactor (this resistance should be very high/open circuit).
****************************************************************************/


void Task2(void *p_arg){    //This task is meant to cause contactor to remain open
    RTOS_BPS_DelayTick(250);

    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);

    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(EXTRA);
        RTOS_BPS_DelayTick(25);   // heartbeat
    }

    exit(0);
}

void Task1(void *p_arg){
    OS_ERR err;

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    RTOS_BPS_SemCreate(&SafetyCheck_Sem4, "Safety Check Semaphore", 0);

    RTOS_BPS_TaskCreate(&CriticalState_TCB,				// TCB
				"TASK_CRITICAL_STATE_PRIO",	// Task Name (String)
				Task_CriticalState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CRITICAL_STATE_PRIO,			// Priority
				CriticalState_Stk,	// Watermark limit for debugging
				TASK_CRITICAL_STATE_STACK_SIZE);					// return err code
    
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                10,
                Task2_Stk,
                256);

    OSTaskDel(NULL, &err);
    assertOSError(err);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();
    BSP_Contactor_Init();

    __disable_irq();

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);
    assertOSError(err);

    __enable_irq();

    OSStart(&err);
    assertOSError(err);
}
