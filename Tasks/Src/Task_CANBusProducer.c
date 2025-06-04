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

    CANMSG_t CANMsg;

    CANbus_Init((bool) p_arg, false);
    
    while(1) {
      // BLOCKING =====================
      // Wait for CAN Bus to have any message
      // CANmsg will be filled with the id and payload when a new message is received
      CANbus_WaitToReceive(&CANMsg.id, &CANMsg.payload);

      // Place this into the CAN queue, where the recieve entries array will be updated
      CAN_ReceiveQueue_Post(CANMsg);
    }
}