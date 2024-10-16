/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include <stdio.h>
#include <stdint.h>
#include "RTOS_BPS.h"
#include "BSP_UART.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"

OS_TCB UART_Deadlocks_TCB;
CPU_STK UART_Deadlocks_Stk[512];

OS_TCB UART_Deadlocks2_TCB;
CPU_STK UART_Deadlocks2_Stk[512];

int counter1 = 0;
int counter2 = 0;

void foo(){
    return;
}

void UART_Deadlocks(void *p_arg){
    (void)p_arg;
    char* str1U = "Printing string 1 in thread 1 to USB";
    char* str1B = "Printing string 1 in thread 1 to BLE";
    char* str2U = "Printing string 2 in thread 1 to USB";
    char* str2B = "Printing string 2 in thread 1 to BLE";
    char* str3U = "Printing string 3 in thread 1 to USB";
    char* str3B = "Printing string 3 in thread 1 to BLE";

    printf("initializing UART...\n");
    BSP_UART_Init(foo, foo, UART_USB);
    BSP_UART_Init(foo, foo, UART_BLE);
    int counter = 0;

    while (1){
        //for(int i = 0; i < 4; i++) {
        RTOS_BPS_SemPend(&SafetyCheck_Sem4, OS_OPT_PEND_BLOCKING);
        //}
        BSP_UART_Write(str1U, 36, UART_USB);
        BSP_UART_ReadLine(str1U, UART_USB);
        printf("%s\n", str1U);
        BSP_UART_Write(str1B, 36, UART_BLE);
        BSP_UART_ReadLine(str1B, UART_BLE);
        printf("%s\n", str1B);
        BSP_UART_Write(str2U, 36, UART_USB);
        BSP_UART_ReadLine(str2U, UART_USB);
        printf("%s\n", str2U);
        BSP_UART_Write(str2B, 36, UART_BLE);
        BSP_UART_ReadLine(str2B, UART_BLE);
        printf("%s\n", str2B);
        BSP_UART_Write(str3U, 36, UART_USB);
        BSP_UART_ReadLine(str3U, UART_USB);
        printf("%s\n", str3U);
        BSP_UART_Write(str3B, 36, UART_BLE);
        BSP_UART_ReadLine(str3B, UART_BLE);
        printf("%s\n", str3B);
        counter++;
        counter1++;
        if (counter >= 1000){
            printf("task 1 ran %d times\ntask 2 ran %d times\n", counter1, counter2);
            exit(0);
        }
    }
}

void UART_Deadlocks2(void *p_arg){
    (void)p_arg;

    char* str1U = "Printing string 1 in thread 2 to USB";
    char* str1B = "Printing string 1 in thread 2 to BLE";
    char* str2U = "Printing string 2 in thread 2 to USB";
    char* str2B = "Printing string 2 in thread 2 to BLE";
    char* str3U = "Printing string 3 in thread 2 to USB";
    char* str3B = "Printing string 3 in thread 2 to BLE";

    printf("initializing UART...\n");

    BSP_UART_Init(foo, foo, UART_USB);
    BSP_UART_Init(foo, foo, UART_BLE);
    int counter = 0;

    while (1){
        RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_ALL);
        BSP_UART_Write(str1U, 36, UART_USB);
        BSP_UART_ReadLine(str1U, UART_USB);
        printf("%s\n", str1U);
        BSP_UART_Write(str1B, 36, UART_BLE);
        BSP_UART_ReadLine(str1B, UART_BLE);
        printf("%s\n", str1B);
        BSP_UART_Write(str2U, 36, UART_USB);
        BSP_UART_ReadLine(str2U, UART_USB);
        printf("%s\n", str2U);
        BSP_UART_Write(str2B, 36, UART_BLE);
        BSP_UART_ReadLine(str2B, UART_BLE);
        printf("%s\n", str2B);
        BSP_UART_Write(str3U, 36, UART_USB);
        BSP_UART_ReadLine(str3U, UART_USB);
        printf("%s\n", str3U);
        BSP_UART_Write(str3B, 36, UART_BLE);
        BSP_UART_ReadLine(str3B, UART_BLE);
        printf("%s\n", str3B);
        counter++;
        counter2++;
        if (counter >= 1000){
            printf("task 1 ran %d times\ntask 2 ran %d times\n", counter1, counter2);
            exit(0);
        }
    }
}


int main() {
    OS_ERR err;
    OSInit(&err);
    RTOS_BPS_TaskCreate(&UART_Deadlocks_TCB,				// TCB
				"UART Deadlocks Test",	// Task Name (String)
				UART_Deadlocks,				// Task function pointer
				(void *)0,				// Task function args
				1,			// Priority
				UART_Deadlocks_Stk,	// Watermark limit for debugging
				512);					// return err code
	// ASSERT err
    RTOS_BPS_TaskCreate(&UART_Deadlocks2_TCB,				// TCB
				"UART Deadlocks Test",	// Task Name (String)
				UART_Deadlocks2,				// Task function pointer
				(void *)0,				// Task function args
				2,			// Priority
				UART_Deadlocks2_Stk,	// Watermark limit for debugging
				512);					// return err code
    // ASSERT err
	OSStart(&err);
}
