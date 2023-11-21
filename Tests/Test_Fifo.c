/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "Tasks.h"
#include "stdint.h"
#include "stm32f4xx.h"
#include "BSP_UART.h"
#include "BSP_PLL.h"
#include "BSP_Lights.h"

#define FIFO_TYPE int
#define FIFO_SIZE (128)
#define FIFO_NAME Test_Fifo
#include "fifo.h"
static Test_Fifo_t testFifo;

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

void test(){
    //Check empty Fifo
    int out = Test_Fifo_is_empty(&testFifo);
    printf("Test 1 expected output: 1, Received: %u", out);

    out = Test_Fifo_is_full(&testFifo);
    printf("Test 2 expected output: 0, Received: %u", out);

    //Test partially filled Fifo
    Test_Fifo_put(&testFifo, 1);
    Test_Fifo_put(&testFifo, 2);
    Test_Fifo_put(&testFifo, 3);

    out = Test_Fifo_is_empty(&testFifo);
    printf("Test 3 expected output: 0, Received: %u", out);

    out = Test_Fifo_is_full(&testFifo);
    printf("Test 4 expected output: 0, Received: %u", out);

    Test_Fifo_get(&testFifo, &out);
    printf("Test 5 expected output: 3, Received: %u", out);

    Test_Fifo_peek(&testFifo, &out);
    printf("Test 6 expected output: 2, Received: %u", out);

    Test_Fifo_popback(&testFifo, &out);
    printf("Test 7 expected output: 2, Received: %u", out);

    out = Test_Fifo_space(&testFifo);
    printf("Test 8 expected output: 127, Received: %u", out);

    //Fill the Fifo
    for(uint8_t i = 0; i < 127; i++){
        Test_Fifo_put(&testFifo, i);
    }

    out = Test_Fifo_is_full(&testFifo);
    printf("Test 9 expected output: 1, Received: %u", out);

    out = Test_Fifo_space(&testFifo);
    printf("Test 10 expected output: 0, Received: %u", out);
}

// Task for this test
void Task1(void *p_arg){
	OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
    BSP_UART_Init(NULL, NULL, UART_USB);
    Test_Fifo_new(&testFifo);
    test();
}

int main(void){
    OS_ERR err;
    BSP_PLL_Init();
    BSP_Lights_Init();

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);
    assertOSError(err);

    OSStart(&err);
}
