/** UART.h
 * UART driver. Mainly for communication with PuTTY.
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "config.h"
#include "FIFO.h"

/***** Unlike SPI, UART cannot have multiple modules connected to the same tx/rx line *****/

/** UART1_Write
 * Sends data through tx pin for UART1
 * @param ptr to char buffer, size of buffer
 */
void UART1_Write(char *txBuf, uint32_t txSize);

/** UART1_Read
 * Sends data through tx pin for UART1
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART1_Read(char *rxBuf, uint32_t rxSize);

/**
* @brief Configures the UART1 Peripheral for interrupts.
* @param None
* @retval None
*/
void UART1_Init(void);

//use when fifo is storing commands received over uart
//returns if fifo has a command in it by checking for carriage return
bool UART1_HasCommand(Fifo *fifo);

//for using fifo to store commands received from UART
//fails if multiple commands are stored in queue
//preconditions: fifo has a command stored in it, buffer is large enough to hold command
//parameters: fifo with command stored in it, buffer to receive stored command
//fills buffer with received command
void UART1_GetCommand(Fifo *fifo, char buffer[]);

//checks if new data has been received from UART interrupts
//if so, it echoes the new character to Putty and adds the character to the passed fifo
//backspace is supported
void UART1_CheckAndEcho(Fifo *uartFifo);

#endif
