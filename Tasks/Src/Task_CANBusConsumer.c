/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Tasks.h"
#include "BSP_CAN.h"
#include "CANbus.h"
#include "CAN_Queue.h"
#include "config.h"

#if !BPS_CAN_MIRROR_OUTPUT_USB

#pragma message("CAN mirroring over UART is disabled (default). To enable, set BPS_CAN_MIRROR_OUTPUT_USB to 'true' when building: 'make DEFINES=\"BPS_CAN_MIRROR_OUTPUT_USB=true\"'")

#else   // BPS_CAN_MIRROR_OUTPUT_USB enabled

#pragma message("CAN mirroring over UART is enabled. To disable, set BPS_CAN_MIRROR_OUTPUT_USB to 'false' when building: 'make DEFINES=\"BPS_CAN_MIRROR_OUTPUT_USB=false\"'")

#include "Print_Queue.h"
#include "PrintCANHelper.h"

// Define some options here
#define CAN_MIRROR_HUMAN_READABLE   false   // prints newline-terminated human-readable hex strings: [ID] [IDX] [DATA] ([CRC])
#define CAN_MIRROR_ONLY_VALID_DATA  true    // truncates can payload to actual length. results in variable length messages.
#define CAN_MIRROR_CRC              true    // appends crc8 to end of message.

#if CAN_MIRROR_HUMAN_READABLE
#define can_to_string(msg, buf) CAN_ToHexString_HumanReadable(msg, buf, CAN_MIRROR_ONLY_VALID_DATA, CAN_MIRROR_CRC)
#else
#define can_to_string(msg, buf) CAN_ToBytes(msg, buf, CAN_MIRROR_ONLY_VALID_DATA, CAN_MIRROR_CRC)
#endif

static char can_str_buf[40];

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

        int len = can_to_string(&message, can_str_buf);
        PQ_Write(can_str_buf, len);

        #endif
    }
}
