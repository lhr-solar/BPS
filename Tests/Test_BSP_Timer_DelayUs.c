/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"
#include "RTOS_BPS.h"
#include "Tasks.h"
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

//OS_TCB Init_TCB;
//CPU_STK Init_Stk[DEFAULT_STACK_SIZE];

void Task1(void *p_arg);
void Task2(void *p_arg);
void Init(void *p_arg);
//idle

void Init(void *p_arg) {
    OS_ERR err;
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    printf("Clock speed: %ld", BSP_Timer_GetRunFreq());
    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                DEFAULT_STACK_SIZE);
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                1,
                Task2_Stk,
                DEFAULT_STACK_SIZE);
    OSTaskDel(NULL,&err);
}

void Task1(void *p_arg) {
    while (1) {
        uint32_t time = BSP_Timer_GetTicksElapsed();
        printf("Task 1 time: %ld\n\r", time);
        RTOS_BPS_DelayMs(1000);
    }
}

void Task2(void *p_arg) {
    while (1) {
        BSP_Timer_GetTicksElapsed();
        RTOS_BPS_DelayUs(900);
        uint32_t time = BSP_Timer_GetTicksElapsed();
        printf("Task 2 time: %ld\n\r", time);
        RTOS_BPS_DelayMs(100);

    }
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Timer_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);
    BSP_Timer_Start_TickCounter();
    OSInit(&err);
    RTOS_BPS_TaskCreate(&Init_TCB,
        "Init Task",
        Init,
        (void*) 0,
        1, 
        Init_Stk,
        256);
    assertOSError(err);
    OSStart(&err);

}