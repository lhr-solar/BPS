/* Copyright (c) 2020 UT Longhorn Racing Solar */

/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_WDTimer.h"
void EnterFaultState(void);

int main() {
	
	OS_ERR err;

	// If the WDTimer counts down to 0, then the BPS resets. If BPS has reset, enter a fault state.
	if (BSP_WDTimer_DidSystemReset()) {
		EnterFaultState();
	}

	OSInit(&err);
	assertOSError(err);

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
	assertOSError(err);

	OSStart(&err);

	// Should not get here or else there is an error
}

