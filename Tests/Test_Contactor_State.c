/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#else
#include "Simulator.h"
#endif
#include "Contactor.h"
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"
#include "BSP_UART.h"

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[256];

OS_TCB SetContactorState_TCB;
CPU_STK SetContactorState_Stk[256];




OS_TCB Task_Read_TCB;
CPU_STK TaskRead_Stk[256];


/******************************************************************************
 * Check Contactor Task Test Plan
 * 
 * 1. Build the BPS code for the stm32f413 for this test file (see README.md)
 * 2. Flash the BPS (see README.md)
 * 3. Run the code in this file. NOTE: This code includes a heartbeat.
 * 4. Make sure the contactor goes from open to closed. Use an ohmmeter to check the resistance across the contactor's terminals to determine the state of the contactor (this resistance should be close to 0 Ohms).
 * 5. Unpower the contactor, while keeping the BPS powered on. Make sure the BPS goes into a fault state
****************************************************************************/
void foo(void){
    return;
}

void SetContactorState(void *p_arg){

    BSP_PLL_DelayMs(30);
    CANMSG_t CanMsg;
    while(1){
        BSP_Light_Toggle(RUN);
        Contactor_On(HVHIGH_CONTACTOR);

        //send can message turning array contactor on and off
        static uint8_t osciallation = 0;
        CanMsg.id = ARRAY_CONTACTOR_STATE_CHANGE;
        CanMsg.payload.data.b = (osciallation) ? 0 : 1;
        CAN_TransmitQueue_Post(CanMsg);

        RTOS_BPS_DelayMs(100);
    }
}

void Task_Read(void *p_arg) {
  CANMSG_t message = {0};


  RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
  RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
  RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
  RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);

  while(1) {

    if(CAN_ReceiveQueue_Pend(&message) == ERROR) {
      //printf("Receive FIFO empty!!! \n\r");
    }
    switch (message.id) {
      case VOLTAGE_DATA_ARRAY:
        printf("ID: Voltage data\n\r");
        break;
      case CHARGING_ENABLED:
        printf("ID: Charging Enabled\n\r");
        break;
      case TEMPERATURE_DATA_ARRAY:
        printf("ID: Temp data\n\r");
        break;
      case ARRAY_CONTACTOR_STATE_CHANGE:  
        printf("ID: Array state changed!!!\n\r");
        if (message.payload.data.b) {
          printf("Array Contactor is on!\n\r");
        } else {
          printf("Array Contactor is off!!\n\r");
        }
        break;
      case BPS_CONTACTOR_STATE:
        printf("Recieved BPS Contactor state information\n\r");
        break;
      default:
        printf("ID: unknown\n\r");
        break;
    }
    printf("CAN message: %x %x %x %x %x %x %x %x\n\r",message.payload.data.bytes[0], 
                                                      message.payload.data.bytes[1],
                                                      message.payload.data.bytes[2],
                                                      message.payload.data.bytes[3],
                                                      message.payload.data.bytes[4],
                                                      message.payload.data.bytes[5],
                                                      message.payload.data.bytes[6],
                                                      message.payload.data.bytes[7]
    );
    // for (int i = 0;i < 8;i++) {
    //   printf("byte %d: %d", i, message.payload.data.bytes[i]);
    // }
    // printf("\n");


    RTOS_BPS_DelayTick(10);
  }
}



void Task1(void *p_arg){
    OS_ERR err;

#ifndef SIMULATION
    OS_CPU_SysTickInit(SystemCoreClock / (CPU_INT32U) OSCfg_TickRate_Hz);
#else
    OS_CPU_SysTickInit();
#endif

    RTOS_BPS_SemCreate(&SafetyCheck_Sem4, "Safety Check Semaphore", 0);

    RTOS_BPS_TaskCreate(&CheckContactor_TCB,    // TCB
				"Task_CheckContactor",          // Task Name (String)
				Task_CheckContactor,            // Task function pointer
				(void *)true,                      // Use loop back mode
				TASK_CHECK_CONTACTOR_PRIO,      // Priority
				CheckContactor_Stk,             // Stack
				TASK_CHECK_CONTACTOR_STACK_SIZE
                );

    RTOS_BPS_TaskCreate(&SetContactorState_TCB,    // TCB
				"Task_SetContactor",          // Task Name (String)
				SetContactorState,            // Task function pointer
				(void *)true,                      // Task function args
				6,      // Priority
				SetContactorState_Stk,             // Stack
				TASK_CHECK_CONTACTOR_STACK_SIZE
                );

    RTOS_BPS_TaskCreate(&Task_Read_TCB, // TCB
            "TASK_READ",
            Task_Read,
            (void *)0,
            5,
            TaskRead_Stk,
            TASK_CHECK_CONTACTOR_STACK_SIZE
            );

    RTOS_BPS_TaskCreate(&CANBusConsumer_TCB,				// TCB
		"TASK_CANBUS_CONSUMER_PRIO",	// Task Name (String)
		Task_CANBusConsumer,				// Task function pointer
		(void *)true,				// use loopback mode
		TASK_CANBUS_CONSUMER_PRIO,			// Priority
		CANBusConsumer_Stk,	// Watermark limit for debugging
		TASK_CANBUS_CONSUMER_STACK_SIZE);

    RTOS_BPS_TaskCreate(&CANBusProducer_TCB,	// TCB
        "TASK_CANBUS_PRODUCER",	            // Task Name (String)
        Task_CANBusProducer,				// Task function pointer
        (void *)true,				        // don't use loopback mode
        TASK_CANBUS_PRODUCER_PRIO,			// Priority
        CANBusProducer_Stk,				    // Stack
        TASK_CANBUS_PRODUCER_STACK_SIZE);

    CAN_Queue_Init();

    OSTaskDel(NULL, &err);
}

#ifdef SIMULATION
int main(int argc, char **argv) {
#else
int main() {
#endif

#ifdef SIMULATION
    // the first command line argument is the path to the JSON file
    Simulator_Init(argv[1]);
#endif

    OS_ERR err;
    BSP_PLL_Init();
    BSP_UART_Init(foo, foo, UART_USB);
    BSP_Lights_Init();
    Contactor_Init();

#ifndef SIMULATION
    __disable_irq();
#endif

    OSInit(&err);
    assertOSError(err);

    RTOS_BPS_TaskCreate(&Task1_TCB,
                "Task 1",
                Task1,
                (void *)0,
                1,
                Task1_Stk,
                256);

#ifndef SIMULATION
    __enable_irq();
#endif

    OSStart(&err);
    assertOSError(err);
}
