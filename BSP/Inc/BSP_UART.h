/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef BSP_UART_H
#define BSP_UART_H

#include "common.h"
#include "os.h"

//enum to describe what UART port you are trying to access
typedef enum {UART_USB, UART_BLE} UART_Port;

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
 * @brief   Transmits data through UART line
 * @param   str : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @param   usart : which usart to read from (USB or BLE)
 * @return  number of bytes that were sent
 */
uint32_t BSP_UART_Write(char *str, uint32_t len, UART_Port usart);

#endif
