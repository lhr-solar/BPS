/** I2C.c
 * This I2C code was taken from STM's example code for an EEPROM
 */

#include "I2C.h"

void I2C3_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// Uses PA8 for SCL and PC9 for SDA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	// Initialize PA8 for SCL on I2C3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;		// Watch out for Output Type. I2C is an open-drain comm protocol unlike SPI/UART
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Initialize PC9 for SDA on I2C3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;	// Only need to change pin since the rest of the pin configurations are the same
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Set which alternate function mode we want for these pins.
	// We want to make these pins use I2C3.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);	// PA8
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);		// PC9

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;						// regular mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;		// 50% duty cycle on clock
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		// 7-bit addressing mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;					// Enable ack to confirm if the slave device has received the data or not
	I2C_InitStruct.I2C_ClockSpeed = 100000;						// Generic speed
	I2C_InitStruct.I2C_OwnAddress1 = 0x50 << 1;				// Any address will do. I just chose the same address as the EEPROM.
																									// Note I left shifted the address. That's because the address is supposed
																									// to be 7-bits and the least significant bit indicates the R/W setting
	I2C_Init(I2C3, &I2C_InitStruct);
	I2C_Cmd(I2C3, ENABLE);
}

uint8_t I2C3_Read(uint8_t deviceAddr, uint16_t regAddr){
	uint8_t data;
	I2C3_ReadMultiple(deviceAddr, regAddr, &data, 1);
	return data;
}

ErrorStatus I2C3_ReadMultiple(uint8_t deviceAddr, uint16_t startAddr, uint8_t *rxData, uint32_t rxSize){
	int timeout_count = 0;
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)){
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}
	
	I2C_AcknowledgeConfig(I2C3, ENABLE);

	// First write address that you want to read from

	readPoll:
	// Since no one is using the I2C bus, take control
	I2C_GenerateSTART(I2C3, ENABLE);
	// Wait until start edge event occurred
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}

	// Select device to talk to
	I2C_Send7bitAddress(I2C3, deviceAddr, I2C_Direction_Transmitter);		// Sets RW bit to 0
	// Wait until finished sending
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if(I2C3->SR1 & 0x0400) {
			I2C3->SR1 &= ~0x0400;
			I2C_GenerateSTOP(I2C3, ENABLE);
			goto readPoll;
		}
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}

	// Send start address (MSB first)
	I2C_SendData(I2C3, (uint8_t)((startAddr & 0xFF00) >> 8));
	// Wait until transmit event occurred.
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}
	// Send rest of start address (LSB)
	I2C_SendData(I2C3, (uint8_t)(startAddr & 0x00FF));
	// Wait until transmit event occurred.
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}

	// Wait until byte transmission finished
	timeout_count = 0;
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}


	// Now read data starting from address that was sent

	// Since no one is using the I2C bus, take control
	I2C_GenerateSTART(I2C3, ENABLE);
	// Wait until start edge event occurred
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}

	// Select device to talk to
	I2C_Send7bitAddress(I2C3, deviceAddr, I2C_Direction_Receiver);		// Sets RW bit to 1
	// Wait until finished sending
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after 3 seconds (240 million iterations)
		if(timeout_count > 240000000) {
			return ERROR;
		}
	}

	while(rxSize > 0){
		if(rxSize > 1){
			// Wait until byte transmission finished
			timeout_count = 0;
			while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET) {
				// Assume running at 80 MHz
				timeout_count++;
				// Returns and breaks after 3 seconds (240 million iterations)
				if(timeout_count > 240000000) {
					return ERROR;
				}
			}
			*rxData = I2C_ReceiveData(I2C3);
			rxData++;
			rxSize--;
		} else {
			// Disable ack, since this is the last byte
			I2C_AcknowledgeConfig(I2C3, DISABLE);
		
			timeout_count = 0;
			while(I2C_GetFlagStatus(I2C3, I2C_FLAG_RXNE) == RESET) {
				// Assume running at 80 MHz
				timeout_count++;
				// Returns and breaks after 3 seconds (240 million iterations)
				if(timeout_count > 240000000) {
					return ERROR;
				}
			}

			*rxData = I2C_ReceiveData(I2C3);
			rxData++;
			rxSize = 0;
		}
	}

	// Generate the stop
	I2C_GenerateSTOP(I2C3, ENABLE);
	
	// Return Success if all executed properly
	return SUCCESS;
}

void I2C3_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t data){
	I2C3_WriteMultiple(deviceAddr, regAddr, &data, 1);
}

void I2C3_WriteMultiple(uint8_t deviceAddr, uint16_t startAddr, uint8_t *txData, uint32_t txSize){
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)){
		// Add timeout later. Since this is not a safety critical part of the system, a timeout would be okay.
		// The next time this is called, the I2C bus should not be busy.
	}

	I2C_AcknowledgeConfig(I2C3, ENABLE);

	// First write address that you want to read from

	writePoll:
	// Since no one is using the I2C bus, take control
	I2C_GenerateSTART(I2C3, ENABLE);
	// Wait until start edge event occurred
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));

	// Select device to talk to
	I2C_Send7bitAddress(I2C3, deviceAddr, I2C_Direction_Transmitter);		// Sets RW bit to 0
	
	// THIS IS WHERE WE GOT STUCK
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if(I2C3->SR1 & 0x0400) {
			I2C3->SR1 &= ~0x0400;
			I2C_GenerateSTOP(I2C3, ENABLE);
			goto writePoll;
		}
	}

	// Send start address (MSB first)
	I2C_SendData(I2C3, (uint8_t)((startAddr & 0xFF00) >> 8));
	// Wait until transmit event occurred.
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
	// Send rest of start address (LSB)
	I2C_SendData(I2C3, (uint8_t)(startAddr & 0x00FF));
	// Wait until transmit event occurred.
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

	// Wait until byte transmission finished
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET);

	// Start writing data sequentially
	while(txSize > 0){

		// Wait until BTF flag is set
		while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET);
		I2C_SendData(I2C3, *txData);
		txData++;
		txSize--;
	}

	I2C_GenerateSTOP(I2C3, ENABLE);
}
