/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "RTOS_BPS.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#endif

void Task_Init(void *p_arg) {

#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#else
    OS_CPU_SysTickInit();
#endif

    OS_ERR err;
    RTOS_BPS_SemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0);

    RTOS_BPS_SemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0);

    RTOS_BPS_MutexCreate(&WDog_Mutex, "Watchdog Mutex");

//    //1
//    RTOS_BPS_TaskCreate(&FaultState_TCB,	    // TCB
//            "TASK_FAULT_STATE",	                // Task Name (String)
//            Task_FaultState,			        // Task function pointer
//            (void *)0,				            // Task function args
//            TASK_FAULT_STATE_PRIO,	            // Priority
//            FaultState_Stk,			            // Stack
//            TASK_FAULT_STATE_STACK_SIZE);
//    //2
//    RTOS_BPS_TaskCreate(&CriticalState_TCB,	    // TCB
//            "TASK_CRITICAL_STATE",	            // Task Name (String)
//            Task_CriticalState,				    // Task function pointer
//            (void *)0,				            // Task function args
//            TASK_CRITICAL_STATE_PRIO,		    // Priority
//            CriticalState_Stk,				    // Stack
//            TASK_CRITICAL_STATE_STACK_SIZE);
//    //3
//    // RTOS_BPS_TaskCreate(&PetWDog_TCB,	    // TCB
//    // 		"TASK_PETWDOG",	                    // Task Name (String)
//    // 		Task_PetWDog,				        // Task function pointer
//    // 		(void *)0,				            // Task function args
//    // 		TASK_PETWDOG_PRIO,			        // Priority
//    // 		PetWDog_Stk,				        // Stack
//    // 		TASK_PETWDOG_STACK_SIZE);
//    //4
//    RTOS_BPS_TaskCreate(&VoltTempMonitor_TCB,	// TCB
//            "TASK_VOLT_TEMP_MONITOR",	        // Task Name (String)
//            Task_VoltTempMonitor,				// Task function pointer
//            (void *)0,				            // Task function args
//            TASK_VOLT_TEMP_MONITOR_PRIO,		// Priority
//            VoltTempMonitor_Stk,				// Stack
//            TASK_VOLT_TEMP_MONITOR_STACK_SIZE);
//    //5
//    RTOS_BPS_TaskCreate(&AmperesMonitor_TCB,	// TCB
//            "TASK_AMPERES_MONITOR",	            // Task Name (String)
//            Task_AmperesMonitor,				// Task function pointer
//            (void *)0,				            // Task function args
//            TASK_AMPERES_MONITOR_PRIO,			// Priority
//            AmperesMonitor_Stk,				    // Stack
//            TASK_AMPERES_MONITOR_STACK_SIZE);
//    /*
//    //6
//    RTOS_BPS_TaskCreate(&BatteryBalance_TCB,	// TCB
//            "TASK_BATTERY_BALANCE",	            // Task Name (String)
//            Task_BatteryBalance,				// Task function pointer
//            (void *)0,				            // Task function args
//            TASK_BATTERY_BALANCE_PRIO,			// Priority
//            BatteryBalance_Stk,				    // Stack
//            TASK_BATTERY_BALANCE_STACK_SIZE,	// Stack size
//            );
//    */
//    //7
//    RTOS_BPS_TaskCreate(&LogInfo_TCB,			// TCB
//            "TASK_LOG_INFO",	                // Task Name (String)
//            Task_LogInfo,				        // Task function pointer
//            (void *)0,				            // Task function args
//            TASK_LOG_INFO_PRIO,			        // Priority
//            LogInfo_Stk,				        // Stack
//            TASK_LOG_INFO_STACK_SIZE);
//    //8
//    RTOS_BPS_TaskCreate(&CANBusConsumer_TCB,	// TCB
//            "TASK_CANBUS_CONSUMER",	            // Task Name (String)
//            Task_CANBusConsumer,				// Task function pointer
//            (void *)false,				        // don't use loopback mode
//            TASK_CANBUS_CONSUMER_PRIO,			// Priority
//            CANBusConsumer_Stk,				    // Stack
//            TASK_CANBUS_CONSUMER_STACK_SIZE);
    //9
    RTOS_BPS_TaskCreate(&CLI_TCB,				// TCB
            "TASK_CLI",	                        // Task Name (String)
            Task_CLI,				            // Task function pointer
            (void *)0,				            // Task function args
            TASK_CLI_PRIO,			            // Priority
            CLI_Stk,				            // Stack
            TASK_CLI_STACK_SIZE		            // Stack size
            );
    //10
    RTOS_BPS_TaskCreate(&Idle_TCB,				// TCB
            "TASK_IDLE",	                    // Task Name (String)
            Task_Idle,				            // Task function pointer
            (void *)0,				            // Task function args
            TASK_IDLE_PRIO,			            // Priority
            Idle_Stk,				            // Stack
            TASK_IDLE_STACK_SIZE);
    
    CAN_Queue_Init();
    //delete task
    OSTaskDel(NULL, &err); // Delete task
}
