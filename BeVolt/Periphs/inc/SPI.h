/** SPI.h
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
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
 * SPI3:
 *		PC10 : SCK
 *		PC11 : MISO
 *		PC12: MOSI
 */

/** SPI1_Init
 * Initializes SPI1 for multiple slaves to use.
 * SPI1 Pins:
 * 		PB3 : SCK
 *		PB4 : MISO
 *		PB5 : MOSI 
 */
void SPI1_Init(void);

/** SPI1_Write
 * Sends single 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single byte that will be sent to the slave.
 */
void SPI1_Write(uint8_t txData);

/** SPI1_WriteMulti
 * Sends multiple 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of bytes to send.
 */
void SPI1_WriteMulti(uint8_t *txBuf, uint32_t txSize);

/** SPI1_Read
 * Reads single byte from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single byte that has been received.
 */
uint8_t SPI1_Read(void);

/** SPI1_Read
 * Reads multiple 8-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 */
void SPI1_ReadMulti(uint8_t *rxBuf, uint32_t rxSize);

/** SPI1_WriteRead
 * Sends and receives single 8-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single byte that will be sent to the slave.
 * @return rxData single byte that was read from the slave.
 */
uint8_t SPI1_WriteRead(uint8_t txData);

/** SPI1_WriteReadMulti
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
void SPI1_WriteReadMulti(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize, bool cmd);

/** SPI3_Init
 * Initializes SPI3 for multiple slaves to use.
 * SPI3 Pins:
 * 		PC10 : SCK
 *		PC11 : MISO
 *		PC12 : MOSI 
 */
void SPI3_Init(void);

/** SPI3_Write
 * Sends single 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single byte that will be sent to the slave.
 */
void SPI3_Write(uint8_t txData);

/** SPI3_WriteMulti
 * Sends multiple 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of bytes to send.
 */
void SPI3_WriteMulti(uint8_t *txBuf, uint32_t txSize);

/** SPI3_Read
 * Reads single byte from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single byte that has been received.
 */
uint8_t SPI3_Read(void);

/** SPI3_Read
 * Reads multiple 8-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 */
void SPI3_ReadMulti(uint8_t *rxBuf, uint32_t rxSize);

/** SPI3_WriteRead
 * Sends and receives single 8-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single byte that will be sent to the slave.
 * @return rxData single byte that was read from the slave.
 */
uint8_t SPI3_WriteRead(uint8_t txData);

/** SPI3_WriteReadMulti
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
void SPI3_WriteReadMulti(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize, bool cmd);

#ifdef __cplusplus
}
#endif

#endif	/* SPI_H__ */
