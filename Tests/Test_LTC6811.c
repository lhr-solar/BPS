/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "Tasks.h"
#include "config.h"
#include "LTC6811.h"
#include "BSP_UART.h"
#include "BSP_SPI.h"
#include "LTC6811.h"
#include "BSP_PLL.h"
#include "BSP_Lights.h"
#include "BSP_Timer.h"
#include "stm32f4xx.h"

// Cell channels to measure (binary)
// 	000 : All cells
//	001 : Cell 1 and 7
//	010 : Cell 2 and 8
//	011 : Cell 3 and 9
//	100 : Cell 4 and 10
//	101 : Cell 5 and 11
//	110 : Cell 6 and 12
#define CELL_CHANNELS 000

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

    while(1) {
        for(int mes = 0; mes <= 6; mes++){ // iteratures through each option of cell channels to sample
            LTC6811_wrcfg(NUM_MINIONS, minions);

            wakeup_sleep(NUM_MINIONS);
            int8_t pec = LTC6811_rdcfg(NUM_MINIONS, minions);
            printf("pec: %d\n", pec);

            RTOS_BPS_DelayTick(15);

            LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP, mes);

            LTC6811_rdcv_safe(0, NUM_MINIONS, minions);
            // printf("Minion %d\r\n", i);
            printf("\tVoltages:\r\n");

            printf("Completed Sampling!\r\n");
        }
    }
}

int main() {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();
    BSP_Timer_Init();

    __disable_irq();

    OSInit(&err);
    CPU_Init();
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);

    __enable_irq();

    OSStart(&err);
}
