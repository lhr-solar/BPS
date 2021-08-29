/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "os.h"
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"


void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    CANMSG_t message;

    CANbus_Init((bool) p_arg);
    
    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        CAN_Queue_Pend(&message);
        CANbus_BlockAndSend(message.id, message.payload);
    }
}
