/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_PLL.h"

void Task_CheckContactor(void *p_arg) {
    (void)p_arg;
    
    CANMSG_t SendCanMsg = {.payload = {.idx = 0, .data.b = 1}};
    CANMSG_t ReceiveCanMsg = {.payload = {.idx = 0, .data.b = 0}, .id = ARRAY_CONTACTOR_STATE_CHANGE};

    BSP_PLL_DelayMs(30); // delay is needed for pull up resistor to stabilize before we check for contactor state
    // If a contactor is on before we turn it on in this task, it may have failed and welded closed
    if (Contactor_GetState(HVHIGH_CONTACTOR) || Contactor_GetState(HVLOW_CONTACTOR)) {
        Fault_BitMap |= Fault_ESTOP;
        EnterFaultState();
    }

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for (uint8_t i = 0; i < NUM_FAULT_POINTS; i++){
        RTOS_BPS_SemPend(&SafetyCheck_Sem4,OS_OPT_PEND_BLOCKING);
    }

    // Turn Contactor On
    Contactor_On(HVHIGH_CONTACTOR);
    Contactor_On(ARRAY_CONTACTOR);
    Contactor_On(HVLOW_CONTACTOR);

    // Push All Clear message to CAN Q
    SendCanMsg.id = BPS_ALL_CLEAR;
    CAN_TransmitQueue_Post(SendCanMsg);

    //Set send can ID to BPS_CONTACTOR_STATE now
    SendCanMsg.id = BPS_CONTACTOR_STATE;

    while(1) {
        //delay of 250ms
        RTOS_BPS_DelayMs(250);

        // fault if the contactor is open
        if (Contactor_GetState(HVHIGH_CONTACTOR) != true) {
            Fault_BitMap |= Fault_ESTOP;
            EnterFaultState();
        }

        // Turn on/off array contactor based on what we receive from controls
        CAN_ReceiveQueue_Pend(&ReceiveCanMsg);
        if (ReceiveCanMsg.id == ARRAY_CONTACTOR_STATE_CHANGE) {
          if (ReceiveCanMsg.payload.data.b) {
            Contactor_On(ARRAY_CONTACTOR);
          } else {
            Contactor_Off(ARRAY_CONTACTOR);
          }
        }

        //Send BPS contactor state via CAN
        SendCanMsg.payload.data.b = (Contactor_GetState(HVHIGH_CONTACTOR) << 2) + 
                                    (Contactor_GetState(HVLOW_CONTACTOR) << 1) +
                                    Contactor_GetState(ARRAY_CONTACTOR);
        CAN_TransmitQueue_Post(SendCanMsg);
        
        
    }
}

