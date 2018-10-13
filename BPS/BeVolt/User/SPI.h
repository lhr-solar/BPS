/** SPI.h
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 10/2/2018
 */

#ifndef SPI_H__
#define SPI_H__

#include <stdint.h>

/**
 * SPI1 module is used so function names start with SPI1. This is to differentiate between SPI modules
 * and also due to name conflicts in the standard peripheral library. If you want to add more SPI pins,
 * just add the SPI module number in the beginning of the function name.
 */

/** Pins:
 * SPI1:
 * 		PB3 : SCK
 *		PB4 : MISO
 *		PB5 : MOSI 
 */

/** SPI_Init8
 * Initializes SPI1 for multiple slaves to use
 */
void SPI_Init8(void);

/** SPI_Write8
 * Sends data to slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit tx buffer, number of bytes to send
 */
void SPI_Write8(uint8_t *txBuf, uint32_t txSize);

/** SPI_Read8
 * Reads data from slave and places it in rxBuf and rxSize
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit rx buffer, number of bytes to read
 */
void SPI_Read8(uint8_t *rxBuf, uint32_t rxSize);

/** SPI_WriteRead8
 * Sends and receives data from slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit tx buffer, number of bytes to sen, ptr to unsigned 16-bit rx buffer, number of bytes to rea
 */
void SPI_WriteRead8(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize);

/** SPI_Init16
 * Initializes SPI1 for multiple slaves to use
 */
void SPI_Init16(void);

/** SPI_Write16
 * Sends data to slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit tx buffer, number of bytes to send
 */
void SPI_Write16(uint16_t *txBuf, uint32_t txSize);

/** SPI_Read16
 * Reads data from slave and places it in rxBuf and rxSize
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit rx buffer, number of bytes to read
 */
void SPI_Read16(uint16_t *rxBuf, uint32_t rxSize);

/** SPI_WriteRead16
 * Sends and receives data from slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit tx buffer, number of bytes to sen, ptr to unsigned 16-bit rx buffer, number of bytes to rea
 */
void SPI_WriteRead16(uint16_t *txBuf, uint32_t txSize, uint16_t *rxBuf, uint32_t rxSize);

#endif
