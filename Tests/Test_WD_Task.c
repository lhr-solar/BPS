/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "BSP_WDTimer.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];
OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

void EnterFaultState();

void Task2(void *p_arg){
    OS_ERR err;
    
    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(EXTRA);
        RTOS_BPS_DelayTick(25);
        // Comment out the following lines to test watchdog timeout
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);
        WDog_BitMap = 7;
        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
    }

    exit(0);
}

void Task1(void *p_arg){
    OS_ERR err;

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    RTOS_BPS_MutexCreate(&WDog_Mutex, "Watchdog Mutex");
    
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                10,
                Task2_Stk,
                256);

    RTOS_BPS_TaskCreate(&PetWDog_TCB,				// TCB
				"TASK_PETWDOG_PRIO",	// Task Name (String)
				Task_PetWDog,				// Task function pointer
				(void *)0,				// Task function args
				TASK_PETWDOG_PRIO,			// Priority
				PetWDog_Stk,	// Watermark limit for debugging
				TASK_PETWDOG_STACK_SIZE);					// return err code

    OSTaskDel(NULL, &err);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();
    //BSP_WDTimer_Init();

    if (BSP_WDTimer_DidSystemReset()) {
		EnterFaultState();
	}

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
