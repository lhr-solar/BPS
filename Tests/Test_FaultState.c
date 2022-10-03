/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "BSP_Contactor.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];
OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

void Task2(void *p_arg){
    OS_ERR err;

    RTOS_BPS_TimeDelay(250);

    Fault_BitMap = Fault_OW;   // set this to whatever you want
    OSSemPost(&Fault_Sem4,
                OS_OPT_POST_1,
                &err);
    assertOSError(err);
    
    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(EXTRA);
        RTOS_BPS_TimeDelay(25);
    }

    exit(0);
}

void Task1(void *p_arg){
    OS_ERR err;

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);
    assertOSError(err);

    OSTaskCreate(&FaultState_TCB,				// TCB
				"TASK_FAULT_STATE_PRIO",	// Task Name (String)
				Task_FaultState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_FAULT_STATE_PRIO,			// Priority
				FaultState_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_FAULT_STATE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
    
    OSTaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                10,
                Task2_Stk,
                16,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);

    OSTaskDel(NULL, &err);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();
    BSP_Contactor_Init();
    BSP_Contactor_On();

    __disable_irq();

    OSInit(&err);
    while(err != OS_ERR_NONE);

    OSTaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                16,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);
    while(err != OS_ERR_NONE);

    __enable_irq();

    OSStart(&err);
}
