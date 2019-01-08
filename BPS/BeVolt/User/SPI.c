/** SPI.c
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
 * @authors Sijin Woo
 * @lastRevised 12/4/2018
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

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "SPI.h"

/** SPI_Init8
 * Initializes SPI1 for multiple slaves to use. This SPI line is for 8 bit message formats.
 */
void SPI_Init8(void){
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
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

/** SPI_Init16
 * Initialize SPI2. This SPI line is for 16 bit message formats.
 */
void SPI_Init16(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	//Initialize pins
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI2);
	
	// Initialize SPI port
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_Cmd(SPI2, ENABLE);
}

/** SPI_InitCS
 * Initializes Port B pin for SPI chip select
 * Use GPIO_Pin_x (replace x) for easier code readablity.
 * @param pin number
 */
void SPI_InitCS(uint16_t pin){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/** SPI_Write8
 * Sends single 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single byte that will be sent to the slave.
 */
void SPI_Write8(uint8_t txData){
	uint8_t volatile junk = SPI_WriteRead8(txData);
}

/** SPI_WriteMulti8
 * Sends multiple 8-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of bytes to send.
 */
void SPI_WriteMulti8(uint8_t *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		SPI_Wait(SPI1);
		SPI1->DR = txBuf[i] & 0x00FF;
		SPI_Wait(SPI1);
		uint8_t volatile junk = SPI1->DR;
	}
}

/** SPI_Read8
 * Reads single byte from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single byte that has been received.
 */
uint8_t SPI_Read8(void){
	return SPI_WriteRead8(0x00);
}

/** SPI_Read8
 * Reads multiple 8-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of bytes to read from slave.
 */
void SPI_ReadMulti8(uint8_t *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		SPI_Wait(SPI1);
		SPI1->DR = 0x00;
		SPI_Wait(SPI1);
		rxBuf[i] = SPI1->DR & 0x00FF;
	}
}

/** SPI_WriteRead8
 * Sends and receives single 8-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single byte that will be sent to the slave.
 * @return rxData single byte that was read from the slave.
 */
uint8_t SPI_WriteRead8(uint8_t txData){
	SPI_Wait(SPI1);
	SPI1->DR = txData & 0x00FF;
	SPI_Wait(SPI1);
	return SPI1->DR & 0x00FF;
}

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
void SPI_WriteReadMulti8(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize, bool cmd){
	if(cmd){
		SPI_WriteMulti8(txBuf, txSize);
		SPI_ReadMulti8(rxBuf, rxSize);
	}else{
		for(uint32_t i = 0; i < txSize; i++){
			rxBuf[i] = SPI_WriteRead8(txBuf[i]);
		}
		for(uint32_t i = txSize; i < txSize - rxSize; i++){
			rxBuf[i] = SPI_WriteRead8(0x00);
		}
	}
}

/** SPI_Write16
 * Sends single 16-bit packet to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txData single 16-bit half-word that will be sent to the slave.
 */
void SPI_Write16(uint16_t txData){
	uint16_t volatile junk = SPI_WriteRead16(txData);
}

/** SPI_WriteMulti16
 * Sends multiple 16-bit packets to slave.
 * This function is usually used to send a command to a slave and then calling a read function
 * to get the response.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param txBuf ptr to unsigned 16-bit tx buffer.
 * @param txSize number of 16-bit half-words to send.
 */
void SPI_WriteMulti16(uint16_t *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		SPI_Wait(SPI2);
		SPI2->DR = txBuf[i] & 0xFFFF;
		SPI_Wait(SPI2)
		uint16_t volatile junk = SPI2->DR;
	}
}

/** SPI_Read16
 * Reads single 16-bit half-word from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @return single 16-bit half-word that has been received.
 */
uint16_t SPI_Read16(void){
	return SPI_WriteRead16(0x0000);
}

/** SPI_Read16
 * Reads multiple 16-bit packets from slave.
 * This function is usually used after calling a write function with a command and slave
 * responds in the next data transfer.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing.
 * @param rxBuf buffer that the received packet will be placed.
 * @param rxSize number of 16-bit half-words to read from slave.
 */
void SPI_ReadMulti16(uint16_t *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		SPI_Wait(SPI2);
		SPI2->DR = 0x0000;	// any command, this will be ignored by slave.
		SPI_Wait(SPI2);
		rxBuf[i] = SPI2->DR & 0xFFFF;
	}
}

/** SPI_WriteRead16
 * Sends and receives single 16-bit data from slave.
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param txData single 16-bit half-words that will be sent to the slave.
 * @return rxData single 16-bit half-words that was read from the slave.
 */
uint8_t SPI_WriteRead16(uint16_t txData){
	SPI_Wait(SPI2);
	SPI2->DR = txData & 0xFFFF;
	SPI_Wait(SPI2);
	return SPI2->DR & 0xFFFF;
}

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
void SPI_WriteReadMulti16(uint16_t *txBuf, uint32_t txSize, uint16_t *rxBuf, uint32_t rxSize, bool cmd){
	if(cmd){
		SPI_WriteMulti16(txBuf, txSize);
		SPI_ReadMulti16(rxBuf, rxSize);
	}else{
		for(uint32_t i = 0; i < txSize; i++){
			rxBuf[i] = SPI_WriteRead16(txBuf[i]);
		}
		for(uint32_t i = txSize; i < txSize - rxSize; i++){
			rxBuf[i] = SPI_WriteRead16(0x0000);
		}
	}
}

/** SPI_CSHigh
 * Sets PortB pin to high.
 * Use GPIO_Pin_x (replace x) for easier code readablity.
 * @param pin number of PortB
 */
void SPI_CSHigh(uint16_t pin){
	GPIO_SetBits(GPIOB, pin);
}

/** SPI_CSLow
 * Resets PortB pin to low.
 * Use GPIO_Pin_x (replace x) for easier code readablity.
 * @param pin number if PortB
 */
void SPI_CSLow(uint16_t pin){
	GPIO_ResetBits(GPIOB, pin);
}
