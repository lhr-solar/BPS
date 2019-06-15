// LTC2983.c
/**
 * Driver for LTC2983 chip related functions. The IC uses SPI to communicate to
 * to MCU. IsoSPI is not required since temperature sensors are galvanically isolated.
 * regular SPI to isoSPI.
 * If using SPI: MCU --SPI--> LTC2983
 * If using isoSPI: MCU --SPI--> LTC6820 --isoSPI--> LTC2983
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "SPI.h"
#include "LTC2983.h"


const uint8_t READ_CMD = 0x03;
const uint8_t WRITE_CMD = 0x02;
uint8_t COMMAND_STATUS_REGISTER[2] = {0x00, 0x00};
uint8_t START_CONVERSION_CH1 = 0x81;

// Test
uint8_t channel1_configure[2] = {0x02, 0x00};		// Channel 1 address
uint8_t channel1_result[2] = {0x01, 0x00};

/** LTC2983_Init
 * Initializes SPI pins
 * Initializes and configures LTC2983 chip 
 */
void LTC2983_Init(void){
	for(int i = 0; i < 2000000; i++) ;
	uint8_t message[7] = {WRITE_CMD, 0x02, 0x00, 0xF4, 0x00, 0x00, 0x00};	
	SPI_Init8();
	
	// Initialize pins
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// Up vs Down
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIOB->ODR |= GPIO_Pin_13;			// Start CS High
	
	uint8_t readyToTransmit = 0x40;
	
	while(LTC2983_Ready()) {
		printf("Delay Init\n\r");
	};	// Wait until LTC2983 is ready starting up (might not be necessary
	
	uint8_t directADCOption[4] = {0xF4, 0x00, 0x00, 0x00};			// ADC Data Option Command
	
	// Configure channel 1 to ADC mode
//	SPI_Write8(WRITE_CMD);
//	SPI_WriteMulti8(channel1_configure, 2);			
//	SPI_WriteMulti8(directADCOption, 4);
	GPIOB->ODR &= ~GPIO_Pin_13;
	SPI_WriteMulti8(message, 7);
	GPIOB->ODR |= GPIO_Pin_13;
printf("          Direct ADC selected\r\n");
  
		
//	uint8_t *buff;
//	
//	txBuff[0] = 0x02;
//	txBuff[1] = 0x00;
//	txBuff[2] = 0x00;
//	txBuff[3] = 0x81;
//	
//	buff[0] = 0x02;
//	buff[1] = 0x02;
//	buff[2] = 0x00;
//	buff[3] = 0xF4;
//	buff[4] = 0x00;
//	buff[5] = 0x00;
//	buff[6] = 0x00;
//	
//	//SPI_WriteMulti8(txBuff, 4);
//	
//	for(int i = 0; i < 20; i++) {
//		SPI_WriteMulti8(txBuff, 4);
//		txBuff[3]++;
//		
//		SPI_WriteMulti8(buff, 7);
//		buff[2] += 4;
//		
//	}	
	
}



/** LTC2983_Ready
 * Checks if LTC2984 is ready after startup
 * @return unsigned int 16-bit data
 */
bool LTC2983_Ready(void){
	uint8_t message[3] = {READ_CMD, 0x00, 0x00};
	uint8_t result[1];
//	SPI_Write8(READ_CMD);		// Send read cmd
//	SPI_WriteMulti8(COMMAND_STATUS_REGISTER, 2); 
	GPIOB->ODR &= ~GPIO_Pin_13;
	//SPI_WriteMulti8(message, 3);
	
	SPI_WriteReadMulti8(message, 3, result, 1, true);
	GPIOB->ODR |= GPIO_Pin_13;
	printf("Slave Ready : %c\n\r", result[0]);
  if(/*SPI_Read8()*/result[0] == 0x40) {
		//GPIOB->ODR |= GPIO_Pin_13;
		return true;
	}
  else {
		//GPIOB->ODR |= GPIO_Pin_13;
		return false;
	}
	
}



