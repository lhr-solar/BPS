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

    // uint16_t filter_ids[] = {IO_STATE};
    // CANbus_Init((bool) p_arg, false, filter_ids, sizeof(filter_ids) / sizeof(*filter_ids));
    CANbus_Init((bool) p_arg, false, NULL, 0);
    
    while(1) {
      // BLOCKING =====================
      // Wait for CAN Bus to have message
      CANbus_WaitToReceive(&CANMsg.id, &CANMsg.payload);
      CAN_ReceiveQueue_Post(CANMsg);
    }
}