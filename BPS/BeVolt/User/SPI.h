/** SPI.h
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
 * @authors Sijin Woo
 * @lastRevised 11/19/2018
 */

#ifndef SPI_H__
#define SPI_H__

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
// Use this macro function to wait until SPI communication is complete
#define SPI_Wait(SPIx)		while(((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/**
 * SPI1 module is used so function names start with SPI1. This is to differentiate between SPI modules
 * and also due to name conflicts in the standard peripheral library. If you want to add more SPI pins,
 * just add the SPI module number in the beginning of the function name.
 */

/** Pins:
 * SPI1:
 *		PB3 : SCK
 *		PB4 : MISO
 *		PB5 : MOSI 
 * SPI2:
 *		PB13 : SCK
 *		PB14 : MISO
 *		PB15 : MOSI
 */

/** SPI_Init8
 * Initializes SPI1 for multiple slaves to use.
 * SPI1 Pins:
 * 		PB3 : SCK
 *		PB4 : MISO
 *		PB5 : MOSI 
 */
void SPI_Init8(void);

/** SPI_Write8
 * Sends single 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single byte that will be sent to the slave.
 */
void SPI_Write8(uint8_t txData);

/** SPI_WriteMulti8
 * Sends multiple 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of bytes to send.
 */
void SPI_WriteMulti8(uint8_t *txBuf, uint32_t txSize);

/** SPI_Read8
 * Reads single byte from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single byte that has been received.
 */
uint8_t SPI_Read8(void);

/** SPI_Read8
 * Reads multiple 8-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 */
void SPI_ReadMulti8(uint8_t *rxBuf, uint32_t rxSize);

/** SPI_WriteRead8
 * Sends and receives single 8-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single byte that will be sent to the slave.
 * @return rxData single byte that was read from the slave.
 */
uint8_t SPI_WriteRead8(uint8_t txData);

/** SPI_WriteReadMulti8
 * Sends and receives multiple 8-bit data packets from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @precondition txSize >= rxSize if cmd is false.
 * @param txBuf buffer of data that will be sent to the slave.
 * @param txSize number of bytes to be sent.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 * @param cmd set if txBuf are commands where the slave will not respond with any data
 *			during during the transmit process. Reset if during transmit sequence,
 *			data will be received from the slave.
 */
void SPI_WriteReadMulti8(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize, bool cmd);

/** SPI_Init16
 * Initializes SPI2 with 16-bit data length for multiple slaves to use.
 * SPI2 Pins:
 *		PB13 : SCK
 *		PB14 : MISO
 *		PB15 : MOSI
 */
void SPI_Init16(void);

/** SPI_Write16
 * Sends single 16-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single 16-bit half-word that will be sent to the slave.
 */
void SPI_Write16(uint16_t txData);

/** SPI_WriteMulti16
 * Sends multiple 16-bit packets to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of 16-bit half-words to send.
 */
void SPI_WriteMulti16(uint16_t *txBuf, uint32_t txSize);

/** SPI_Read16
 * Reads single 16-bit half-word from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single 16-bit half-word that has been received.
 */
uint16_t SPI_Read16(void);

/** SPI_Read16
 * Reads multiple 16-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of 16-bit half-words to read from slave.
 */
void SPI_ReadMulti16(uint16_t *rxBuf, uint32_t rxSize);

/** SPI_WriteRead16
 * Sends and receives single 16-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single 16-bit half-words that will be sent to the slave.
 * @return rxData single 16-bit half-words that was read from the slave.
 */
uint8_t SPI_WriteRead16(uint16_t txData);

/** SPI_WriteReadMulti16
 * Sends and receives multiple 16-bit data packets from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @precondition txSize >= rxSize if cmd is false.
 * @param txBuf buffer of data that will be sent to the slave.
 * @param txSize number of 16-bit half-words to be sent.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of 16-bit half-words to read from slave.
 * @param cmd set if txBuf are commands where the slave will not respond with any data
 *			during during the transmit process. Reset if during transmit sequence,
 *			data will be received from the slave.
 */
void SPI_WriteReadMulti16(uint16_t *txBuf, uint32_t txSize, uint16_t *rxBuf, uint32_t rxSize, bool cmd);

/** SPI_InitCS
 * Initializes Port B pin for SPI chip select
 * Use GPIO_Pin_x (replace x) for easier code readablity.
 * @param pin number
 */
void SPI_InitCS(uint16_t pin);

/** SPI_CSHigh
 * Sets PortB pin to high.
 * Use GPIO_Pin_x (replace x) for easier code readablity.
 * @param pin number of PortB
 */
void SPI_CSHigh(uint16_t pin);

/** SPI_CSLow
 * Resets PortB pin to low.
 * Use GPIO_Pin_x (replace x) for easier code readablity.
 * @param pin number if PortB
 */
void SPI_CSLow(uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif	/* SPI_H__ */
