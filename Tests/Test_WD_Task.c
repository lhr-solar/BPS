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

StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[256];
StaticTask_t Task2_TCB;
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
    
    xTaskCreateStatic(Task2,
		"Task 2",
		256,
		(void *)0,,
		10,
		Task2_Stk,
		&Task2_TCB);

    xTaskCreateStatic(Task_PetWDog,
		"TASK_PETWDOG_PRIO",
		TASK_PETWDOG_STACK_SIZE,
		(void *)0,
		TASK_PETWDOG_PRIO,
		PetWDog_Stk,
		&PetWDog_TCB);					// return err code

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

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);
    while(err != OS_ERR_NONE);

    __enable_irq();

    OSStart(&err);
}
