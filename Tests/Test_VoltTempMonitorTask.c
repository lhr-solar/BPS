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
void EnterFaultState(void);

// Initialization task for this test
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

    RTOS_BPS_MutexCreate(&WDog_Mutex, "Watchdog Mutex");

    // Spawn tasks needed for Amperes readings to affect contactor
    //1
    RTOS_BPS_TaskCreate(&FaultState_TCB,				// TCB
            "TASK_FAULT_STATE_PRIO",	// Task Name (String)
            Task_FaultState,				// Task function pointer
            (void *)0,				// Task function args
            TASK_FAULT_STATE_PRIO,			// Priority
            FaultState_Stk,	// Watermark limit for debugging
            TASK_FAULT_STATE_STACK_SIZE);					// return err code
    //2
    RTOS_BPS_TaskCreate(&CriticalState_TCB,				// TCB
            "TASK_CRITICAL_STATE_PRIO",	// Task Name (String)
            Task_CriticalState,				// Task function pointer
            (void *)0,				// Task function args
            TASK_CRITICAL_STATE_PRIO,			// Priority
            CriticalState_Stk,	// Watermark limit for debugging
            TASK_CRITICAL_STATE_STACK_SIZE);					// return err code

    //3
    RTOS_BPS_TaskCreate(&PetWDog_TCB,				// TCB
			"TASK_PETWDOG_PRIO",	// Task Name (String)
			Task_PetWDog,				// Task function pointer
			(void *)0,				// Task function args
			TASK_PETWDOG_PRIO,			// Priority
			PetWDog_Stk,	// Watermark limit for debugging
			TASK_PETWDOG_STACK_SIZE);					// return err code
    // Spawn Task_VoltTempMonitor with PRIO 4
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
    assertOSError(err);

    // Initialize CAN queue
    CAN_Queue_Init();

	//delete task
	OSTaskDel(NULL, &p_err); // Delete task
}

//Task to prevent watchdog from tripping
void Task2(void *p_arg){
    OS_ERR err;

    OSSemPost(&SafetyCheck_Sem4,      //Set semaphore once since Amperes Task doesn't run
                OS_OPT_POST_1,
                &err);
	assertOSError(err);
    OSSemPost(&SafetyCheck_Sem4,      //Set semaphore once since Battery Balancing Task doesn't run
                OS_OPT_POST_1,
                &err);
	assertOSError(err);

    while(1){
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);
        WDog_BitMap |= WD_AMPERES;
        WDog_BitMap |= WD_BALANCING;
        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);
        //delay of 100ms
        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
        assertOSError(err);
        //BSP_Light_Toggle(RUN);
    }
}

// Similar to the production code main. Does not mess with contactor 
int main(void) {

    //Resetting the contactor
    BSP_Contactor_Init();
    BSP_Contactor_Off();

    if (BSP_WDTimer_DidSystemReset()) {
        Fault_BitMap = Fault_WDOG;
        EnterFaultState();
    }

    OS_ERR err;
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
    assertOSError(err);

    //Give same priority as amperes task thread
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                5,
                Task2_Stk,
                256);
    assertOSError(err);

    OSStart(&err);
}
