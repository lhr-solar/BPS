/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "BSP_UART.h"
#include "Voltage.h"
#include "CAN_Queue.h"


void Task_CriticalState(void *p_arg) {
    (void)p_arg;
    OS_ERR err;
    
    CANMSG_t CANMSG;
    CANPayload_t CanPayload; 
    CanPayload.idx    = 0;
    CanPayload.data.b = 1;

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for (int i = 0; i < NUM_FAULT_POINTS; i++){
        RTOS_BPS_SemPend(&SafetyCheck_Sem4,OS_OPT_PEND_BLOCKING);
    }

    // launch watchdog task
    RTOS_BPS_TaskCreate(&PetWDog_TCB,              // TCB
                "TASK_PETWDOG_PRIO",        // Task Name (String)
                Task_PetWDog,               // Task function pointer
                (void *)0,                  // Task function args
                TASK_PETWDOG_PRIO,          // Priority
                PetWDog_Stk,                // Stack
                TASK_PETWDOG_STACK_SIZE
                );

    // Turn Contactor On
    Contactor_On(HVHIGH_CONTACTOR);
    Contactor_On(ARRAY_CONTACTOR);
    Contactor_On(HVLOW_CONTACTOR);

    // launch check contactor task
    // RTOS_BPS_TaskCreate(&CheckContactor_TCB,                       // TCB
	// 			"Task_CheckContactor",                      // Task Name (String)
	// 			Task_CheckContactor,                        // Task function pointer
	// 			(void *)0,                                  // Task function args
	// 			TASK_CHECK_CONTACTOR_PRIO,                  // Priority
	// 			CheckContactor_Stk,                         // Stack
	// 			TASK_CHECK_CONTACTOR_STACK_SIZE
    //             );

    // Push All Clear message to CAN Q
    CANMSG.id = ALL_CLEAR;
    CANMSG.payload = CanPayload;
    CAN_Queue_Post(CANMSG);
    // Push Contactor State message to CAN Q
    CANMSG.id = CONTACTOR_STATE;
    CAN_Queue_Post(CANMSG);
    OSTaskDel(NULL, &err); // Delete task
}
