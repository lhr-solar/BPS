/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"
#include "LTC6811.h"
#include "BSP_OS.h"
#include "BSP_PLL.h"
#include "BSP_Lights.h"
#include "stm32f4xx.h"
#include "RTOS_BPS.h"

OS_TCB Task1_TCB, Task2_TCB;
CPU_STK Task1_Stk[256];
CPU_STK Task2_Stk[256];
OS_SEM semaphore;
bool initFlag = false;

void pend(void) {
    OS_ERR err;
    CPU_TS ts;
    OSSemPend(&semaphore,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);
}

void post(void) {
    OS_ERR err;
    //BSP_Light_On(FAULT);
    OSSemPost(&semaphore,
            OS_OPT_PEND_BLOCKING,
            &err);
    //BSP_Light_Toggle(OVOLT);
    //for (volatile int i = 0; i < 1000000; i++);
}

void Task1(void *p_arg){
    BSP_Lights_Init();
    BSP_UART_Init(NULL, NULL, UART_USB);    // Initialize UART to use printf

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    bsp_os_t spi_os;
    OS_ERR err;
    RTOS_BPS_SemCreate(&semaphore,
                "SPI Semaphore",
                0);
    spi_os.pend = pend;
    spi_os.post = post;

    BSP_SPI_Init(spi_ltc2315, &spi_os);
    

    uint8_t data[32/*4+NUM_MINIONS*8*/] = {0xAB, 0xCD, 0xEF, 0x6E, 0x00, 0x00};  

    while(1) {
        BSP_Light_Toggle(FAULT);

        BSP_SPI_Write(spi_ltc2315, data, 27);
        for (volatile int i = 0; i < 1000000; i++);
        // OSTimeDly(15, OS_OPT_TIME_DLY, &err);
    }

    exit(0);
}

// Idle task for this test case
void Task2(void *p_arg){
    __attribute__((unused)) OS_ERR err;
    while(1){
        BSP_Light_Toggle(UVOLT);
        // for (volatile int i = 0; i < 1000000; i++);
        RTOS_BPS_DelayTick(15);
    }
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();

    __disable_irq();

    OSInit(&err);
    while(err != OS_ERR_NONE);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);
    while(err != OS_ERR_NONE);

    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                2,
                Task2_Stk,
                256);
    while(err != OS_ERR_NONE);

    __enable_irq();

    OSStart(&err);
}
