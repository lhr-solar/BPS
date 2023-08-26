#include "os.h"
#include "Tasks.h"
#include "stm32f4xx.h"
#include "BSP_PLL.h"

// Used by Task1
OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Print_Task_TCB;
CPU_STK Print_Task_Stk[DEFAULT_STACK_SIZE];

void Task_Print();

void Task1(void *p_arg) {
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    // Spawn CANBUS Consumer, PRIO 7
    RTOS_BPS_TaskCreate(&Print_Task_TCB,				// TCB
            "TASK_Print",	// Task Name (String)
            Task_Print,			// Task function pointer
            (void *)0,
            6,			// Priority
            Print_Task_Stk,	// Watermark limit for debugging
            DEFAULT_STACK_SIZE);
}

int main(void) {
    OS_ERR err;
    BSP_PLL_Init();

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

