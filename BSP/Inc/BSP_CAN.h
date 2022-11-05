/* Copyright (c) 2022 UT Longhorn Racing Solar */

#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "common.h"
#include "config.h"

/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   rxEvent     : the function to execute when recieving a message. NULL for no action.
 * @param   txEnd       : the function to execute after transmitting a message. NULL for no action.
 * @param   loopback    : if we should use loopback mode (for testing)
 * @return  None
 */
void BSP_CAN_Init(callback_t rxEvent, callback_t txEnd, bool loopback);

/**
 * @brief   Transmits the data onto the CAN bus with the specified id
 * @param   id : Message of ID. Also indicates the priority of message. The lower the value, the higher the priority.
 * @param   data : data to be transmitted. The max is 8 bytes.
 * @param   length : num of bytes of data to be transmitted. This must be <= 8 bytes or else the rest of the message is dropped.
 * @return  ERROR if module was unable to transmit the data onto the CAN bus. SUCCESS indicates data was transmitted.
 */
ErrorStatus BSP_CAN_Write(uint32_t id, uint8_t data[8], uint8_t length);

/**
 * @brief   Gets the data that was received from the CAN bus.
 * @note    Non-blocking statement
 * @pre     The data parameter must be at least 8 bytes or hardfault may occur.
 * @param   id : pointer to store id of the message that was received.
 * @param   data : pointer to store data that was received. Must be 8bytes or bigger.
 * @return  ERROR if nothing was received so ignore id and data that was received. SUCCESS indicates data was received and stored.
 */
ErrorStatus BSP_CAN_Read(uint32_t *id, uint8_t *data);

#endif
