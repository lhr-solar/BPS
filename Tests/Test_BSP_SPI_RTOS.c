/* Copyright (c) 2021 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"
#include "LTC6811.h"
#include "BSP_OS.h"
#include "BSP_PLL.h"
#include "BSP_Lights.h"
#include "stm32f4xx.h"

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

    bsp_os_t spi_os;
    OS_ERR err;
    OSSemCreate(&semaphore,
                "SPI Semaphore",
                0,
                &err);
    spi_os.pend = pend;
    spi_os.post = post;

    BSP_SPI_Init(spi_ltc6811, &spi_os);

    uint8_t data[4+NUM_MINIONS*8] = {0xAB, 0xCD, 0xEF, 0x6E};  

    while(1) {
        BSP_Light_Toggle(FAULT);

        BSP_SPI_Write(spi_ltc6811, data, 4);
        //for (volatile int i = 0; i < 1000000; i++);
        OSTimeDly(15, OS_OPT_TIME_DLY, &err);
    }

    exit(0);
}

// Idle task for this test case
void Task2(void *p_arg){
    //BSP_Lights_Init();
    //BSP_UART_Init(NULL, NULL, UART_USB);
    while(1){
        BSP_Light_Toggle(UVOLT);
        for (volatile int i = 0; i < 1000000; i++);
    }
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();

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

    OSTaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                2,
                Task2_Stk,
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
