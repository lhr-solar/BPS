/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "BSP_PLL.h"
#include "LTC2315.h"
#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_UART.h"
#include "Amps.h"

/******************************************************************************
 * LTC2315 Driver and Amperes App Test Plan
 * 
 * 1. Set up BPS Leader, Amperes, and Shunt Resistor boards so that they are connected properly.
 * 2. Power on the system.
 * 3. Build this test program and flash it onto the BPS.
 * 4. Connect your computer to the BPS's UART-USB port and open it in Putty.
 * 5. Reset the BPS.
 * 6. Apply a small positive voltage (> 5mV, < 15mV) to the shunt resistor. The BPS should read a positive current.
 * 7. Apply a small negative voltage (> -15mV, < -5mV) to the shunt resistor. The BPS should read a negative current.
 * 8. Apply a range of small positive and negative voltages to the shunt resistor and compare with expected currents.
 * 9. Use a power supply to run a known positive current through the shunt resistor.
 *    Verify that the BPS UART output agrees with the power supply (fail the test if we are more than 200mA off)
 * 10. Repeat step 8 with a negative current.
 * 11. Use a Lithium-Ion battery module from the pack with a current-limiting resistor to run a large current (>75 Amps)
 *     through the shunt resistor. Verify that the BPS UART output (fail the test if we are more than 200mA off)
 * 12. Repeat step 10 with a large negative current (<-20 Amps)
 * 
 * Expected Output:
 * Current (milliAmps): <Positive or Negative value for step 6/7>
 * Is charging: <"true" if Negative current, "false" if Positive current>
 * Status: <"SAFE" if current is between (-20000, 75000)mA, "DANGER" if outside of range>
 * 
 ******************************************************************************/

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

void Task1(void *p_arg){
    OS_ERR err;
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    
    BSP_Lights_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    Amps_Init(); // I could write this out, but it just initializes the semaphore and mutex and calls LTC2315_Init()
    
    char *statuses[4] = {"SAFE", "DANGER", "OVERVOLTAGE", "UNDERVOLTAGE"};

    while(1) {
        Amps_UpdateMeasurements();
        printf("Current (milliAmps): %ld\n\r", Amps_GetReading());

        bool check = Amps_IsCharging();
        printf("Is charging: %s\n\r", (check ? "true" : "false"));
        printf("Status: %s\n\r", statuses[Amps_CheckStatus(check)]);

        BSP_Light_Toggle(EXTRA);
        
        RTOS_BPS_TimeDelay(100);
    }

    exit(0);
}

int main(void){
    OS_ERR err;
    BSP_PLL_Init();

    __disable_irq();

    OSInit(&err);
    while(err != OS_ERR_NONE);

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
    while(err != OS_ERR_NONE);

    __enable_irq();

    OSStart(&err);
}
