#include "os.h"
#include "Tasks.h"

void Task_Init(void *p_arg) {

    OS_ERR err;
    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);
    assertOSError(err);

    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);
    assertOSError(err);

    OSMutexCreate(&WDog_Mutex,
                "Watchdog Mutex",
                &err);
    assertOSError(err);

    // TODO: Initialize threads! Look at main.c for reference
        //0
    	OSTaskCreate(&Init_TCB,				// TCB
				"Initialize System",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_INIT_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //1
    	OSTaskCreate(&Init_TCB,				// TCB
				"TASK_FAULT_STATE_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_FAULT_STATE_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //2
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_CRITICAL_STATE_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CRITICAL_STATE_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //3
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_PETWDOG_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_PETWDOG_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //4
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_VOLT_TEMP_MONITOR_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_VOLT_TEMP_MONITOR_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //5
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_AMPERES_MONITOR_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_AMPERES_MONITOR_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //6
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_BATTERY_BALANCE_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_BATTERY_BALANCE_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //7
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_DASHBOARD_NOTIFY_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_DASHBOARD_NOTIFY_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //8
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_LOG_INFO_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_LOG_INFO_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //9
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_CANBUS_CONSUMER_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CANBUS_CONSUMER_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code
        //10
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_CLI_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CLI_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code}
        //11
        OSTaskCreate(&Init_TCB,				// TCB
				"TASK_IDLE_PRIO",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_IDLE_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code}
}
