/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_UART.h"
#include "Tasks.h"
#include "BSP_PLL.h"
#include "stm32f4xx.h"
#include "Amps.h"

/******************************************************************************
 * Amperes App Test Plan
 * 
 * 1. Set up the BPS test board such that the voltages for each simulated module are between 0 and 6 Volts
 * 2. Connect the BPS's voltage probes to the BPS test board
 * 3. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 4. Flash the BPS (see README.md)
 * 5. Replace the shunt resistor board with a breadboard and connnect it to the amperes board. Use this breadboard to input the desired test current into the current board.
 * 6. Connect your computer to the BPS's UART-USB port and open a COM port terminal
 * 7. Reset the BPS by pressing and releasing the reset button
 * 8. Check the terminal output
 * 9. Repeat with positive current (discharging)
 * 10. Repeat with negative current (charging)
 *     
 * 
 * Expected Output:
 * 
 * Testing Amps_UpdateMeasurements and Amps_GetReading ...
 * Value is in milliampers and should be compared with readings from a multimeter.
 * 
 * Testing Amps_CheckStatus...
 * Is Charging: <"true" or "false", depending on what the BPS thinks is happening. This is only important for steps 9 and 10>
 * Status: <"SAFE", "OVERVOLTAGE", or "UNDERVOLTAGE". This should match the expected result in () for that step.
 * 
 * 
 *****************************************************************************/

// Used by Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];


void test(void) {
    
    char *statuses[4] = {"SAFE", "DANGER", "OVERVOLTAGE", "UNDERVOLTAGE"};

    BSP_UART_Init(NULL, NULL, UART_USB);    

    printf("Testing Amps_UpdateMeasurements and Amps_GetReading...\r\n");

    Amps_Init();

    Amps_UpdateMeasurements();

    printf("%ld mA", Amps_GetReading());

    printf("\r\nTesting Amps_CheckStatus and Amps_IsCharging ...\r\n");\
    bool check = Amps_IsCharging();
    printf("Is charging: %s\r\n", (check ? "true" : "false"));
    printf("Status: %s\r\n", statuses[Amps_CheckStatus(check)]);


}



// Task for this test
void Task1(void *p_arg){
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
	
    test();
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
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
