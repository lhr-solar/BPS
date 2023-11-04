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
    
    while(1) {
      
      
      CANMSG_t CANMsg = {0};
      CANbus_WaitToReceive(&CANMsg.id, CANMsg.payload.data.bytes);
      CANMsg.payload.idx = CanMetadataLUT[CANMsg.id].idx_used;

      CAN_ReceiveQueue_Post(CANMsg);

    }
}