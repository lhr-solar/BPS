/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "Contactor.h"
#include "FreeRTOS.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#endif

void Task_Init(void *p_arg) {

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

    xTaskCreateStatic(Task_PetWDog,
		"TASK_PETWDOG",
		TASK_PETWDOG_STACK_SIZE,
		(void *)0,				        // Task function args,
		TASK_PETWDOG_PRIO,
		PetWDog_Stk,
		&PetWDog_TCB);
    xTaskCreateStatic(Task_CheckContactor,
		"Task_CheckContactor",
		TASK_CHECK_CONTACTOR_STACK_SIZE,
		(void *)0,                      // Task function args,
		TASK_CHECK_CONTACTOR_PRIO,
		CheckContactor_Stk,
		&CheckContactor_TCB);
    xTaskCreateStatic(Task_VoltTempMonitor,
		"TASK_VOLT_TEMP_MONITOR",
		TASK_VOLT_TEMP_MONITOR_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_VOLT_TEMP_MONITOR_PRIO,
		VoltTempMonitor_Stk,
		&VoltTempMonitor_TCB);
    xTaskCreateStatic(Task_AmperesMonitor,
		"TASK_AMPERES_MONITOR",
		TASK_AMPERES_MONITOR_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_AMPERES_MONITOR_PRIO,
		AmperesMonitor_Stk,
		&AmperesMonitor_TCB);
    /*
    xTaskCreateStatic(Task_BatteryBalance,
		"TASK_BATTERY_BALANCE",
		TASK_BATTERY_BALANCE_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_BATTERY_BALANCE_PRIO,
		BatteryBalance_Stk,
		&BatteryBalance_TCB,	// Stack size
            );
    */
    xTaskCreateStatic(Task_LogInfo,
		"TASK_LOG_INFO",
		TASK_LOG_INFO_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_LOG_INFO_PRIO,
		LogInfo_Stk,
		&LogInfo_TCB);
    xTaskCreateStatic(Task_CANBusConsumer,
		"TASK_CANBUS_CONSUMER",
		TASK_CANBUS_CONSUMER_STACK_SIZE,
		(void *)BPS_CAN_LOOPBACK,				        // don't use loopback mode,
		TASK_CANBUS_CONSUMER_PRIO,
		CANBusConsumer_Stk,
		&CANBusConsumer_TCB);
    /*
    xTaskCreateStatic(Task_CLI,
		"TASK_CLI",
		TASK_CLI_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_CLI_PRIO,
		CLI_Stk,
		&CLI_TCB,		        // Stack size
            );
    */
    xTaskCreateStatic(Task_Idle,
		"TASK_IDLE",
		TASK_IDLE_STACK_SIZE,
		(void *)0,				            // Task function args,
		TASK_IDLE_PRIO,
		Idle_Stk,
		&Idle_TCB);
    
    CAN_Queue_Init();

    //delete task
    OSTaskDel(NULL, &err); // Delete task
}
