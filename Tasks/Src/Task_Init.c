/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "Contactor.h"
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

    RTOS_BPS_SemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0);

    RTOS_BPS_MutexCreate(&WDog_Mutex, "Watchdog Mutex");

    RTOS_BPS_TaskCreate(&PetWDog_TCB,	        // TCB
                "TASK_PETWDOG",	                // Task Name (String)
                Task_PetWDog,				    // Task function pointer
                (void *)0,				        // Task function args
                TASK_PETWDOG_PRIO,			    // Priority
                PetWDog_Stk,				    // Stack
                TASK_PETWDOG_STACK_SIZE);
    RTOS_BPS_TaskCreate(&CheckContactor_TCB,    // TCB
                "Task_CheckContactor",          // Task Name (String)
                Task_CheckContactor,            // Task function pointer
                (void *)0,                      // Task function args
                TASK_CHECK_CONTACTOR_PRIO,      // Priority
                CheckContactor_Stk,             // Stack
                TASK_CHECK_CONTACTOR_STACK_SIZE);
    RTOS_BPS_TaskCreate(&VoltTempMonitor_TCB,	// TCB
            "TASK_VOLT_TEMP_MONITOR",	        // Task Name (String)
            Task_VoltTempMonitor,				// Task function pointer
            (void *)0,				            // Task function args
            TASK_VOLT_TEMP_MONITOR_PRIO,		// Priority
            VoltTempMonitor_Stk,				// Stack
            TASK_VOLT_TEMP_MONITOR_STACK_SIZE);
    RTOS_BPS_TaskCreate(&AmperesMonitor_TCB,	// TCB
            "TASK_AMPERES_MONITOR",	            // Task Name (String)
            Task_AmperesMonitor,				// Task function pointer
            (void *)0,				            // Task function args
            TASK_AMPERES_MONITOR_PRIO,			// Priority
            AmperesMonitor_Stk,				    // Stack
            TASK_AMPERES_MONITOR_STACK_SIZE);
    
    RTOS_BPS_TaskCreate(&BatteryBalance_TCB,	// TCB
            "TASK_BATTERY_BALANCE",	            // Task Name (String)
            Task_BatteryBalance,				// Task function pointer
            (void *)0,				            // Task function args
            TASK_BATTERY_BALANCE_PRIO,			// Priority
            BatteryBalance_Stk,				    // Stack
            TASK_BATTERY_BALANCE_STACK_SIZE);	// Stack size
            
    
    RTOS_BPS_TaskCreate(&LogInfo_TCB,			// TCB
            "TASK_LOG_INFO",	                // Task Name (String)
            Task_LogInfo,				        // Task function pointer
            (void *)0,				            // Task function args
            TASK_LOG_INFO_PRIO,			        // Priority
            LogInfo_Stk,				        // Stack
            TASK_LOG_INFO_STACK_SIZE);
    RTOS_BPS_TaskCreate(&CANBusConsumer_TCB,	// TCB
            "TASK_CANBUS_CONSUMER",	            // Task Name (String)
            Task_CANBusConsumer,				// Task function pointer
            (void *)BPS_CAN_LOOPBACK,				        // don't use loopback mode
            TASK_CANBUS_CONSUMER_PRIO,			// Priority
            CANBusConsumer_Stk,				    // Stack
            TASK_CANBUS_CONSUMER_STACK_SIZE);
    /*
    RTOS_BPS_TaskCreate(&CLI_TCB,				// TCB
            "TASK_CLI",	                        // Task Name (String)
            Task_CLI,				            // Task function pointer
            (void *)0,				            // Task function args
            TASK_CLI_PRIO,			            // Priority
            CLI_Stk,				            // Stack
            TASK_CLI_STACK_SIZE,		        // Stack size
            );
    */
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
