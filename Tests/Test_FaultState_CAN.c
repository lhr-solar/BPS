#include "common.h"
#include "config.h"
#include "Tasks.h"
#include "BSP_WDTimer.h"
#include "Contactor.h"
#include "BSP_PLL.h"
#include "BSP_UART.h"
#include "EEPROM.h"
#include "Charge.h"
#include "BSP_Lights.h"
#include "BSP_CAN.h"

OS_TCB Init_Task_TCB;
CPU_STK Init_Task_Stk[TASK_FAULT_STATE_STACK_SIZE];
OS_TCB Init_Fault_Task_TCB;
CPU_STK Init_Fault_Stk[TASK_FAULT_STATE_STACK_SIZE];
OS_TCB Init_CAN_Task_TCB;
CPU_STK Init_CAN_Task_Stk[TASK_FAULT_STATE_STACK_SIZE];
OS_TCB Init_Fault_Semaphore_Task_TCB;
CPU_STK Init_Fault_Semaphore_Stk[TASK_FAULT_STATE_STACK_SIZE];

//Activate Semaphore
void Task3(void *p_arg){

    //Send messages before and after]
    OS_ERR err;

    OSTimeDly(250, OS_OPT_TIME_DLY, &err);

    OSSemPost(&Fault_Sem4,
        OS_OPT_POST_1,
        &err);
    assertOSError(err);

    while(1){
        BSP_Light_Toggle(EXTRA);
        OSTimeDly(25, OS_OPT_TIME_DLY, &err);
    }

    OSTaskDel(NULL, &err);
}

void Task2(void *p_arg){
    uint8_t message[8] = {0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08};
  
    uint8_t length = 8;
    BSP_CAN_Init(NULL, NULL, true);
   
    OS_ERR err;

    for(int i = 0; i < 3; i++){
       BSP_CAN_Write(0x10A, message, length);
       OSTimeDly(25, OS_OPT_TIME_DLY, &err);
    }
}

//Init Task
void Task1(void *p_arg){
    OS_ERR err;

    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);

    OSSemCreate(&Fault_Sem4,
        "Fault/Tripped FaultState_TCSemaphore",
        0,
        &err);
    assertOSError(err);

    OSTaskCreate(&Init_Fault_Task_TCB,				// TCB
				"INIT_FAULT_STATE_TASK",	// Task Name (String)
				Task_FaultState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_FAULT_STATE_PRIO,			// Priority
				Init_Fault_Stk,			// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_FAULT_STATE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);

    OSTaskCreate(&Init_CAN_Task_TCB,
                "Init_CAN",
                Task2,
                (void *)0,
                TASK_CANBUS_CONSUMER_PRIO,
                Init_CAN_Task_Stk,
                WATERMARK_STACK_LIMIT,
                TASK_FAULT_STATE_STACK_SIZE,
                0,
                10,
                (void *)0,
                OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,
                &err);

    OSTaskCreate(&Init_Fault_Semaphore_Task_TCB,
                "Increment_Fault_Sempahore",
                Task3,
                (void *)0,
                10,
                Init_Fault_Semaphore_Stk,
                16,
                256,
                0,
                0,
                (void *)0,
                OS_OPT_TASK_SAVE_FP | OS_OPT_TASK_STK_CHK,
                &err);

    OSTaskDel(NULL, &err);
}

int main(void) {
    OS_ERR err;

    BSP_PLL_Init();

    //Resetting the contactor
    Contactor_Init();
    Contactor_Off(ALL_CONTACTORS);

    BSP_Lights_Init();

    // // If the WDTimer counts down to 0, then the BPS resets. If BPS has reset, enter a fault state.
    // if (BSP_WDTimer_DidSystemReset()) {
    //     Fault_BitMap = Fault_WDOG; //When function called in if statement, RCC flag cleared so set bitmap here
    //     EnterFaultState();
    // }

    // set up EEPROM and state of charge
    EEPROM_Init();
    Charge_Init();

    __disable_irq();

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Init_Task_TCB,
        "Task 1",
        Task1,
        (void *)0,
        1,
        Init_Task_Stk,
        TASK_FAULT_STATE_STACK_SIZE
    );

    __enable_irq();

    OSStart(&err);
}
