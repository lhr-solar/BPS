/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "Contactor.h"

StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[256];
StaticTask_t Task2_TCB;
CPU_STK Task2_Stk[256];

void Task2(void *p_arg){
    RTOS_BPS_DelayTick(250);

    Fault_BitMap = Fault_OW;   // set this to whatever you want
    EnterFaultState();
    
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

    xTaskCreateStatic(Task2,
		"Task 2",
		256,
		(void *)0,,
		10,
		Task2_Stk,
		&Task2_TCB);

    OSTaskDel(NULL, &err);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();
    Contactor_Init();
    Contactor_Off(HVLOW_CONTACTOR);
    Contactor_Off(HVHIGH_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);

    __disable_irq();

    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);

    __enable_irq();

    OSStart(&err);
}