/** LTC2983_MeasureSingleChannel
 * Sends command to LTC2983 to gather and save all ADC values
 * @param 16 bit channel size in a 1 byte array
 * @return unsigned int 32-bit measurements from specified channel.
 *       returns a -1 if invalid ADC measurement
 */
int32_t LTC2983_MeasureSingleChannel(void /*uint8_t *channelAddr*/){
	uint8_t result[4];
	uint8_t status[3];
	uint8_t message[4] = {WRITE_CMD, 0x00, 0x00, 0x81};	 // Command, Address, Data
				// message for start conversion
	uint8_t receive[3] = {READ_CMD, 0x01, 0x00};
				// Read conversion result from channel 1 
	// Initiate conversion
//	SPI_Write8(WRITE_CMD);
//	SPI_WriteMulti8(COMMAND_STATUS_REGISTER, 2);
//	SPI_Write8(START_CONVERSION_CH1);
	GPIOB->ODR &= ~GPIO_Pin_13;
	SPI_WriteReadMulti8(message, 4, status, 1, true);
	GPIOB->ODR &= ~GPIO_Pin_13;
	status[0] &= 0x40;
	//printf("Status: %c\n\r", status[0]);
	//if(status[0] != 0x40) return -1;
	//while(LTC2983_Ready()) {
	//	printf("Waiting");
	//}		// Wait until conversion done		
// Should I exit function and run rest of program until ready when I call again?
	
	// Read result
//	SPI_Write8(READ_CMD);
//	SPI_WriteMulti8(channel1_result, 2);
//	SPI_ReadMulti8(result, 4);
	GPIOB->ODR &= ~GPIO_Pin_13;			
	//SPI_WriteMulti8(receive, 3);		
	//SPI_ReadMulti8(result, 4);
		
	SPI_WriteReadMulti8(receive, 4, result, 4, true);
	GPIOB->ODR |= GPIO_Pin_13;	
		
	if((result[0] & 0x01) == 0x01) {
		return ((int32_t)result & 0x00FFFFFF);
	} else {
		return -1;
	}
		
}




///** LTC2983_Measure
// * Sends command to LTC2983 to gather and save all ADC values
// * @return unsigned int 16-bit measurements from all ADCs
// */
//uint16_t *LTC2983_Measure(void){
//	
//}



///** LTC2983_ReadChannel
// * Reads the 24 bit ADC value at specified channel
// *    Message Format:
// *        1.) CMD (read = 0x02)
// *        2.) Channel Address (2 bytes : 0x200 - 0x24C)
// *        3.) Data (4 bytes : Either received or transmitted)
// * @param unsigned int 16-bit channel number
// * @return unsinged 32-bit data for channel
// */
//uint32_t LTC2983_ReadChannel(uint16_t channel) {
//	uint16_t const CHANNEL_OFFSET = 0x010;
//	uint8_t const READ_CMD = 0x03;
//	
//	uint16_t conversion;
//	//uint8_t convert[2];
//	uint16_t channelAddr;
//	
//	// Buffers to send and receive
//	uint8_t txBuff[3];
//	uint8_t rxBuff[4];
//	
//	channelAddr = (channel * 4) + CHANNEL_OFFSET;		// Get address of the channel (4 bytes each)
//	conversion = channelAddr;   // Temp
//	
////	convert[1] = (uint8_t)(channelAddr & 0x00FF);
////	convert[0] = (uint8_t)((conversion & 0xFF00) >> 8);
//	
//	txBuff[2] = (uint8_t)(channelAddr & 0x00FF);
//	txBuff[1] = (uint8_t)((conversion & 0xFF00) >> 8);
//	
//	txBuff[0] = READ_CMD;			// First instruction READ = 0x02
////	txBuff[1] = convert[0];
////	txBuff[2] = convert[1];
//	
//	// Transmit
//	SPI_WriteMulti8(txBuff, 3);
//	SPI_ReadMulti8(rxBuff, 4);
//	
//	return (uint32_t)rxBuff[0];
//} 





