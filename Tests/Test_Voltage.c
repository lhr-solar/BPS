/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "Voltage.h"
#include "BSP_UART.h"
#include "BSP_Lights.h"
#include "Tasks.h"
#include "BSP_PLL.h"
#include "stm32f4xx.h"
#include "Amps.h"

/******************************************************************************
 * Voltage App Test Plan
 * 
 * 1. Set up the BPS test board such that the voltages for each simulated module are between 0 and 11 Volts
 * 2. Connect the BPS's voltage probes to the BPS test board
 * 3. Modify NUM_MINIONS and NUM_BATTERY_MODULES in Config/Inc/config.h to match your testing setup (probably 1 and 8)
 * 4. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 5. Flash the BPS (see README.md)
 * 6. Set up the BPS test board such that the voltages for each simulated module are between 2.7 and 4.0 Volts (safe)
 * 7. Connect the BPS's voltage probes to the BPS test board
 * 8. Connect your computer to the BPS's UART-USB port and open a COM port terminal
 * 9. Reset the BPS by pressing and releasing the reset button
 * 10. Check the terminal output
 * 11. Repeat with some modules between 0 and 2.55 Volts (undervoltage)
 * 12. Repeat with some modules between 4.0 and 5.0 Volts (overvoltage)
 * 13. Repeat with positive current (discharging) between 2.55 and 2.7 Volts (undervoltage)
 *     (Requires working Amperes task, may need to put off until lead-acid testing) 
 * 14. Repeat with negative current (charging) between 2.55 and 2.7 Volts (safe)
 *     (Requires working Amperes task, may need to put off until lead-acid testing) 
 * 
 * Expected Output:
 * 
 * Testing Voltage_UpdateMeasurements and Voltage_GetModuleMillivoltage...
 * <list of modules and voltages of the form "[module number]: [voltage]mV".
 *  These should be compared with readings from a multimeter (and both measurements recorded).
 *  The goal is to be within 10mV of the multimeter's readings>
 * 
 * Testing Voltage_CheckStatus...
 * Is Charging: <"true" or "false", depending on what the BPS thinks is happening. This is only important for steps 13 and 14>
 * Status: <"SAFE", "OVERVOLTAGE", or "UNDERVOLTAGE". This should match the expected result in () for that step.
 * 
 * Testing Voltage_GetModulesInDanger...
 * <for all of the modules:
 * Module <n> voltage status <SAFE, OVERVOLTAGE, or UNDERVOLTAGE depending on test board settings>>
 * <for all of the wires from the minion board to the test board:
 * Wire <n> status: <SAFE or DANGER depending on if wire is plugged in>>
 * 
 * Testing Voltage_OpenWire...
 * Open Wire Status: <SAFE or DANGER depending on if all wires are plugged in>
 * 
 * Testing Voltage_GetOpenWire...
 * Open wire bitmap: <a hexadecimal number where each bit represents a wire. 0 is connected, 1 is disconnected>
 * 
 * Testing Voltage_OpenWireSummary...
 * <A big summary of the open wire statuses that looks reasonable. This is just a wrapper for vendor code>
 * 
 * -----End of Voltage_OpenWireSummary-----
 * 
 * Testing Voltage_GetTotalPackVoltage...
 * Total pack voltage: <this number should be the sum of all of the module voltages> millivolts
 * 
 *****************************************************************************/

// Used by Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

cell_asic minions[NUM_MINIONS];

void test(void) {
    char *statuses[4] = {"SAFE", "DANGER", "OVERVOLTAGE", "UNDERVOLTAGE"};

    BSP_UART_Init(NULL, NULL, UART_USB);    // Initialize printf

    printf("Testing Voltage_UpdateMeasurements and Voltage_GetModuleMillivoltage...\r\n");

    Voltage_Init(minions);

    Voltage_UpdateMeasurements();

    for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
        printf("\t%d: %dmV\r\n", i, Voltage_GetModuleMillivoltage(i));
    }

    printf("\r\nTesting Voltage_CheckStatus...\r\n");
    printf("Is charging: %s\r\n", (Amps_IsCharging() ? "true" : "false"));
    printf("Status: %s\r\n", statuses[Voltage_CheckStatus(NULL)]);

    VoltageSafety_t v;
    printf("\r\nTesting Voltage_GetModulesInDanger...\r\n");
    Voltage_GetModulesInDanger(&v);
    for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
        printf("Module %d voltage status: %s\n\r", i, statuses[v.module_checks[i]]);
    }
    for (int i = 0; i < MAX_VOLT_WIRES; i++) {
        printf("Wire %d status: %s\n\r", i, statuses[v.wire_checks[i]]);
    }

    printf("\r\nTesting Voltage_OpenWire...\r\n");
    printf("Open Wire Status: %s\r\n", statuses[Voltage_OpenWire()]);
    
    printf("\r\nTesting Voltage_GetOpenWire...\r\n");
    printf("Open wire bitmap: %lx\r\n", Voltage_GetOpenWire());

    printf("\r\nTesting Voltage_OpenWireSummary...\r\n");
    Voltage_OpenWireSummary();
    printf("\r\n-----End of Voltage_OpenWireSummary-----\r\n");

    printf("\r\nTesting Voltage_GetTotalPackVoltage...\r\n");
    printf("Total pack voltage: %lu millivolts\r\n", Voltage_GetTotalPackVoltage());
}

// Task for this test
void Task1(void *p_arg){
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    while (1) {
        test();
        BSP_Light_Toggle(RUN);
    }
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
int main(void) {
    OS_ERR err;
    BSP_PLL_Init();

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

    OSStart(&err);
}
