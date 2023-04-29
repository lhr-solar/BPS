/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include "common.h"
#include "config.h"
#include "Tasks.h"
#include "BSP_WDTimer.h"
#include "Contactor.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"
#include "BSP_Timer.h"
#include "EEPROM.h"
#include "Charge.h"
#include "BSP_Lights.h"
#ifdef SIMULATION
#include "Simulator.h"
#endif

// the simulator take command line args, the embedded version does not
#ifdef SIMULATION
int main(int argc, char **argv) {
#else
int main() {
#endif

#ifdef SIMULATION
    // the first command line argument is the path to the JSON file
    Simulator_Init(argv[1]);
#endif
    
    OS_ERR err;

    BSP_PLL_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);	

    //Resetting the contactor
    Contactor_Init();
    Contactor_Off(HVLOW_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);
    Contactor_Off(HVHIGH_CONTACTOR);
    
    BSP_Lights_Init();
    BSP_Timer_Init();

    // If the WDTimer counts down to 0, then the BPS resets. If BPS has reset, enter a fault state.
    if (BSP_WDTimer_DidSystemReset()) {
        Fault_BitMap = Fault_WDOG; //When function called in if statement, RCC flag cleared so set bitmap here
        EnterFaultState();
    }

    // set up EEPROM and state of charge
    EEPROM_Init();
    Charge_Init();

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Init_TCB,		// TCB
        "TASK_INIT",	                // Task Name (String)
        Task_Init,				        // Task function pointer
        (void *)0,				        // Task function args
        TASK_INIT_PRIO,			        // Priority
        Init_Stk,				        // Stack
        DEFAULT_STACK_SIZE);	        // Stack size
                
    OSStart(&err);

    // Should not get here or else there is an error
}
