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

    RTOS_BPS_DelayTick(250);

    Fault_BitMap = Fault_OW;   // set this to whatever you want
    OSSemPost(&Fault_Sem4,
                OS_OPT_POST_1,
                &err);
    assertOSError(err);
    
    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(EXTRA);
        RTOS_BPS_DelayTick(25);
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

    RTOS_BPS_TaskCreate(&FaultState_TCB,				// TCB
				"TASK_FAULT_STATE_PRIO",	// Task Name (String)
				Task_FaultState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_FAULT_STATE_PRIO,			// Priority
				FaultState_Stk,	// Watermark limit for debugging
				TASK_FAULT_STATE_STACK_SIZE);					// return err code
    
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                10,
                Task2_Stk,
                256);

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

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);
    while(err != OS_ERR_NONE);

    __enable_irq();

    OSStart(&err);
}
