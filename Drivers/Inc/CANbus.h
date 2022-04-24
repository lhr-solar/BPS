/* Copyright (c) 2022 UT Longhorn Racing Solar */

#ifndef CANBUS_H
#define CANBUS_H

#include "common.h"
#include "config.h"

//Enum for ID's of all messages that can be sent across CAN bus
typedef enum {
    TRIP = 0x02,
    ALL_CLEAR = 0x101,
    CONTACTOR_STATE = 0x102,
    CURRENT_DATA = 0x103,
    VOLT_DATA = 0x104,
    TEMP_DATA = 0x105,
    SOC_DATA = 0x106,
    WDOG_TRIGGERED = 0x107,
    CAN_ERROR = 0x108,
    CHARGE_ENABLE = 0x10C
} CANId_t;

//Union of data that can be sent across CAN bus. Only one field must be filled out
typedef union {
	uint8_t b;
	uint16_t h;
	uint32_t w;
	float f;
} CANData_t;

/**
 * @note    idx is only used when an array of data needs to be sent.
 * @note    data is a union so only one of the fields should be filled out or bad stuff will happen.
 */
typedef struct {
	uint8_t idx : 8;
	CANData_t data;
} CANPayload_t;

/*This data type is used to push messages onto the queue*/
typedef struct {
    CANId_t id;
    CANPayload_t payload;
}CANMSG_t;

/**
 * @brief   Initializes the CAN system
 * @param   loopback	: if we should use loopback mode (for testing)	
 * @return  None
 */
void CANbus_Init(bool loopback);

/**
 * @brief   Transmits data onto the CANbus. This is non-blocking and will fail with an error if
 * 			the CAN mailboxes are fully occupied. Be sure to check the return code or call CANbus_BlockAndSend.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if data wasn't sent, otherwise it was sent.
 */
ErrorStatus CANbus_Send(CANId_t id, CANPayload_t payload);

/**
 * @brief   Transmits data onto the CANbus. If there are no mailboxes available,
 *          this will put the thread to sleep until there are.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if error, SUCCESS otherwise
 */
ErrorStatus CANbus_BlockAndSend(CANId_t id, CANPayload_t payload);

/**
 * @brief   Receives data from the CAN bus. This is a non-blocking operation.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was no message, SUCCESS otherwise.
 */
ErrorStatus CANbus_Receive(CANId_t *id, uint8_t *buffer);

/**
 * @brief   Waits for data to arrive.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was an error, SUCCESS otherwise.
 */
ErrorStatus CANbus_WaitToReceive(CANId_t *id, uint8_t *buffer);

#endif
