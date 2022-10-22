/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "LTC6811.h"
#include "BSP_UART.h"
#include "common.h"
#include "config.h"
#include "BSP_SPI.h"
#include "BSP_UART.h"
#include "LTC6811.h"
#include "BSP_OS.h"
#include "BSP_PLL.h"
#include "BSP_Lights.h"
#include "stm32f4xx.h"

cell_asic minions[NUM_MINIONS];
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

void Task1(void *p_arg){
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    BSP_UART_Init(NULL, NULL, UART_USB);    // Initialize printf
    BSP_Lights_Init();

    printf("Testing LTC6811 functions.\r\n");

    wakeup_sleep(NUM_MINIONS);
    LTC6811_Init(minions);

    LTC6811_wrcfg(NUM_MINIONS, minions);

    wakeup_sleep(NUM_MINIONS);
    int8_t pec = LTC6811_rdcfg(NUM_MINIONS, minions);
    printf("pec: %d\n", pec);

    OS_ERR err;
    OSTimeDly(15, OS_OPT_TIME_DLY, &err);

    LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);

    LTC6811_rdcv_safe(0, NUM_MINIONS, minions);

    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("Minion %d:\r\n", i);
        printf("\tVoltages:\r\n");

        for(int j = 0; j < 12; j++) {
            printf("\t%d: %dmV\r\n", j, minions[i].cells.c_codes[j]/*/10*/);
        }
    }

    while(1) {

    }
}

int main() {

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

    __enable_irq();

    OSStart(&err);
}
