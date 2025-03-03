/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "config.h"
#include "Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_PLL.h"
#include "Charge.h"

void Task_CheckContactor(void *p_arg) {
    (void)p_arg;
    
    // buffers for CAN messages
    // static to avoid stack allocation
    static CANMSG_t all_clear =         {.id = BPS_ALL_CLEAR, .payload.data.b = 1};
    static CANMSG_t contactor_state =   {.id = BPS_CONTACTOR_STATE};
    static CANMSG_t recv;

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

    // Initialize state of charge from measured open-circuit voltage before we close the contactors
    Charge_Init();

    // Turn Contactor On
    Contactor_On(HVHIGH_CONTACTOR);
    Contactor_On(HVLOW_CONTACTOR);

    // Push All Clear message to CAN Queue
    CAN_TransmitQueue_Post(all_clear);

    uint32_t controls_no_msg = 0;

    while (1) {
        // delay of 200ms
        // controls IO_STATE message every ~250ms, so we need to check for the 
        // message more frequently to ensure our CAN queue doesn't fill. 
        // 200ms is chosen because it's a neat number and should be fast enough. 
        RTOS_BPS_DelayMs(200);

        // fault if the contactor is open -- this should only happen if ESTOP is hit
        if (Contactor_GetState(HVHIGH_CONTACTOR) != true) {
            Fault_BitMap |= Fault_ESTOP;
            EnterFaultState();
        }

        // Turn on/off array contactor based on what we receive from controls
        // if we get to this point and there's no message we try again ~200ms later
        ErrorStatus status = CAN_ReceiveQueue_Pend(&recv);
        if (status == SUCCESS && recv.id == IO_STATE) {
            controls_no_msg = 0;
            uint8_t array_state = (recv.payload.data.bytes[3] >> 2) & 0x1;
            if (array_state) {
                Contactor_On(ARRAY_CONTACTOR);
            } else {
                Contactor_Off(ARRAY_CONTACTOR);
            }
        } else if (status != SUCCESS && controls_no_msg >= 25) {
            Contactor_On(ARRAY_CONTACTOR);
        } 
        else if (status != SUCCESS) {
            controls_no_msg += 1;
        }

        //Send BPS contactor state via CAN
        contactor_state.payload.data.b = (Contactor_GetState(HVHIGH_CONTACTOR) << 2) |
                                         (Contactor_GetState(HVLOW_CONTACTOR) << 1) |
                                          Contactor_GetState(ARRAY_CONTACTOR);
        CAN_TransmitQueue_Post(contactor_state);
    }
}

