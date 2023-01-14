/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Contactor.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"

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
 * 4. Make sure the contactor goes from open to closed. Use an ohmmeter to check the resistance across the contactor's terminals to determine the state of the contactor (this resistance should be close to 0 Ohms).
 * 5. Unpower the contactor, while keeping the BPS powered on. Make sure the BPS goes into a fault state
****************************************************************************/

void Task2(void *p_arg){    //This task is meant to allow contactor to close
    OS_ERR err;

    OSTimeDly(250, OS_OPT_TIME_DLY, &err);


    OSSemPost(&SafetyCheck_Sem4,
                OS_OPT_POST_1,
                &err);
    assertOSError(err);
    
    OSSemPost(&SafetyCheck_Sem4,
                OS_OPT_POST_1,
                &err);
    assertOSError(err);

    OSSemPost(&SafetyCheck_Sem4,
                OS_OPT_POST_1,
                &err);
    assertOSError(err);

    OSSemPost(&SafetyCheck_Sem4,
                OS_OPT_POST_1,
                &err);
    assertOSError(err);

    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(RUN);
        OSTimeDly(25, OS_OPT_TIME_DLY, &err);
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

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
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
		assertOSError(err);

    OSTaskCreate(&CriticalState_TCB,				// TCB
				"TASK_CRITICAL_STATE_PRIO",	// Task Name (String)
				Task_CriticalState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CRITICAL_STATE_PRIO,			// Priority
				CriticalState_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_CRITICAL_STATE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);

    
    OSTaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                4,
                Task2_Stk,
                16,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);
		assertOSError(err);

        OSTaskCreate(&CANBusConsumer_TCB,				// TCB
				"TASK_CANBUS_CONSUMER_PRIO",	// Task Name (String)
				Task_CANBusConsumer,				// Task function pointer
				(void *)false,				// don't use loopback mode
				TASK_CANBUS_CONSUMER_PRIO,			// Priority
				CANBusConsumer_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_CANBUS_CONSUMER_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);

    CAN_Queue_Init();

    OSTaskDel(NULL, &err);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();
    BSP_Contactor_Init();

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
