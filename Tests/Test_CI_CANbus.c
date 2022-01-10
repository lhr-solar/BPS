/* Copyright (c) 2022 UT Longhorn Racing Solar */

// continuous integration test for CANbus driver

#include "common.h"
#include "CANbus.h"
#include "os.h"
#include "Tasks.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

const CANId_t messages[] = {
    TRIP,
    ALL_CLEAR,
    CONTACTOR_STATE,
    CURRENT_DATA,
    VOLT_DATA,
    TEMP_DATA,
    SOC_DATA,
    WDOG_TRIGGERED,
    CAN_ERROR,
    CHARGE_ENABLE
};

const CANPayload_t payloads[] = {
    {.idx = 0, .data = {.w = 0}},
    {.idx = 0, .data = {.w = 0}},
    {.idx = 0, .data = {.b = 1}},
    {.idx = 0, .data = {.w = 0x1234}},
    {.idx = 3, .data = {.w = 0x2000}},
    {.idx = 7, .data = {.w = 0x6789}},
    {.idx = 0, .data = {.w = 0xBEEF}},
    {.idx = 0, .data = {.w = 0x5555}},
    {.idx = 0, .data = {.b = 0}},
    {.idx = 0, .data = {.b = 1}},
};

void Task1(void *p_arg){
    CANbus_Init(false);
   
    for (size_t i = 0; i < sizeof(messages) / sizeof(messages[0]); ++i) {
        CANbus_BlockAndSend(messages[i], payloads[i]);
    }

    for (size_t i = 0; i < sizeof(messages) / sizeof(messages[0]); ++i) {
        CANbus_Send(messages[i], payloads[i]);
    }

    exit(0);
}

int main(void) {
    OS_ERR err;

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

    OSStart(&err);
}
