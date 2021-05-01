/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "BSP_PLL.h"
#include "AS8510.h"
#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_UART.h"
#include "Amps.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

void Task1(void *p_arg){
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    //OS_CPU_SysTickInit();
    
    BSP_Lights_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    BSP_Light_On(EXTRA);
    Amps_Init(); // I could write this out, but it just initializes the semaphore and mutex and calls AS8510_Init()
    BSP_Light_Off(EXTRA);
   
    while(1) {
        int16_t current = AS8510_GetCurrent();
        printf("current: %d\n\r", current);
        BSP_Light_Toggle(EXTRA);
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