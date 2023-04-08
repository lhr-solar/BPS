/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"
#include "BSP_PLL.h"
#include "RTOS_BPS.h"
#include "Tasks.h"
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];
void Task1(void *p_arg);
void Task2(void *p_arg);

//idle
void Task1(void *p_arg) {
    while (1) {
        uint32_t time = BSP_Timer_GetTicksElapsed();
        printf("Task 1 time: %d\n", time);
        RTOS_BPS_DelayMs(100);
    }
}

void Task2(void *p_arg) {
    while (1) {
        uint32_t time = BSP_Timer_GetTicksElapsed();
        printf("Task 2 time: %d\n", time);
        for (int i = 0;i<100000;i++) {
            RTOS_BPS_DelayUs(1);
        }

    }
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    BSP_Timer_Init();
    OSInit(&err);
    assertOSError(err);
    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                DEFAULT_STACK_SIZE);
    assertOSError(err);
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                1,
                Task2_Stk,
                DEFAULT_STACK_SIZE);
    assertOSError(err);
}