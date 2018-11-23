/** SPI.c
 * SPI driver for all modules using SPI protocol.
 * To use this driver, you must initialize your own chip select GPIO pins in your respective source files.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 10/2/2018
 */

/** Pins:
 * SPI1:
 * 		PB3 : SCK
 *		PB4 : MISO
 *		PB5 : MOSI 
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"

/** SPI_Init8
 * Initializes SPI1 for multiple slaves to use. This SPI line is for 8 bit message formats.
 */
void SPI_Init8(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO portB clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;		// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;							// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;					// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO portB clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;		// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;							// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;					// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI2);
	
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
	GPIO_InitStruct.GPIO_Pin = pin;								// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/** SPI_Write8
 * Sends data to slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 8-bit tx buffer, number of bytes to send
 */
void SPI_Write8(uint8_t *txBuf, uint32_t txSize){
	//SPI1->DR = 0xCC;
	for(uint32_t i = 0; i < txSize; i++){
		while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Wait until transmission is done
		while((SPI1->SR&SPI_SR_TXE) == 0);					// Wait until transmit buffer empty
		SPI1->DR = txBuf[i] & 0xFF;
		while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Wait until transmission is done
		while((SPI1->SR&SPI_SR_RXNE) == 0);					// Wait until data has been received
		uint32_t junk = SPI1->DR;
	}
	while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);		// Wait until transmission is done
}

/** SPI_Read8
 * Reads data from slave and places it in rxBuf and rxSize
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 8-bit rx buffer, number of bytes to read
 */
void SPI_Read8(uint8_t *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Wait until transmission is done
		while((SPI1->SR&SPI_SR_TXE) == 0);					// Wait until transmit buffer is empty
		SPI1->DR = 0xFF;		// Send nothing
		while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Wait until transmission is done
		while((SPI1->SR&SPI_SR_RXNE) == 0);					// Wait until data has been received
		rxBuf[i] = SPI1->DR & 0xFF;
	}
	
	while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);		// Wait until transmission is done	
}

/** SPI_WriteRead8
 * Sends and receives data from slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 8-bit tx buffer, number of bytes to sen, ptr to unsigned 16-bit rx buffer, number of bytes to rea
 */
void SPI_WriteRead8(uint8_t *txBuf, uint32_t txSize, uint8_t *rxBuf, uint32_t rxSize){
	SPI_Write8(txBuf, txSize);
	SPI_Read8(rxBuf, rxSize);
	
	/*
	for(uint32_t txIdx = 0; txIdx < txSize; txIdx++){
		while((SPI1->SR&SPI_SR_TXE) != SPI_SR_TXE);
		SPI1->DR = txBuf[txIdx] & 0xFF;
		while((SPI1->SR&SPI_SR_RXNE) != SPI_SR_RXNE);
		uint32_t junk = SPI1->DR;
	}
	
	for(int i = 0; i < rxSize; i++){
		while((SPI1->SR&SPI_SR_TXE) != SPI_SR_TXE);
		SPI1->DR = 0xFF;
		while((SPI1->SR&SPI_SR_RXNE) != SPI_SR_RXNE);
		rxBuf[i] = SPI1->DR & 0xFF;
		UART3_Write((char *)&rxBuf[i], 1);
	}
	*/
	
	// DEBUG
	//UART3_Write((char *)rxBuf, 1);
	// DEBUG
}

/** SPI_Write16
 * Sends data to slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 8-bit tx buffer, number of bytes to send
 */
void SPI_Write16(uint16_t *txBuf, uint32_t txSize){
	//SPI1->DR = 0xCC;
	for(uint32_t i = 0; i < txSize; i++){
		while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Wait until transmission is done
		SPI2->DR = txBuf[i] & 0xFF;
		while((SPI2->SR&SPI_SR_RXNE) == SPI_SR_RXNE){
			uint32_t volatile junk = SPI2->DR;
		}
	}
	while((SPI2->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Have to wait until transmission is done
}

/** SPI_Read16
 * Reads data from slave and places it in rxBuf and rxSize
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 8-bit rx buffer, number of bytes to read
 */
void SPI_Read16(uint16_t *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		while((SPI1->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Wait until transmission is done
		SPI2->DR = 0xFF;		// Send nothing
		while((SPI2->SR&SPI_SR_RXNE) != SPI_SR_RXNE);
		rxBuf[i] = SPI2->DR;
	}
	while((SPI2->SR&SPI_SR_BSY) == SPI_SR_BSY);	// Have to wait until transmission is done
}

/** SPI_WriteRead16
 * Sends and receives data from slave
 * NOTE: You must bring CS down before calling this function and raise it up after finishing
 * @param ptr to unsigned 8-bit tx buffer, number of bytes to sen, ptr to unsigned 16-bit rx buffer, number of bytes to rea
 */
void SPI_WriteRead16(uint16_t *txBuf, uint32_t txSize, uint16_t *rxBuf, uint32_t rxSize){
	SPI_Write16(txBuf, txSize);
	SPI_Read16(rxBuf, rxSize);
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
