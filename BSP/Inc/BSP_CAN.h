/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef BSP_CAN_H
#define BSP_CAN_H

#include "common.h"
#include "config.h"

/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   rxEvent       : the function to execute when recieving a message. NULL for no action.
 * @param   txEnd         : the function to execute after transmitting a message. NULL for no action.
 * @param   faultState    : if we should initialize CAN interrupts
 * @param   loopback      : if we should use loopback mode (for testing)
 * @param   txIDFilter    : array of IDs to accept messages from. Pass NULL for no filtering.
 * @param   txIDFilterLen : length of txIDFilter array. Max 28 * 4 (28 filter banks * 4 IDs per bank)
 * @return  None
 */
void BSP_CAN_Init(callback_t rxEvent, 
                  callback_t txEnd, 
                  bool faultState, 
                  bool loopback,
                  uint16_t *txIDFilter,
                  uint8_t txIDFilterLen);

/**
 * @brief   Calls stm-level CAN_DeInit
 * @return  None
 */
void BSP_CAN_DeInit();

ErrorStatus BSP_CAN_Write(uint32_t id, uint8_t data[], uint8_t length);

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
