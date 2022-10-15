/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_CAN.h"
#include "Simulator.h"
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

static bool CAN_Initialized = false;
// User parameters for CAN events
static void (*gRxEvent)(void);
static void (*gTxEnd)(void);

/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   None
 * @return  None
 */
void BSP_CAN_Init(callback_t rxEvent, callback_t txEnd, bool loopback) {
    gTxEnd = txEnd;
    gRxEvent = rxEvent;
    CAN_Initialized = true;
    Simulator_log("CAN Initialized\n");
}



/**
 * @brief   Transmits the data onto the CAN bus with the specified id
 * @param   id : Message of ID. Also indicates the priority of message. The lower the value, the higher the priority.
 * @param   data : data to be transmitted. The max is 8 bytes.
 * @param   length : num of bytes of data to be transmitted. This must be <= 8 bytes or else the rest of the message is dropped.
 * @return  ERROR if module was unable to transmit the data onto the CAN bus. SUCCESS indicates data was transmitted.
 */
ErrorStatus BSP_CAN_Write(uint32_t id, uint8_t data[], uint8_t length) {
    if (CAN_Initialized) {
        if (length > 8) return ERROR;
        // log message
        char canMsgBuf[100] = {0};
        uint64_t val = 0;
        for (int i = 0; i < length; i++) {
            val <<= 8;
            val += data[i];
        }
        sprintf(canMsgBuf, "Writing CAN message with ID [%d], DATA [0x%016" PRIx64 "], LEN [%d]\n", id, val, length);
        Simulator_log(canMsgBuf);
        if (gTxEnd != NULL)
            gTxEnd();
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @brief   Gets the data that was received from the CAN bus.
 * @note    Non-blocking statement
 * @pre     The data parameter must be at least 8 bytes or hardfault may occur.
 * @param   id : pointer to store id of the message that was received.
 * @param   data : pointer to store data that was received. Must be 8bytes or bigger.
 * @return  ERROR if nothing was received so ignore id and data that was received. SUCCESS indicates data was received and stored.
 */
ErrorStatus BSP_CAN_Read(uint32_t *id, uint8_t *data) {
    if (CAN_Initialized) {
        char buffer[75];
        sprintf(buffer, "Read CAN message ID [%d] DATA [0x%016" PRIx64 "]\n", *id, *(uint64_t*)(data));
        Simulator_log(buffer);
        if (gRxEvent != NULL) // so we dont error out and die
            gRxEvent();
    }
    return ERROR;
}
