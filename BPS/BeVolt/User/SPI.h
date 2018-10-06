/** SPI.h
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 10/2/2018
 */

#ifndef SPI_H__
#define SPI_H__

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

/** SPI1_Init
 * Initializes SPI1 for multiple slaves to use
 */
void SPI1_Init(void);

/** SPI1_Write
 * Sends data to slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit tx buffer, number of bytes to send
 */
void SPI1_Write(uint8_t *txBuf, uint32_t txSize);

/** SPI1_Read
 * Reads data from slave and places it in rxBuf and rxSize
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit rx buffer, number of bytes to read
 */
void SPI1_Read(uint8_t *rxBuf, uint32_t rxSize);

/** SPI1_WriteRead
 * Sends and receives data from slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 16-bit tx buffer, number of bytes to sen, ptr to unsigned 16-bit rx buffer, number of bytes to rea
 */
void SPI1_WriteRead(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize);

#endif
