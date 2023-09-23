/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "BSP_UART.h"
#include "stm32f4xx.h"

StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[256];

StaticTask_t Task2_TCB;
CPU_STK Task2_Stk[256];

void Task1(void *p_arg);
void Task2(void *p_arg);

void Task1(void *p_arg) {
    (void)p_arg;
    
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    BSP_UART_Init(NULL, NULL, UART_USB);
    printf("Checkpoint A\n");

    RTOS_BPS_DelayTick(1);

    // Create Task 2
    xTaskCreateStatic(Task2,
		"Task 2",
		256,
		(void *)0,,
		2,
		Task2_Stk,
		&Task2_TCB);

    while(1) {
        for(int i = 0; i < 4; i++) {
	xSemaphoreTake(SafetyCheck_Sem4, (TickType_t)portMAX_DELAY);
        }

        RTOS_BPS_DelayTick(1);
        printf("1\r\n");
    }
}

void Task2(void *p_arg) {
    (void)p_arg;

    while(1) {
	xSemaphoreGive(SafetyCheck_Sem4);
        RTOS_BPS_DelayTick(1);
        printf("2\r\n");
    }
}

int main(void) {
    OS_ERR err;

    __disable_irq();

    OSInit(&err);
    assertOSError(err);

	SafetyCheck_Sem4 = xSemaphoreCreateBinary();

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);

    __enable_irq();

    OSStart(&err);
}
