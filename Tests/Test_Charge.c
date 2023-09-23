/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "Charge.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"

#include "FreeRTOS.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"

StaticTask_t Task1_TCB;
CPU_STK Task1_Stk[256];
uint32_t charge_reading;

void Task1(void *p_arg){
    BSP_UART_Init(NULL, NULL, UART_USB);
    Charge_Init();
    Charge_SetAccum(50000);
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
   
    while(1) {
        printf("Charge after 5Amps for 100 milliseconds : %ld\n\r", charge_reading);
        Charge_Calculate(5000);
        charge_reading = Charge_GetPercent();
        RTOS_BPS_DelayTick(10);
    }

    exit(0);
}

int main(void){
    OS_ERR err;
    BSP_PLL_Init();

    __disable_irq();

    OSInit(&err);
    assertOSError(err);

    xTaskCreateStatic(Task1,
		"Task 1",
		256,
		(void *)0,,
		1,
		Task1_Stk,
		&Task1_TCB);

    __enable_irq();

    OSStart(&err);
    assertOSError(err);
}
