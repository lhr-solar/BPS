/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "BatteryBalancing.h"
#include "Tasks.h"

/******************************************************************************
 * BatteryBalancing Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Flash the BPS (see README.md)
 * 3. Connect the BPS UART USB to your laptop and open PuTTY (or comparable)
 * 4. Connect the BPS test board
 * 5. Keep all modules at half voltage except one (of your choice) and keep that at full
 * 6. Reset the BPS by pressing and releasing the reset button
 * 7. Probe the balancing transistor to ensure it is turned on
 * 8. Repeat 5-7 for different modules
 ****************************************************************************/

void Task1(void* p_arg) {
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    OS_ERR err;

    OSTaskCreate(
        &BatteryBalance_TCB,				// TCB
        "TASK_BATTERY_BALANCE",	// Task Name (String)
        Task_BatteryBalance,				// Task function pointer
        (void *)0,				// Task function args
        6,			            // Priority
        BatteryBalance_Stk,				// Stack
        WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
        DEFAULT_STACK_SIZE,		// Stack size
        0,						// Queue size (not needed)
        10,						// Time quanta (time slice) 10 ticks
        (void *)0,				// Extension pointer (not needed)
        OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
        &err
    );					// return err code}
}

void main(void) {
    OS_ERR err;
    BSP_PLL_Init();

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

    OSStart(&err);
}
