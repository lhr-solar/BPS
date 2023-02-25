/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "BSP_UART.h"
#include "Voltage.h"
#include "CAN_Queue.h"

void Task_CheckContactor(void *p_arg) {
    (void)p_arg;
    
    CANMSG_t CANMSG;
    CANPayload_t CanPayload; 
    CanPayload.idx    = 0;
    CanPayload.data.b = 1;

    // If a contactor is on before we turn it on in this task, it may have failed and welded shut
    if(Contactor_GetState(HVHIGH_CONTACTOR) || Contactor_GetState(HVLOW_CONTACTOR)) {
        Fault_BitMap |= Fault_ESTOP;
        EnterFaultState();
    } else {
        RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
    }

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, contactors, and current(Amperes) are all checked and safe
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

    // Push All Clear message to CAN Q
    CANMSG.id = ALL_CLEAR;
    CANMSG.payload = CanPayload;
    CAN_Queue_Post(CANMSG);
    // Push Contactor State message to CAN Q
    CANMSG.id = CONTACTOR_STATE;
    CAN_Queue_Post(CANMSG);

    while(1) {
        //delay of 250ms
        RTOS_BPS_DelayMs(250);

        // fault if the contactor is open
        if (Contactor_GetState(HVHIGH_CONTACTOR) != true) {
            Fault_BitMap |= Fault_ESTOP;
            EnterFaultState();
        }
    }
}

