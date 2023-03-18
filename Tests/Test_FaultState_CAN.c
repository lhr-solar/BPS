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
CPU_STK Init_Task_Stk[256];
OS_TCB Init_CAN_Task_TCB;
CPU_STK Init_CAN_Task_Stk[256];
OS_TCB Init_Fault_Semaphore_Task_TCB;
CPU_STK Init_Fault_Semaphore_Stk[256];

//Activate Semaphore
void Task3(void *p_arg){

    //Send messages before and after]
    OS_ERR err;

    OSTimeDly(250, OS_OPT_TIME_DLY, &err);

    EnterFaultState();

    while(1){
        BSP_Light_Toggle(EXTRA);
        OSTimeDly(25, OS_OPT_TIME_DLY, &err);
    }

    OSTaskDel(NULL, &err);
}

void Task2(void *p_arg){
    uint8_t message[8] = {0x01, 0x02,0x03,0x04,0x05,0x06,0x07,0x08};
  
    uint8_t length = 8;
    BSP_CAN_Init(NULL, NULL, false, true);
   
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

    RTOS_BPS_TaskCreate(&Init_CAN_Task_TCB,
        "Init_CAN",
        Task2,
        (void *)0,
        TASK_CANBUS_CONSUMER_PRIO,
        Init_CAN_Task_Stk,
        256
    );

    RTOS_BPS_TaskCreate(&Init_Fault_Semaphore_Task_TCB,
        "Increment_Fault_Sempahore",
        Task3,
        (void *)0,
        10,
        Init_Fault_Semaphore_Stk,
        256
    );

    OSTaskDel(NULL, &err);
}

int main(void) {
    OS_ERR err;

    BSP_PLL_Init();

    BSP_Lights_Init();

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
        256
    );

    __enable_irq();

    OSStart(&err);
}
