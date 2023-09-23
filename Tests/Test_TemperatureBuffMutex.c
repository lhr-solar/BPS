/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "Temperature.h"

StaticTask_t Temp1_TCB;
CPU_STK Temp1_Stk[DEFAULT_STACK_SIZE];

StaticTask_t Temp2_TCB;
CPU_STK Temp2_Stk[DEFAULT_STACK_SIZE];

StaticTask_t Dummy_TCB;
CPU_STK Dummy_Stk[DEFAULT_STACK_SIZE];

OS_TMR tmr;
SemaphoreHandle_t tmr_Sem4;

cell_asic minions[NUM_MINIONS];

void trigger(void *p_tmr, void *p_arg) {
	xSemaphoreGive(tmr_Sem4);
}

void Task_TestTempMutex1(void *p_args) {
    (void)p_args;

    OS_ERR err;

    OSTmrCreate(&tmr,
                "Temperature Timer",
                10,
                0,
                OS_OPT_TMR_ONE_SHOT,
                trigger,
                NULL,
                &err);
    assertOSError(err);

	tmr_Sem4 = xSemaphoreCreateBinary();

    Temperature_Init(minions);

    while(1) {
        printf("1\r\n");
        Temperature_UpdateAllMeasurements();
        // OSTimeDly(10,
        //         OS_OPT_TIME_DLY,
        //         &err);
        // assert

	xSemaphoreTake(tmr_Sem4, (TickType_t)portMAX_DELAY);
        // assert

        Temperature_CheckStatus(1);
    }
}

void Task_TestTempMutex2(void *p_args) {
    (void)p_args;

    while(1) {
        printf("2\r\n");
        Temperature_CheckStatus(1);
        for(int i = 0; i < 16; i++) {
            printf("%ld\r\n", Temperature_GetModuleTemperature(i));
        }
        // for(int i = 0; i < 1000000; i++);
    }
}

void Task_Dummy(void *p_args) {
    (void)p_args;

    while(1) {
        printf("3\r\n");
	xSemaphoreGive(tmr_Sem4);
        for(volatile int i = 0; i < 1000000; i++);
    }
}

int main() {
	
	OS_ERR err;

    OSInit(&err);
    // assert

	xTaskCreateStatic(Task_TestTempMutex1,
		"Temperature Buffer Mutex1",
		DEFAULT_STACK_SIZE,
		(void *)0,
		2,
		Temp1_Stk,
		&Temp1_TCB); 

    xTaskCreateStatic(Task_TestTempMutex2,
		"Temperature Buffer Mutex2",
		DEFAULT_STACK_SIZE,
		(void *)0,
		3,
		Temp2_Stk,
		&Temp2_TCB); 

    xTaskCreateStatic(Task_Dummy,
		"Dummy",
		DEFAULT_STACK_SIZE,
		(void *)0,
		4,
		Dummy_Stk,
		&Dummy_TCB); 

	OSStart(&err);

	// Should not get here or else there is an error
}
