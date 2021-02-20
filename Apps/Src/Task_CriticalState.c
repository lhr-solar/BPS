/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "os.h"
#include "config.h"
#include "BSP_Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "BSP_UART.h"
#include "Voltage.h"


void Task_CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;
    CANMSG_t CANMSG;
    CANPayload_t CanPayload; 
    CanPayload.idx    = 0;
    CanPayload.data.b = 1;

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    while(OSSemPend(&SafetyCheck_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err) != 4);
    // Turn Contactor On
    BSP_Contactor_On();
    // Push All Clear message to CAN Q
    CANMSG.id = ALL_CLEAR;
    CANMSG.payload = CanPayload;
    OSQPost(&CANBus_MsgQ, &CANMSG, 1, OS_OPT_POST_FIFO, &err);
    // Push Contactor State message to CAN Q
    CANMSG.id = CONTACTOR_STATE;
    OSQPost(&CANBus_MsgQ, &CANMSG, 1, OS_OPT_POST_FIFO, &err);
    OSTaskDel(NULL, &err); // Delete task
    
}
