/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

void Task1(void *p_arg){
    CANID_t id = STATE_OF_CHARGE_DATA;
    CANData_t data;
    data.w = 0xBEEF;
    CANPayload_t payload;
    payload.idx = 0;
    payload.data = data;

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    
    BSP_Lights_Init();
    CANbus_Init(true, false, NULL, 0);
   
    while(1) {
        CANbus_BlockAndSend(id, payload);
        BSP_Light_Toggle(EXTRA);
    }

    exit(0);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();

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

    __enable_irq();

    OSStart(&err);
}
