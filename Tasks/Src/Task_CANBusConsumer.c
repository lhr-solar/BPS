/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"


void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    CANMSG_t message;
    uint8_t rxBuffer[8];
    CANId_t id;

    CANbus_Init((bool) p_arg);
    
    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        // Consume messages from other tasks and output to CAN
        CAN_Queue_Pend(&message);
        CANbus_BlockAndSend(message.id, message.payload);

        // Consume messages from CAN
        while (CANbus_Receive(&id, rxBuffer)) {
            // if message is fault state from controls, trigger fault state  
		    // Fault_BitMap |= Fault_EXT;
            // RTOS_BPS_SemPost(&Fault_Sem4, OS_OPT_POST_1);
        }
    }
}

