/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#else
#include "Simulator.h"
#endif
#include "Contactor.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"

StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[256];
StaticTask_t Task2_TCB;
CPU_STK Task2_Stk[256];

/******************************************************************************
 * Check Contactor Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Flash the BPS (see README.md)
 * 3. Run the code in this file. NOTE: This code includes a heartbeat.
 * 4. Make sure the contactor goes from open to closed. Use an ohmmeter to check the resistance across the contactor's terminals to determine the state of the contactor (this resistance should be close to 0 Ohms).
 * 5. Unpower the contactor, while keeping the BPS powered on. Make sure the BPS goes into a fault state
****************************************************************************/

void Task2(void *p_arg){    //This task is meant to allow contactor to close

    RTOS_BPS_DelayTick(250);

	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);
	xSemaphoreGive(SafetyCheck_Sem4);

    BSP_Lights_Init();
   
    while(1) {
        BSP_Light_Toggle(RUN);
        RTOS_BPS_DelayTick(25);
    }

    exit(0);
}

void Task1(void *p_arg){
    OS_ERR err;

#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#else
    OS_CPU_SysTickInit();
#endif

	SafetyCheck_Sem4 = xSemaphoreCreateBinary();

    xTaskCreateStatic(Task_CheckContactor,
		"Task_CheckContactor",
		TASK_CHECK_CONTACTOR_STACK_SIZE,
		(void *)0,
		TASK_CHECK_CONTACTOR_PRIO,
		CheckContactor_Stk,
		&CheckContactor_TCB
                );

    xTaskCreateStatic(Task2,
		"Task 2",
		256,
		(void *)0,,
		4,
		Task2_Stk,
		&Task2_TCB);
    
    xTaskCreateStatic(Task_CANBusConsumer,
        "TASK_CANBUS_CONSUMER",
        TASK_CANBUS_CONSUMER_STACK_SIZE,
        (void *)false,
        TASK_CANBUS_CONSUMER_PRIO,
        CANBusConsumer_Stk,
        &CANBusConsumer_TCB);

    CAN_Queue_Init();

    OSTaskDel(NULL, &err);
}

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
    BSP_Lights_Init();
    Contactor_Init();

#ifndef SIMULATION
    __disable_irq();
#endif

    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);

#ifndef SIMULATION
    __enable_irq();
#endif

    OSStart(&err);
    assertOSError(err);
}
