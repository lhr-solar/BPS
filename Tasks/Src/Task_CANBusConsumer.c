/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"
#include "config.h"
#include "inttypes.h"

#if BPS_CAN_MIRROR_OUTPUT_USB
#include "BSP_UART.h"

char can_msg_str_buf[128];
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

        uint32_t len = snprintf(
                can_msg_str_buf, sizeof(can_msg_str_buf), 
                "(CAN) id:0x%03" PRIx16 ", idx:%" PRIu16 ", msg:[ ",
                message.id, 
                message.payload.idx);
        for (uint32_t i = 0; i < 8; i++) {
            len += snprintf(
                    can_msg_str_buf + len, sizeof(can_msg_str_buf) - len, 
                    "0x%02" PRIx8 " ", 
                    message.payload.data.bytes[i]);
        }
        len += snprintf(can_msg_str_buf + len, sizeof(can_msg_str_buf) - len, "]\n\r");

        BSP_UART_Write(can_msg_str_buf, len, UART_USB);

        #endif
    }
}
