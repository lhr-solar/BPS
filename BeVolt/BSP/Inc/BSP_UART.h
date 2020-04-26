#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "common.h"

/**
 * @brief   Initializes the UART peripheral
 * 
 */
void BSP_UART_Init(void);

/**
 * @brief   Gets one line of ASCII text that was received.
 * @pre     Buffer should be at least 128bytes long.
 * @param   buffer : pointer to buffer to store the array. This buffer should be initialized
 *                  before hand.
 * @return  number of bytes that was read
 */
uint32_t BSP_UART_ReadLine(uint8_t *buffer);

/**
 * @brief   Transmits data to through UART line
 * @param   buffer : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @return  numer of bytes that were sent
 */
uint32_t BSP_UART_Write(uint8_t *buffer, uint32_t len);

#endif
