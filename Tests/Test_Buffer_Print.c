#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"
#include "Print_Queue.h"

// Used by Task1
OS_TCB Init_Task_TCB;
CPU_STK Init_Task_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Basic_Functionality_TCB;
CPU_STK Print_Basic_Functionality_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Concurrent_Writes_TCB;
CPU_STK Print_Concurrent_Writes_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Overflow_Buffer_TCB;
CPU_STK Print_Overflow_Buffer_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Task_TCB;
CPU_STK Print_Task_Stk[DEFAULT_STACK_SIZE];

void Basic_Functionality(){
    char* str = "nope\n\r";
    RTOS_BPS_snPrintf(str);
}

void Concurrent_Writes(){
    char* str1 = "nope\n\r";
    char* str2 = "yeppers\n\r";
    RTOS_BPS_snPrintf(str1);
    RTOS_BPS_snPrintf(str2);
}

void Overflow_Buffer(){
    char* str1 = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678922222222222222222222222222222222222222222222222222222222";
    RTOS_BPS_snPrintf(str1);
}

void Init_Task(void *p_arg) {
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

        //char* test = "PRINTTASK\n";
        //BSP_UART_Write(test, 11, UART_USB);

    // Spawn CANBUS Consumer, PRIO 7

    RTOS_BPS_TaskCreate(&Print_Basic_Functionality_TCB,     // TCB
            "Basic_Functionality",                          // Task Name (String)
            Basic_Functionality,                            // Task Function Pointer
            (void *)0,                                      
            2,                                              // Priority (lower is more important)
            Print_Basic_Functionality_Stk,                  // Watermark Limit for Debugging
            DEFAULT_STACK_SIZE);
    
    RTOS_BPS_TaskCreate(&Print_Concurrent_Writes_TCB,
            "Concurrent_Writes",
            Concurrent_Writes,
            (void *)0,
            3,
            Print_Concurrent_Writes_Stk,
            DEFAULT_STACK_SIZE);
    
    RTOS_BPS_TaskCreate(&Print_Overflow_Buffer_TCB,
            "Overflow_Buffer",
            Overflow_Buffer,
            (void *)0,
            3,
            Print_Overflow_Buffer_Stk,
            DEFAULT_STACK_SIZE);

    RTOS_BPS_TaskCreate(&Print_Task_TCB,
            "Print_Task",
            Task_Print,
            (void *)0,
            1,
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

