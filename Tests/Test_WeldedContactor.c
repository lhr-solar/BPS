/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "Contactor.h"
#ifdef SIMULATION
#include "Simulator.h"
#else
#include "stm32f4xx.h"
#endif

// Task that simulates BPS init and triggers fault upon detecting welded semaphore

void Task_InitWeldedContactor(void *p_arg) {
#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#else
    OS_CPU_SysTickInit();
#endif

    OS_ERR err;
	SafetyCheck_Sem4 = xSemaphoreCreateBinary();
                "Safety Check Semaphore",
                0);

WDog_Mutex = xSemaphoreCreateMutex();

    //2
    xTaskCreateStatic(Task_CheckContactor,
		"Task_CheckContactor",
		TASK_CHECK_CONTACTOR_STACK_SIZE,
		(void *)0,
		TASK_CHECK_CONTACTOR_PRIO,
		CheckContactor_Stk,
		&CheckContactor_TCB);
    //10
    xTaskCreateStatic(Task_Idle,
		"TASK_IDLE",
		TASK_IDLE_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_IDLE_PRIO,
		Idle_Stk,
		&Idle_TCB);
    
    CAN_Queue_Init();

    // Simulate volt, amp, temp, and openwire safety checks passing
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);

    //delete task
    OSTaskDel(NULL, &err); // Delete task
}

#ifdef SIMULATION
int main(int argc, char **argv) {
#else
int main() {
#endif
    OS_ERR err;

#ifdef SIMULATION
    // the first command line argument is the path to the JSON file
    Simulator_Init(argv[1]);
#endif

    BSP_PLL_Init();

    //Resetting the contactor
    Contactor_Init();
    Contactor_Off(HVLOW_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);
    Contactor_Off(HVHIGH_CONTACTOR);

    // Simulate welded contactor
    Contactor_On(HVLOW_CONTACTOR);
    Contactor_On(ARRAY_CONTACTOR);
    Contactor_On(HVHIGH_CONTACTOR);

    BSP_Lights_Init();

    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task_InitWeldedContactor,
		"TASK_INIT",
		DEFAULT_STACK_SIZE,
		(void *)0,				        // Task function args,
		TASK_INIT_PRIO,
		Init_Stk,
		&Init_TCB);	        // Stack size
                
    OSStart(&err);

    // Should not get here or else there is an error
}
