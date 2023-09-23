/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#else
#include "Simulator.h"
#endif
#include "BSP_Lights.h"
#include "CAN_Queue.h"
#include "BSP_PLL.h"
#include "BSP_WDTimer.h"
#include "Contactor.h"
#include "FreeRTOS.h"
#include "Voltage.h"
#include "BSP_UART.h"
#include "Temperature.h"

/******************************************************************************
 * VoltTempMonitor Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 *  --Remember to change Config file parameters NUM_BATTERY_MODULES, NUM_TEMPERATURE_SENSORS, & MAX_VOLT_SENSORS_PER_MINION_BOARD
 * 2. Set up the BPS leader, Minion boards, and Contactor.
 * 3. Flash the BPS (see README.md)
 * 4. Connect a logic analyzer to the BPS's CAN port
 * 5. Start recording the BPS's CAN output
 * 6. Provide UVOLT, SAFE, OVOLT, OWIRE, and OTEMP cases before starting task 
 * and see if task fails for all cases (hear contactor open, see lights turn on).
 * 7. Allow task to run and then check for all fault cases again.
 * 8. Check the logic analyzer recording for the BPS's CAN messages and verfiy
 *    that the observed CAN messages contain accurate readings.
 *****************************************************************************/

// Used by Task1
StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

StaticTask_t Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

OS_ERR p_err;

void foo(void){
    return;
}

// Initialization task for this test
void Task1(void *p_arg){
    
#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#endif

	SafetyCheck_Sem4 = xSemaphoreCreateBinary();

WDog_Mutex = xSemaphoreCreateMutex();

    // Spawn tasks needed for Amperes readings to affect contactor
    xTaskCreateStatic(Task_PetWDog,
		"TASK_PETWDOG_PRIO",
		TASK_PETWDOG_STACK_SIZE,
		(void *)0,                  // Task function args,
		TASK_PETWDOG_PRIO,
		PetWDog_Stk,
		&PetWDog_TCB
                );

    xTaskCreateStatic(Task_VoltTempMonitor,
		"TASK_VOLT_TEMP_MONITOR_PRIO",
		TASK_VOLT_TEMP_MONITOR_STACK_SIZE,
		(void *)0,				// Task function args,
		TASK_VOLT_TEMP_MONITOR_PRIO,
		VoltTempMonitor_Stk,
		&VoltTempMonitor_TCB);					// return err code

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
    OSTaskDel(NULL, &p_err); // Delete task
}

//Task to prevent watchdog from tripping
void Task2(void *p_arg){

	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);

    while(1){
	xSemaphoreTake(WDog_Mutex, (TickType_t)portMAX_DELAY); 
        WDog_BitMap |= WD_AMPERES;
        WDog_BitMap |= WD_BALANCING;
	xSemaphoreGive(WDog_Mutex);
        //delay of 100ms
        RTOS_BPS_DelayTick(1);
    }
}

// Similar to the production code main. Does not mess with contactor 
#ifdef SIMULATION
int main(int argc, char **argv) {
#else
int main() {
#endif
    OS_ERR err;
    
    //Resetting the contactor
    Contactor_Init();
    Contactor_Off(HVLOW_CONTACTOR);
    Contactor_Off(HVHIGH_CONTACTOR);
    Contactor_Off(ARRAY_CONTACTOR);

    if (BSP_WDTimer_DidSystemReset()) {
        Fault_BitMap = Fault_WDOG;
        EnterFaultState();
    }

    BSP_PLL_Init();
    BSP_UART_Init(foo, foo, UART_USB);
    BSP_Lights_Init();
    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);

    //Give same priority as amperes task thread
    xTaskCreateStatic(Task2,
		"Task 2",
		256,
		(void *)0,,
		5,
		Task2_Stk,
		&Task2_TCB);

    OSStart(&err);
}
