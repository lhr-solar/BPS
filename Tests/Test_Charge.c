#include "common.h"
#include "config.h"
#include "Charge.h"
#include "BSP_Timer.h"
#include "BSP_UART.h"

#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];
uint32_t charge_reading;

void Task1(void *p_arg){
    BSP_UART_Init(NULL, NULL, UART_USB);
    Charge_Init();
    Charge_SetAccum(50000);
    OS_ERR err;
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
   
    while(1) {
        printf("Charge after 5Amps for 100 milliseconds : %ld\n\r", charge_reading);
        Charge_Calculate(5000);
        charge_reading = Charge_GetPercent();
        RTOS_BPS_TimeDelay(10);
        assertOSError(err);
    }

    exit(0);
}

int main(void){
    
    

    OS_ERR err;
    BSP_PLL_Init();

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
