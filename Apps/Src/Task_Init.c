/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "os.h"
#include "config.h"
#include "Tasks.h"
#include "CAN_Queue.h"

#ifndef SIMULATION
#include "stm32f4xx.h"
#endif

void Task_Init(void *p_arg) {

#ifndef SIMULATION
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#endif

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
		assertOSError(err);
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
		assertOSError(err);
        //3
        // OSTaskCreate(&PetWDog_TCB,				// TCB
		// 		"TASK_PETWDOG_PRIO",	// Task Name (String)
		// 		Task_PetWDog,				// Task function pointer
		// 		(void *)0,				// Task function args
		// 		TASK_PETWDOG_PRIO,			// Priority
		// 		PetWDog_Stk,				// Stack
		// 		WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
		// 		TASK_PETWDOG_STACK_SIZE,		// Stack size
		// 		0,						// Queue size (not needed)
		// 		10,						// Time quanta (time slice) 10 ticks
		// 		(void *)0,				// Extension pointer (not needed)
		// 		OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
		// 		&err);					// return err code
		// assertOSError(err);
        //4
        OSTaskCreate(&VoltTempMonitor_TCB,				// TCB
				"TASK_VOLT_TEMP_MONITOR_PRIO",	// Task Name (String)
				Task_VoltTempMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_VOLT_TEMP_MONITOR_PRIO,			// Priority
				VoltTempMonitor_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_VOLT_TEMP_MONITOR_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);
        //5
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
		assertOSError(err);
	/*
        //6
        OSTaskCreate(&BatteryBalance_TCB,				// TCB
				"TASK_BATTERY_BALANCE_PRIO",	// Task Name (String)
				Task_BatteryBalance,				// Task function pointer
				(void *)0,				// Task function args
				TASK_BATTERY_BALANCE_PRIO,			// Priority
				BatteryBalance_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_BATTERY_BALANCE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);
		*/
        //7
        OSTaskCreate(&LogInfo_TCB,				// TCB
				"TASK_LOG_INFO_PRIO",	// Task Name (String)
				Task_LogInfo,				// Task function pointer
				(void *)0,				// Task function args
				TASK_LOG_INFO_PRIO,			// Priority
				LogInfo_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_LOG_INFO_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);
        //8
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
	/*
        //9
        OSTaskCreate(&CLI_TCB,				// TCB
				"TASK_CLI_PRIO",	// Task Name (String)
				Task_CLI,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CLI_PRIO,			// Priority
				CLI_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_CLI_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);
	*/
        //10
        OSTaskCreate(&Idle_TCB,				// TCB
				"TASK_IDLE_PRIO",	// Task Name (String)
				Task_Idle,				// Task function pointer
				(void *)0,				// Task function args
				TASK_IDLE_PRIO,			// Priority
				Idle_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_IDLE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
		assertOSError(err);
        
		CAN_Queue_Init();
        assertOSError(err);
	//delete task
	OSTaskDel(NULL, &err); // Delete task
}
