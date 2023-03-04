/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "Tasks.h"
#ifdef SIMULATION
#include "Simulator.h"
#else
#include "stm32f4xx.h"
#endif
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"
#include "BSP_WDTimer.h"
#include "Contactor.h"
#include "BSP_UART.h"
#include "Amps.h"
#include "RTOS_BPS.h"
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

// Used by Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

// Initialization task for this test
void Task1(void *p_arg){
#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#else
    OS_CPU_SysTickInit();
#endif

    OS_ERR err;

    RTOS_BPS_SemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0);

    RTOS_BPS_MutexCreate(&WDog_Mutex, "Watchdog Mutex");

    // Spawn tasks needed for Amperes readings to affect contactor
    //1
    RTOS_BPS_TaskCreate(&CheckContactor_TCB,    // TCB
				"Task_CheckContactor",          // Task Name (String)
				Task_CheckContactor,            // Task function pointer
				(void *)0,                      // Task function args
				TASK_CHECK_CONTACTOR_PRIO,      // Priority
				CheckContactor_Stk,             // Stack
				TASK_CHECK_CONTACTOR_STACK_SIZE);

    // Spawn Task_AmperesMonitor with PRIO 5
    RTOS_BPS_TaskCreate(&AmperesMonitor_TCB,				// TCB
				"TASK_AMPERES_MONITOR_PRIO",	// Task Name (String)
				Task_AmperesMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_AMPERES_MONITOR_PRIO,			// Priority
				AmperesMonitor_Stk,	// Watermark limit for debugging
				TASK_AMPERES_MONITOR_STACK_SIZE);					// return err code

    // Spawn CANBUS Consumer, PRIO 7
    RTOS_BPS_TaskCreate(&CANBusConsumer_TCB,				// TCB
            "TASK_CANBUS_CONSUMER_PRIO",	// Task Name (String)
            Task_CANBusConsumer,				// Task function pointer
            (void *)true,				// Use loopback mode
            TASK_CANBUS_CONSUMER_PRIO,			// Priority
            CANBusConsumer_Stk,	// Watermark limit for debugging
            TASK_CANBUS_CONSUMER_STACK_SIZE);					// return err code
        
    // Initialize CAN queue
    CAN_Queue_Init();

	//delete task
	OSTaskDel(NULL, &err); // Delete task
}

//Task to prevent watchdog from tripping
void Task2(void *p_arg){
    int count = 0;

    // get contactor to close without temperature, voltage, or open wire readings
    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);

    while(1){
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);
        WDog_BitMap |= WD_VOLT_TEMP;
        WDog_BitMap |= WD_BALANCING;
        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE);
        //delay of 100ms
        RTOS_BPS_DelayTick(10);

        if (count == 0) {
            printf("Amps: %ld\n\r", (int64_t)Amps_GetReading());
        }
        count = (count + 1) % 10;

        BSP_Light_Toggle(RUN);
    }
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
#ifdef SIMULATION
int main(int argc, char **argv) {
#else
int main() {
#endif

#ifdef SIMULATION
    // the first command line argument is the path to the JSON file
    Simulator_Init(argv[1]);
#endif

    OS_ERR err;

    BSP_PLL_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    //Resetting the contactor
    Contactor_Init();
    Contactor_Off(HVLOW_CONTACTOR);
    Contactor_Off(HVHIGH_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);

    // If the WDTimer counts down to 0, then the BPS resets. If BPS has reset, enter a fault state.
    if (BSP_WDTimer_DidSystemReset()) {
	    EnterFaultState();
    }

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);

    //Give same priority as volt temp task thread
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                4,
                Task2_Stk,
                256);

    OSStart(&err);
}
