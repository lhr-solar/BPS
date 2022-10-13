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
        OSTimeDly(25, OS_OPT_TIME_DLY, &err);
        // Comment out the following lines to test watchdog timeout
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
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
                Task2_Stk);

    RTOS_BPS_TaskCreate(&PetWDog_TCB,				// TCB
				"TASK_PETWDOG_PRIO",	// Task Name (String)
				Task_PetWDog,				// Task function pointer
				(void *)0,				// Task function args
				TASK_PETWDOG_PRIO,			// Priority
				PetWDog_Stk);					// return err code

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

    