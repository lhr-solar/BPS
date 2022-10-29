/* Copyright (c) 2022 UT Longhorn Racing Solar */\

/**
 * BSP_UART.c - Simulates UART on BPS by just printing and reading from console. 
*/

#include "BSP_UART.h"
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>

/**
 * @brief   Initializes the UART peripheral
 */
void BSP_UART_Init(__attribute__((unused)) callback_t rxCallback,   // I can't be bother to figure out how to use these  
                   __attribute__((unused)) callback_t txCallback,   // callbacks in simulation, so they're just going to
                   __attribute__((unused)) UART_Port usart) {       // be unused for now.

}

/**
 * @brief   Gets one line of ASCII text that was received.
 * @pre     str should be at least 128bytes long.
 * @param   str : pointer to buffer to store the string. This buffer should be initialized
 *                  before hand.
 * @param   usart : which usart to read from
 * @return  number of bytes that was read
 */
uint32_t BSP_UART_ReadLine(char *str, UART_Port usart) {
    scanf("%[^\n]", str);
    return strlen(str);
}

/**
 * @brief   Transmits data to through UART line
 * @param   str : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @param   usart : which usart to write to
 * @return  number of bytes that were sent
 */
uint32_t BSP_UART_Write(char *str, uint32_t len, UART_Port usart) {
    return printf("%.*s", len, str);
}
