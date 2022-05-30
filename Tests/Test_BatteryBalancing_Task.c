/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "BatteryBalancing.h"
#include "Voltage.h"
#include "BSP_PLL.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "config.h"

/******************************************************************************
 * BatteryBalancing Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Flash the BPS (see README.md)
 * 3. Connect the BPS UART USB to your laptop and open PuTTY (or comparable)
 * 4. Connect the BPS test board
 * 5. If possible, keep all modules at half voltage except one (of your choice) and keep that at full
 *  a) Otherwise, measure batteries manually and see which ones are off balance. 
 *     If none are, change balancing tolerance.
 * 6. Reset the BPS by pressing and releasing the reset button
 * 7. Check battery balance diodes to see which ones are turning on (indicating that battery is being balanced)
 * 8. Repeat 5-7 for different modules
 * 9. Put all modules to full voltage except one (leaving it at half)
 * 10. Measure the voltage across each of the resistors besides the half voltage module
 ****************************************************************************/

// Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

// Task_UpdateVoltage
OS_TCB UpdateVoltage_TCB;
CPU_STK UpdateVoltage_Stk[DEFAULT_STACK_SIZE];

extern cell_asic Minions[NUM_MINIONS];

void Task_UpdateVoltage(void* p_arg) {
    (void) p_arg;
    OS_ERR err;

    Voltage_Init(Minions);

    while (1) {
        Voltage_UpdateMeasurements();
        OSTimeDly(5, OS_OPT_TIME_DLY, &err);
        assertOSError(err);
    }
}

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

    OSTaskCreate(
        &UpdateVoltage_TCB,				// TCB
        "TASK_UPDATE_VOLTAGE",	// Task Name (String)
        Task_UpdateVoltage,				// Task function pointer
        (void *)0,				// Task function args
        5,			            // Priority
        UpdateVoltage_Stk,				// Stack
        WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
        DEFAULT_STACK_SIZE,		// Stack size
        0,						// Queue size (not needed)
        10,						// Time quanta (time slice) 10 ticks
        (void *)0,				// Extension pointer (not needed)
        OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
        &err
    );					// return err code}

    OSTaskDel(NULL, &err);
}

int main(void) {
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
