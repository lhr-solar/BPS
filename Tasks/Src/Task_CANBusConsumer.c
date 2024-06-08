/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"
#include "config.h"
#include "inttypes.h"

#if !BPS_CAN_MIRROR_OUTPUT_USB

#pragma message("CAN mirroring over UART is disabled (default). To enable, set BPS_CAN_MIRROR_OUTPUT_USB to 'true' when building: 'make DEFINES=\"BPS_CAN_MIRROR_OUTPUT_USB=true\"'")

#else   // BPS_CAN_MIRROR_OUTPUT_USB enabled

#pragma message("CAN mirroring over UART is enabled. To disable, set BPS_CAN_MIRROR_OUTPUT_USB to 'false' when building: 'make DEFINES=\"BPS_CAN_MIRROR_OUTPUT_USB=false\"'")

#include "Print_Queue.h"

// CAN mirroring -- globals
static char can_str_buf[40];
static const char hex_lut[] = "0123456789ABCDEF";

// CAN mirroring -- helper functions

static inline void byte_to_hex(uint8_t byte, char *buf) {
    buf[0] = hex_lut[(byte >> 4) & 0x0F];
    buf[1] = hex_lut[byte & 0x0F];
}

static void can_to_string(CANMSG_t *msg, char *buf) {
    // ID
    buf[0] = hex_lut[(msg->id >> 8) & 0x0F];
    buf[1] = hex_lut[(msg->id >> 4) & 0x0F];
    buf[2] = hex_lut[msg->id & 0x0F];
    buf[3] = ' ';

    // idx
    byte_to_hex(msg->payload.idx, &buf[4]);
    buf[6] = ' ';

    // data
    for (int i = 0; i < 8; i++) {
        byte_to_hex(msg->payload.data.bytes[i], &buf[7 + i * 2]);
    }
    
    // term
    buf[23] = '\n';
    buf[24] = '\0';
}

#endif  // BPS_CAN_MIRROR_OUTPUT_USB

void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    CANMSG_t message;

    while (!CANbus_IsInitialized());    // initialized by producer.

    #if BPS_CAN_MIRROR_OUTPUT_USB // Send CAN message to USB
    printf("(CAN) CAN send mirroring enabled\n\r");
    #endif
    
    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message
        CAN_TransmitQueue_Pend(&message);
        CANbus_BlockAndSend(message.id, message.payload);

        #if BPS_CAN_MIRROR_OUTPUT_USB // Send CAN message to USB

        can_to_string(&message, can_str_buf);
        PQ_Write(can_str_buf, 24);

        #endif
    }
}
