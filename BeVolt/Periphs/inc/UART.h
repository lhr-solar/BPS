/** UART.h
 * UART driver. Mainly for communication with PuTTY.
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

/***** Unlike SPI, UART cannot have multiple modules connected to the same tx/rx line *****/

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

/** UART3_Init
 * Initializes UART1 Module
 * Pins: 
 *		tx : PB10
 *		rx : PC5
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

/**
* @brief Configures the USART1 Peripheral for interrupts.
* @param None
* @retval None
*/
void USART1_Config(void);

/**
* @brief This function handles USART1 global interrupt request.
* @param None
* @retval None
*/
void USART1_IRQHandler(void);

#endif
