/* Copyright (c) 2022 UT Longhorn Racing Solar */

#ifndef BSP_UART_H
#define BSP_UART_H

#include "common.h"
#include "os.h"

//enum to describe what UART port you are trying to access
typedef enum {UART_USB, UART_BLE, NUM_UART} UART_Port;

/**
 * @brief   Initializes the UART peripheral
 * @param   rxCallback The callback for when data come in to the system
 * @param   txCallback The callback for when transmission completes
 * @param   usart The USART to initialize
 */
void BSP_UART_Init(callback_t rxCallback, callback_t txCallback, UART_Port usart);

/**
 * @brief   Gets one line of ASCII text that was received.
 * @pre     str should be at least 128 bytes long.
 * @param   str : pointer to string to store the string. This buffer should be initialized before hand.
 * @param   usart : which usart to read from (USB or BLE)     
 * @return  number of bytes that was read
 */
uint32_t BSP_UART_ReadLine(char *str, UART_Port usart);

/**
 * @brief   Transmits data to through UART line
 * @param   str : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @param   usart : which usart to read from (2 or 3)
 * @return  number of bytes that were sent
 * 
 * @note This function uses a fifo to buffer the write. If that
 *       fifo is full, this function may block while waiting for
 *       space to open up. Do not call from timing-critical
 *       sections of code.
 */
uint32_t BSP_UART_Write(char *str, uint32_t len, UART_Port usart);

#endif
