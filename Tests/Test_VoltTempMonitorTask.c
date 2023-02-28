/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "os.h"
#include "Tasks.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#else
#include "Simulator.h"
#endif
#include "BSP_Lights.h"
#include "CAN_Queue.h"
#include "BSP_PLL.h"
#include "BSP_WDTimer.h"
#include "Contactor.h"
#include "RTOS_BPS.h"
#include "Voltage.h"
#include "Temperature.h"
#include "BSP_UART.h"

/******************************************************************************
 * VoltTempMonitor Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 *  --Remember to change Config file parameters NUM_BATTERY_MODULES, NUM_TEMPERATURE_SENSORS, & MAX_VOLT_SENSORS_PER_MINION_BOARD
 * 2. Set up the BPS leader, Minion boards, and Contactor.
 * 3. Flash the BPS (see README.md)
 * 4. Connect a logic analyzer to the BPS's CAN port
 * 5. Start recording the BPS's CAN output
 * 6. Provide UVOLT, SAFE, OVOLT, OWIRE, and OTEMP cases before starting task 
 * and see if task fails for all cases (hear contactor open, see lights turn on).
 * 7. Allow task to run and then check for all fault cases again.
 * 8. Check the logic analyzer recording for the BPS's CAN messages and verfiy
 *    that the observed CAN messages contain accurate readings.
 *****************************************************************************/

// Used by Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

OS_ERR p_err;

// Initialization task for this test
void Task1(void *p_arg){
    
#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#endif

    RTOS_BPS_SemCreate(&SafetyCheck_Sem4, "Safety Check Semaphore", 0);

    RTOS_BPS_MutexCreate(&WDog_Mutex, "Watchdog Mutex");

    // Spawn tasks needed for Amperes readings to affect contactor
    //1
    RTOS_BPS_TaskCreate(&CriticalState_TCB,				// TCB
            "TASK_CRITICAL_STATE_PRIO",	// Task Name (String)
            Task_CriticalState,				// Task function pointer
            (void *)0,				// Task function args
            TASK_CRITICAL_STATE_PRIO,			// Priority
            CriticalState_Stk,	// Watermark limit for debugging
            TASK_CRITICAL_STATE_STACK_SIZE);					// return err code

    RTOS_BPS_TaskCreate(&VoltTempMonitor_TCB,				// TCB
			"TASK_VOLT_TEMP_MONITOR_PRIO",	// Task Name (String)
			Task_VoltTempMonitor,				// Task function pointer
			(void *)0,				// Task function args
			TASK_VOLT_TEMP_MONITOR_PRIO,			// Priority
			VoltTempMonitor_Stk,	// Watermark limit for debugging
			TASK_VOLT_TEMP_MONITOR_STACK_SIZE);					// return err code

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
	OSTaskDel(NULL, &p_err); // Delete task
}

//Task to prevent watchdog from tripping
void Task2(void *p_arg){

    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1); //Set semaphore once since Amperes Task doesn't run
    RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1); //Set semaphore once since Battery Balancing Task doesn't run

    while(1){
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);
        WDog_BitMap |= WD_AMPERES;
        WDog_BitMap |= WD_BALANCING;
        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE);
        //delay of 100ms
        RTOS_BPS_DelayTick(10);
        BSP_Light_Toggle(RUN);
    }
}

int main(void) {
    OS_ERR err;
    
    //Resetting the contactor
    BSP_UART_Init(NULL, NULL, UART_USB);
    Contactor_Init();
    Contactor_Off(HVLOW_CONTACTOR);
    Contactor_Off(HVHIGH_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);

    if (BSP_WDTimer_DidSystemReset()) {
        Fault_BitMap = Fault_WDOG;
        EnterFaultState();
    }

    BSP_PLL_Init();
    BSP_Lights_Init();
    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);

    //Give same priority as amperes task thread
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                5,
                Task2_Stk,
                256);

    OSStart(&err);
}
