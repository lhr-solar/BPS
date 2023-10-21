/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef CANBUS_H
#define CANBUS_H

#include "common.h"
#include "config.h"
#include "CANMetaData.h"

/**
 * @brief   Initializes the CAN system
 * @param   loopback	: if we should use loopback mode (for testing)	
 * @param   faultState  : fault state being true disables Tx and Rx interrupts 
 * @return  None
 */
void CANbus_Init(bool loopback, bool faultState);

/**
 * @brief   Deitializes the CAN system 
 * @return  None
 */
void CANbus_DeInit();

/**
 * @brief   Transmits data onto the CANbus. This is non-blocking and will fail with an error if
 * 			the CAN mailboxes are fully occupied. Be sure to check the return code or call CANbus_BlockAndSend.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if data wasn't sent, otherwise it was sent.
 */
ErrorStatus CANbus_Send(CANID_t id, CANPayload_t payload);

/**
 * @brief   Transmits data onto the CANbus. If there are no mailboxes available,
 *          this will put the thread to sleep until there are.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if error, SUCCESS otherwise
 */
ErrorStatus CANbus_BlockAndSend(CANID_t id, CANPayload_t payload);

/**
 * @brief   Transmits data onto the CANbus without mailbox semaphores.
 * @param   id : CAN id of the message
 * @param   payload : the data that will be sent.
 * @return  ERROR if error, SUCCESS otherwise
 */
ErrorStatus CANbus_SendMsg_FaultState(CANID_t id, CANPayload_t payload);

/**
 * @brief   Receives data from the CAN bus. This is a non-blocking operation.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was no message, SUCCESS otherwise.
 */
ErrorStatus CANbus_Receive(CANID_t *id, uint8_t *buffer);

/**
 * @brief   Waits for data to arrive.
 * @param   id : pointer to id variable
 * @param   buffer : pointer to payload buffer
 * @return  ERROR if there was an error, SUCCESS otherwise.
 */
ErrorStatus CANbus_WaitToReceive(CANID_t *id, uint8_t *buffer);

#endif
