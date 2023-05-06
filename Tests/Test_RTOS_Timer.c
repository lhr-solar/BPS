/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"
#include "BSP_Timer.h"
#include "RTOS_BPS.h"

/**
 * Test Procedure:
 * 
 * Task1 and Task2 have different priorities with prio(Task1) > prio(Task2). 
 * We want to test proper suspending of the RTOS with fine-grained RTOS delays. 
 * 
 * If the timer delay is working correctly the two tasks will execute (blink lights) 
 * sequentially rather than concurrently.
 * 
 * Note that in actual deployment RTOS_BPS_DelayUs should never be used for large delay 
 * values (> 1 RTOS tick)
 */

#define LIGHT_DELAY_MILLISECONDS        500

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];


void Task1(void *p_arg){

    for (;;) {
        BSP_Light_Toggle(RUN);

        RTOS_BPS_DelayUs(LIGHT_DELAY_MILLISECONDS * 1000);

        // the OS timer still runs when RTOS_BPS_DelayUs is running 
        // so we must delay by a lot of ticks to catch up
        RTOS_BPS_DelayTick(LIGHT_DELAY_MILLISECONDS / 10 + 1);
    }
}

void Task2(void *p_arg){

    for (;;) {
        BSP_Light_Toggle(UVOLT);

        RTOS_BPS_DelayUs(LIGHT_DELAY_MILLISECONDS * 1000);

        RTOS_BPS_DelayTick(LIGHT_DELAY_MILLISECONDS / 10 + 1);
    }
}

int main() {
    OS_ERR err;

    BSP_PLL_Init();
    BSP_Lights_Init();
    BSP_Timer_Init();

    OSInit(&err);
    assertOSError(err);

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    // create two tasks with the same priority

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);

    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                5,
                Task2_Stk,
                256);

    OSStart(&err);
}
