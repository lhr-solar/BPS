#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_UART.h"
#include "stm32f4xx.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

OS_SEM SafetyCheck_Sem4;

void Task1(void *p_arg);
void Task2(void *p_arg);

void Task1(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;
    
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    BSP_UART_Init(NULL, NULL, UART_USB);
    printf("Checkpoint A\n");

    OSTimeDly(1, OS_OPT_TIME_DLY, &err);
    while(err != OS_ERR_NONE);

    // Create Task 2
    RTOS_BPS_TaskCreate(&Task2_TCB,
                "Task 2",
                Task2,
                (void *)0,
                2,
                Task2_Stk,
                256);
    while(err != OS_ERR_NONE);

    while(1) {
        for(int i = 0; i < 4; i++) {
            OSSemPend(&SafetyCheck_Sem4,
                      0,
                      OS_OPT_PEND_BLOCKING,
                      &ts,
                      &err);
        }

        OSTimeDly(1,
                OS_OPT_TIME_DLY,
                &err);
        printf("1\r\n");
    }
}

void Task2(void *p_arg) {
    (void)p_arg;
    
    OS_ERR err;

    while(1) {
        OSSemPost(&SafetyCheck_Sem4,
                    OS_OPT_POST_ALL,
                    &err);

        OSTimeDly(1,
                OS_OPT_TIME_DLY,
                &err);

        printf("2\r\n");
    }
}

int main(void) {
    OS_ERR err;

    __disable_irq();

    OSInit(&err);
    while(err != OS_ERR_NONE);

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0,
                &err);

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
