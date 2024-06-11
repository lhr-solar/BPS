/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "RTOS_BPS.h"
#include "Tasks.h"
#include "Temperature.h"

OS_TCB Temp1_TCB;
CPU_STK Temp1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Temp2_TCB;
CPU_STK Temp2_Stk[DEFAULT_STACK_SIZE];

OS_TCB Dummy_TCB;
CPU_STK Dummy_Stk[DEFAULT_STACK_SIZE];

OS_TMR tmr;
OS_SEM tmr_Sem4;

cell_asic minions[NUM_MINIONS];

void trigger(void *p_tmr, void *p_arg) {
    RTOS_BPS_SemPost(&tmr_Sem4, OS_OPT_POST_1); 
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

    RTOS_BPS_SemCreate(&tmr_Sem4, "Temperature Timer Semaphore", 1);

    Temperature_Init(minions);

    while(1) {
        printf("1\r\n");
        Temperature_UpdateAllMeasurements();
        // OSTimeDly(10,
        //         OS_OPT_TIME_DLY,
        //         &err);
        // assert

        RTOS_BPS_SemPend(&tmr_Sem4, OS_OPT_PEND_BLOCKING);
        // assert

        Temperature_CheckStatus(1, NULL);
    }
}

void Task_TestTempMutex2(void *p_args) {
    (void)p_args;

    while(1) {
        printf("2\r\n");
        Temperature_CheckStatus(1, NULL);
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
        RTOS_BPS_SemPost(&tmr_Sem4, OS_OPT_PEND_BLOCKING);
        for(volatile int i = 0; i < 1000000; i++);
    }
}

int main() {
	
	OS_ERR err;

    OSInit(&err);
    // assert

	RTOS_BPS_TaskCreate(&Temp1_TCB,				// TCB
				"Temperature Buffer Mutex1",	// Task Name (String)
				Task_TestTempMutex1,				// Task function pointer
				(void *)0,				// Task function args
				2,			            // Priority
				Temp1_Stk,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE); 

    RTOS_BPS_TaskCreate(&Temp2_TCB,				// TCB
				"Temperature Buffer Mutex2",	// Task Name (String)
				Task_TestTempMutex2,				// Task function pointer
				(void *)0,				// Task function args
				3,			            // Priority
				Temp2_Stk,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE); 

    RTOS_BPS_TaskCreate(&Dummy_TCB,				// TCB
				"Dummy",	// Task Name (String)
				Task_Dummy,				// Task function pointer
				(void *)0,				// Task function args
				4,			            // Priority
				Dummy_Stk,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE); 

	OSStart(&err);

	// Should not get here or else there is an error
}
