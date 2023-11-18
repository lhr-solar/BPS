/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"

/**
 * @brief call CANBUS_read and receive data in id/buffer, store into a
 * queue so that other tasks can check can messages and do func based on that
*/
void Task_CANBusProducer(void *p_arg) {
    (void)p_arg;

    CANbus_Init((bool) p_arg, false);
    CANMSG_t CANMsg;
    
    while(1) {
      CANbus_WaitToReceive(&CANMsg.id, &CANMsg.payload);
      CAN_ReceiveQueue_Post(CANMsg);
    }
}