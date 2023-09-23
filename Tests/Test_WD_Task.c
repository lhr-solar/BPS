/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "BSP_WDTimer.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];
OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

void Task2(void *p_arg){    
    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(EXTRA);
        RTOS_BPS_DelayTick(25);
        // Comment out the following lines to test watchdog timeout
	xSemaphoreTake(WDog_Mutex, (TickType_t)portMAX_DELAY); 
        WDog_BitMap = 7;
	xSemaphoreGive(WDog_Mutex);
    }

    exit(0);
}

void Task1(void *p_arg){
    OS_ERR err;

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

WDog_Mutex = xSemaphoreCreateMutex();
    
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
    //BSP_WDTimer_Init(false);

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
