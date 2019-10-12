/** SPI.h
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
 */

/** Pins:
 * SPI1:
 *		PB3 : SCK
 *		PB4 : MISO
 *		PB5 : MOSI 
 * SPI3:
 *		PC10 : SCK
 *		PC11 : MISO
 *		PC12 : MOSI
 */

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "SPI.h"

/** SPI1_Init
 * Initializes SPI1 for multiple slaves to use. This SPI line is for 8 bit message formats.
 */
void SPI1_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// Initialize pins
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	// Initialize SPI port
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 0;	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

/** SPI3_Init
 * Initializes SPI3 for multiple slaves to use. This SPI line is for 8 bit message formats.
 */
void SPI3_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	// Initialize pins
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	
	// Initialize SPI port
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 0;	
	SPI_Init(SPI3, &SPI_InitStruct);
	SPI_Cmd(SPI3, ENABLE);
}

/** SPI1_Write
 * Sends single 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single byte that will be sent to the slave.
 */
void SPI1_Write(uint8_t txData){
	uint8_t volatile junk = SPI1_WriteRead(txData);
}

/** SPI3_Write
 * Sends single 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single byte that will be sent to the slave.
 */
void SPI3_Write(uint8_t txData){
	uint8_t volatile junk = SPI3_WriteRead(txData);
}

/** SPI1_WriteMulti
 * Sends multiple 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of bytes to send.
 */
void SPI1_WriteMulti(uint8_t *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		SPI1_WriteRead(txBuf[i]);
	}
}

/** SPI3_WriteMulti
 * Sends multiple 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of bytes to send.
 */
void SPI3_WriteMulti(uint8_t *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		SPI3_WriteRead(txBuf[i]);
	}
}

/** SPI1_Read
 * Reads single byte from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single byte that has been received.
 */
uint8_t SPI1_Read(void){
	return SPI1_WriteRead(0x00);
}

/** SPI3_Read
 * Reads single byte from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single byte that has been received.
 */
uint8_t SPI3_Read(void){
	return SPI3_WriteRead(0x00);
}

/** SPI1_ReadMulti
 * Reads multiple 8-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 */
void SPI1_ReadMulti(uint8_t *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		rxBuf[i] = SPI1_WriteRead(0x00);
	}
}

/** SPI3_ReadMulti
 * Reads multiple 8-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 */
void SPI3_ReadMulti(uint8_t *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		rxBuf[i] = SPI3_WriteRead(0x00);
	}
}

/** SPI1_WriteRead
 * Sends and receives single 8-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single byte that will be sent to the slave.
 * @return rxData single byte that was read from the slave.
 */
uint8_t SPI1_WriteRead(uint8_t txData){
	//printf("%x ", txData);
	SPI_Wait(SPI1);
	SPI1->DR = txData & 0x00FF;
	SPI_Wait(SPI1);
	return SPI1->DR & 0x00FF;
}

/** SPI3_WriteRead
 * Sends and receives single 8-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single byte that will be sent to the slave.
 * @return rxData single byte that was read from the slave.
 */
uint8_t SPI3_WriteRead(uint8_t txData){
	//printf("%x ", txData);
	SPI_Wait(SPI3);
	SPI3->DR = txData & 0x00FF;
	SPI_Wait(SPI3);
	return SPI3->DR & 0x00FF;
}

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
void SPI1_WriteReadMulti(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize, bool cmd){
	if(cmd){
		SPI1_WriteMulti(txBuf, txSize);
		SPI1_ReadMulti(rxBuf, rxSize);
	}else{
		for(uint32_t i = 0; i < txSize; i++){
			rxBuf[i] = SPI1_WriteRead(txBuf[i]);
		}
		for(uint32_t i = txSize; i < txSize - rxSize; i++){
			rxBuf[i] = SPI1_WriteRead(0x00);
		}
	}
}

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
void SPI3_WriteReadMulti(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize, bool cmd){
	if(cmd){
		SPI3_WriteMulti(txBuf, txSize);
		SPI3_ReadMulti(rxBuf, rxSize);
	}else{
		for(uint32_t i = 0; i < txSize; i++){
			rxBuf[i] = SPI3_WriteRead(txBuf[i]);
		}
		for(uint32_t i = txSize; i < txSize - rxSize; i++){
			rxBuf[i] = SPI3_WriteRead(0x00);
		}
	}
}
