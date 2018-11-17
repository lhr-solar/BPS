/** UART.h
 * UART driver. Mainly for communication with PuTTY.
 * @authors Sijin Woo
 * @lastRevised 10/2/2018
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include "stm32f4xx.h"
#include "FIFO.h"
#include "Definition.h"

/***** Unlike SPI, UART cannot have multiple modules connected to the same tx/rx line *****/
// ONLY USE UART3 FOR DEBUGGING ON NUCLEO!!!!

/** Pins:
 * UART1:
 *		tx : PA9
 *		rx : PA10
 * UART3:		*** NOTE: Nucleo Board uses these pins for USB ***
 *		tx : PD8
 *		rx : PD9
 */

/** UART1_Init
 * Initializes UART1 Module
 * Pins: 
 *		tx : PA9
 *		rx : PA10
 * @param baud rate: 9600, 115200, 500000
 */
void UART1_Init(uint32_t baud);

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

/**************** WARNING ***************/
// ONLY USE THIS FOR DEBUGGING ON NUCLEO!!!!!
/****************************************/
/** UART3_Init
 * Initializes UART1 Module
 * Pins: 
 *		tx : PD8
 *		rx : PD9
 * @param baud rate: 9600, 115200, 500000
 */
void UART3_Init(uint32_t baud);

/** UART3_Write
 * Sends data through tx pin for UART3
 * @param ptr to char buffer, size of buffer
 */
void UART3_Write(char *txBuf, uint32_t txSize);

/** UART3_Read
 * Reads data from rx pin for UART3
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART3_Read(char *rxBuf, uint32_t rxSize);

#endif
