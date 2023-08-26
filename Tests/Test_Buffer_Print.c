/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"
#include "Print_Queue.h"

// Used by Task1
OS_TCB Init_Task_TCB;
CPU_STK Init_Task_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Basic_Functionality_Test_TCB;
CPU_STK Print_Basic_Functionality_Test_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Concurrent_Writes_TCB;
CPU_STK Print_Concurrent_Writes_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Overflow_Buffer_TCB;
CPU_STK Print_Overflow_Buffer_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Edge_Case_Buffer_Limit_Test_TCB;
CPU_STK Print_Edge_Case_Buffer_Limit_Test_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Empty_String_Test_TCB;
CPU_STK Print_Empty_String_Test_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Special_Characters_Test_TCB;
CPU_STK Print_Special_Characters_Test_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Performance_Test_TCB;
CPU_STK Print_Performance_Test_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Task_TCB;
CPU_STK Print_Task_Stk[DEFAULT_STACK_SIZE];

/**
 * @brief Tests the basic functionality of the code
 * 
 */

// void Basic_Functionality_Test(){
//     char* str = "Hello, world!\n\r";
//     RTOS_BPS_NonBlocking_Printf(str);
// }

/**
 * @brief Tests how the program behaves if several calls to the print does not cause errors
 * 
 */

// void Concurrent_Writes(){
//     char* str1 = "First\n\r";
//     char* str2 = "Second\n\r";
//     RTOS_BPS_NonBlocking_Printf(str1);
//     RTOS_BPS_NonBlocking_Printf(str2);
// }

/**
 * @brief Tests how the program responds to an overflow of characters sent from the call
 * 
 */

void Overflow_Buffer(){
    // 260
    char* str = "THIS STRING IS 260 CHARACTERS xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n\r";
    // 129
    // char* str2 = "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabczyx";
    // // 128
    // char* str3 = "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabczy";
    // // 127
    // char* str4 = "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcz";
    RTOS_BPS_NonBlocking_Printf(str);

}

/**
 * @brief Tests the behavior of an overflow of the printFifo
 * 
 */

void Edge_Case_Buffer_Limit_Test(){
    char* str1 = "abcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabcabc";
    char* str2 = "abc";
    RTOS_BPS_NonBlocking_Printf(str1);
    RTOS_BPS_NonBlocking_Printf(str2);    
}

/**
 * @brief Tests the behavior of an empty string
 * 
 */

// void Empty_String_Test(){
//     char* str = "";
//     RTOS_BPS_NonBlocking_Printf(str);
// }

/**
 * @brief Tests the behavior of special charaters and symbols, especially a \0
 * 
 */

// void Special_Characters_Test(){
//     char* str = "Special chars: \t \n \r \0 %% & *";
//     RTOS_BPS_NonBlocking_Printf(str);
// }

/**
 * @brief Tests the performance of the code
 * 
 */

// void Performance_Test(){
//     char* formatStr = "Formatted number: %d\n\r";
//     int num = 42;
//     RTOS_BPS_NonBlocking_Printf(formatStr, num);
// }

void Init_Task(void *p_arg) {
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

        //char* test = "PRINTTASK\n";
        //BSP_UART_Write(test, 11, UART_USB);

    // Spawn CANBUS Consumer, PRIO 7
    
    // RTOS_BPS_TaskCreate(&Print_Basic_Functionality_Test_TCB,     // TCB
    //         "Basic_Functionality_Test",                          // Task Name (String)
    //         Basic_Functionality_Test,                            // Task Function Pointer
    //         (void *)0,                                      
    //         2,                                                   // Priority (lower is more important)
    //         Print_Basic_Functionality_Test_Stk,                  // Watermark Limit for Debugging
    //         DEFAULT_STACK_SIZE);
    
    // RTOS_BPS_TaskCreate(&Print_Concurrent_Writes_TCB,
    //         "Concurrent_Writes",
    //         Concurrent_Writes,
    //         (void *)0,
    //         3,
    //         Print_Concurrent_Writes_Stk,
    //         DEFAULT_STACK_SIZE);
    
    // RTOS_BPS_TaskCreate(&Print_Overflow_Buffer_TCB,
    //         "Overflow_Buffer",
    //         Overflow_Buffer,
    //         (void *)0,
    //         4,
    //         Print_Overflow_Buffer_Stk,
    //         DEFAULT_STACK_SIZE);
    
    RTOS_BPS_TaskCreate(&Print_Edge_Case_Buffer_Limit_Test_TCB,
            "Edge_Case_Buffer_Limit_Test",
            Edge_Case_Buffer_Limit_Test,
            (void *)0,
            5,
            Print_Edge_Case_Buffer_Limit_Test_Stk,
            DEFAULT_STACK_SIZE);

    // RTOS_BPS_TaskCreate(&Print_Empty_String_Test_TCB,
    //         "Empty_String_Test",
    //         Empty_String_Test,
    //         (void *)0,
    //         6,
    //         Print_Empty_String_Test_Stk,
    //         DEFAULT_STACK_SIZE);
    
    // RTOS_BPS_TaskCreate(&Print_Special_Characters_Test_TCB,
    //         "Special_Characters_Test",
    //         Special_Characters_Test,
    //         (void *)0,
    //         7,
    //         Print_Special_Characters_Test_Stk,
    //         DEFAULT_STACK_SIZE);
    
    // RTOS_BPS_TaskCreate(&Print_Performance_Test_TCB,
    //         "Performance_Test",
    //         Performance_Test,
    //         (void *)0,
    //         8,
    //         Print_Performance_Test_Stk,
    //         DEFAULT_STACK_SIZE);

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

