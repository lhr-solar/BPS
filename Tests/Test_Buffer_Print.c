#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"
#include "Print_Queue.h"

// Used by Task1
OS_TCB Init_Task_TCB;
CPU_STK Init_Task_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Spammer_TCB;
CPU_STK Print_Spammer_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Print1_TCB;
CPU_STK Print_Print1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Task_TCB;
CPU_STK Print_Task_Stk[DEFAULT_STACK_SIZE];

void Print_Spammer(){
    while(1){RTOS_BPS_Printf("fuck kartik\n\r");}
}

void Print1(){
    char* str = "nope\n\r";
    char output[10];
    RTOS_BPS_snPrintf(output, sizeof(str), str);
}

void Init_Task(void *p_arg) {
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

        //char* test = "PRINTTASK\n";
        //BSP_UART_Write(test, 11, UART_USB);

    // Spawn CANBUS Consumer, PRIO 7
    RTOS_BPS_TaskCreate(&Print_Spammer_TCB,				// TCB
            "Print_Spammer",	// Task Name (String)
            Print_Spammer,			// Task function pointer
            (void *)0,
            9,			// Priority
            Print_Spammer_Stk,	// Watermark limit for debugging
            DEFAULT_STACK_SIZE);
    
    RTOS_BPS_TaskCreate(&Print_Print1_TCB,
            "Print1",
            Print1,
            (void *)0,
            9,
            Print_Print1_Stk,
            DEFAULT_STACK_SIZE);

    RTOS_BPS_TaskCreate(&Print_Task_TCB,
            "Print_Task",
            Task_Print,
            (void *)0,
            6,
            Print_Task_Stk,
            DEFAULT_STACK_SIZE);
}

void foo(void){
    return;
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();
    BSP_UART_Init(foo, foo, UART_USB);
    Print_Queue_Init();

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Init_Task_TCB,
                "Init_Task",
                Init_Task,
                (void *)0,
                1,
                Init_Task_Stk,
                256);
    assertOSError(err);

    OSStart(&err);
    assertOSError(err);
}

