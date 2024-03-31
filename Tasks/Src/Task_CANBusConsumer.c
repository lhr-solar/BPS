/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"
#include "config.h"
#include "inttypes.h"

#if BPS_CAN_MIRROR_OUTPUT_USB
#include "BSP_UART.h"

#endif

void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    CANMSG_t message;

    while (!CANbus_IsInitialized());

    #if BPS_CAN_MIRROR_OUTPUT_USB // Send CAN message to USB
    printf("(CAN) CAN send mirroring enabled\n\r");
    #endif
    
    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        CAN_TransmitQueue_Pend(&message);
        CANbus_BlockAndSend(message.id, message.payload);

        #if BPS_CAN_MIRROR_OUTPUT_USB // Send CAN message to USB

        // TODO: perform bit-wise output to CAN, merge with nonblocking printf PR.

        #endif
    }
}
