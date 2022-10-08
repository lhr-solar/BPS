/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"
#include "BSP_WDTimer.h"
#include "BSP_Contactor.h"
#include "BSP_UART.h"
#include "Amps.h"
#include <stdio.h>

/******************************************************************************
 * Amperes Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Set up the BPS leader, amperes and shunt resistor boards. Also connect the contactor
 * 3. Flash the BPS (see README.md)
 * 4. Connect a logic analyzer to the BPS's CAN port
 * 5. Start recording the BPS's CAN output
 * 6. Appy a known current to the BPS's shunt resistor (between -20 and 75 Amps)
 * 7. Reset the BPS by pressing and releasing the reset button
 * 8. Check the logic analyzer recording for the BPS's CAN messages and verfiy
 *    that the observed CAN messages contain accurate current readings (within the range 
 *    allowed by the Amperes App test)
 * 9. Apply an overcurrent to the BPS (less than -20 Amps or greater than 75 Amps) and
 *    verify that the contactor opens (the contactor should have closed earlier on startup)
 *****************************************************************************/

void EnterFaultState(void);

// Used by Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

// Initialization task for this test
void Task1(void *p_arg){
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    OS_ERR err;
    
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

    OSMutexCreate(&WDog_Mutex,
                "Watchdog Mutex",
                &err);
    assertOSError(err);

    // Spawn tasks needed for Amperes readings to affect contactor
    //1
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
    //2
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

    // Spawn Task_AmperesMonitor with PRIO 5
    OSTaskCreate(&AmperesMonitor_TCB,				// TCB
				"TASK_AMPERES_MONITOR_PRIO",	// Task Name (String)
				Task_AmperesMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_AMPERES_MONITOR_PRIO,			// Priority
				AmperesMonitor_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_AMPERES_MONITOR_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

    // Spawn CANBUS Consumer, PRIO 7
    OSTaskCreate(&CANBusConsumer_TCB,				// TCB
            "TASK_CANBUS_CONSUMER_PRIO",	// Task Name (String)
            Task_CANBusConsumer,				// Task function pointer
            (void *)true,				// Use loopback mode
            TASK_CANBUS_CONSUMER_PRIO,			// Priority
            CANBusConsumer_Stk,				// Stack
            WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
            TASK_CANBUS_CONSUMER_STACK_SIZE,		// Stack size
            0,						// Queue size (not needed)
            10,						// Time quanta (time slice) 10 ticks
            (void *)0,				// Extension pointer (not needed)
            OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
            &err);					// return err code
        
    // Initialize CAN queue
    CAN_Queue_Init();
    assertOSError(err);

	//delete task
	OSTaskDel(NULL, &err); // Delete task
}

//Task to prevent watchdog from tripping
void Task2(void *p_arg){
    OS_ERR err;

    int count = 0;

    // get contactor to close without temperature, voltage, or open wire readings
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

    while(1){
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);
        WDog_BitMap |= WD_VOLT_TEMP;
        WDog_BitMap |= WD_BALANCING;
        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);
        //delay of 100ms
        RTOS_BPS_DelayTick(10);
        assertOSError(err);

        if (count == 0) {
            printf("Amps: %ld\n\r", Amps_GetReading());
        }
        count = (count + 1) % 10;

        BSP_Light_Toggle(RUN);
    }
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
int main(void) {
    OS_ERR err;


    BSP_PLL_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    //Resetting the contactor
    BSP_Contactor_Init();
    BSP_Contactor_Off();

    // If the WDTimer counts down to 0, then the BPS resets. If BPS has reset, enter a fault state.
    if (BSP_WDTimer_DidSystemReset()) {
	    EnterFaultState();
    }

    OSInit(&err);
    assertOSError(err);

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
    assertOSError(err);

    //Give same priority as volt temp task thread
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

    OSStart(&err);
}
