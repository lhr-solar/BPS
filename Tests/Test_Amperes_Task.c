/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "Tasks.h"
#ifdef SIMULATION
#include "Simulator.h"
#else
#include "stm32f4xx.h"
#endif
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"
#include "BSP_WDTimer.h"
#include "Contactor.h"
#include "BSP_UART.h"
#include "Amps.h"
#include "FreeRTOS.h"
#include <stdio.h>

/******************************************************************************
 * Amperes Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Set up the BPS leader, amperes and shunt resistor boards. Also connect the contactor
 * 3. Flash the BPS (see README.md)
 * 4. Connect a logic analyzer to the BPS's CAN port
 * 5. Start recording the BPS's CAN output
 * 6. Appy a known current to the BPS's shunt resistor (between -20 and 75 Amps)
 * 7. Reset the BPS by pressing and releasing the reset button
 * 8. Check the logic analyzer recording for the BPS's CAN messages and verfiy
 *    that the observed CAN messages contain accurate current readings (within the range 
 *    allowed by the Amperes App test)
 * 9. Apply an overcurrent to the BPS (less than -20 Amps or greater than 75 Amps) and
 *    verify that the contactor opens (the contactor should have closed earlier on startup)
 *****************************************************************************/

// Used by Task1
StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

StaticTask_t Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

// Initialization task for this test
void Task1(void *p_arg){
#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#else
    OS_CPU_SysTickInit();
#endif

    OS_ERR err;

	SafetyCheck_Sem4 = xSemaphoreCreateBinary();
                "Safety Check Semaphore",
                0);

WDog_Mutex = xSemaphoreCreateMutex();

    // Spawn tasks needed for Amperes readings to affect contactor
    //1
    xTaskCreateStatic(Task_CheckContactor,
		"Task_CheckContactor",
		TASK_CHECK_CONTACTOR_STACK_SIZE,
		(void *)0,
		TASK_CHECK_CONTACTOR_PRIO,
		CheckContactor_Stk,
		&CheckContactor_TCB);

    // Spawn Task_AmperesMonitor with PRIO 5
    xTaskCreateStatic(Task_AmperesMonitor,
		"TASK_AMPERES_MONITOR_PRIO",
		TASK_AMPERES_MONITOR_STACK_SIZE,
		(void *)0,
		TASK_AMPERES_MONITOR_PRIO,
		AmperesMonitor_Stk,
		&AmperesMonitor_TCB);					// return err code

    // Spawn CANBUS Consumer, PRIO 7
    xTaskCreateStatic(Task_CANBusConsumer,
		"TASK_CANBUS_CONSUMER_PRIO",
		TASK_CANBUS_CONSUMER_STACK_SIZE,
		(void *)true,				// Use loopback mode,
		TASK_CANBUS_CONSUMER_PRIO,
		CANBusConsumer_Stk,
		&CANBusConsumer_TCB);					// return err code
        
    // Initialize CAN queue
    CAN_Queue_Init();

	//delete task
	OSTaskDel(NULL, &err); // Delete task
}

//Task to prevent watchdog from tripping
void Task2(void *p_arg){
    int count = 0;

    // get contactor to close without temperature, voltage, or open wire readings
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);

    while(1){
	xSemaphoreTake(WDog_Mutex, (TickType_t)portMAX_DELAY); 
        WDog_BitMap |= WD_VOLT_TEMP;
        WDog_BitMap |= WD_BALANCING;
	xSemaphoreGive(WDog_Mutex);
        //delay of 100ms
        RTOS_BPS_DelayTick(10);

        if (count == 0) {
            printf("Amps: %lld\n\r", (int64_t)Amps_GetReading());
        }
        count = (count + 1) % 10;

        BSP_Light_Toggle(RUN);
    }
}

// Similar to the production code main. Does not check watchdog or mess with contactor 
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
    Contactor_Off(HVHIGH_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);

    // If the WDTimer counts down to 0, then the BPS resets. If BPS has reset, enter a fault state.
    if (BSP_WDTimer_DidSystemReset()) {
	    EnterFaultState();
    }

    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);

    //Give same priority as volt temp task thread
    xTaskCreateStatic(Task2,
		"Task 2",
		256,
		(void *)0,,
		4,
		Task2_Stk,
		&Task2_TCB);

    OSStart(&err);
}
