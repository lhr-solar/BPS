/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_I2C.h"
#include "stm32f4xx.h"

#define TIMEOUT_THRESHOLD   1200000 // 15 ms delay threshold (3x the write time)

/**
 * @brief   Initializes the I2C port that interfaces with the EEPROM.
 * @param   None
 * @return  None
 */
void BSP_I2C_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// Uses PA8 for SCL and PC9 for SDA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);

	// Initialize PA8 for SCL on I2C3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;     // Watch out for Output Type. I2C is an open-drain comm protocol unlike SPI/UART
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Initialize PC9 for SDA on I2C3
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;          // Only need to change pin since the rest of the pin configurations are the same
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Set which alternate function mode we want for these pins.
	// We want to make these pins use I2C3.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_I2C3);     // PA8
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_I2C3);     // PC9

	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;						// regular mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;             // 50% duty cycle on clock
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;		// 7-bit addressing mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                    // Enable ack to confirm if the slave device has received the data or not
	I2C_InitStruct.I2C_ClockSpeed = 100000;                     // Generic speed
	I2C_InitStruct.I2C_OwnAddress1 = 0x50 << 1;                 // Any address will do. I just chose the same address as the EEPROM.
                                                                // Note I left shifted the address. That's because the address is supposed
                                                                // to be 7-bits and the least significant bit indicates the R/W setting
	I2C_Init(I2C3, &I2C_InitStruct);
	I2C_Cmd(I2C3, ENABLE);
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param   deviceAddr : the device/IC that the data is intended for.
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
    int timeout_count = 0;
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)){
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	I2C_AcknowledgeConfig(I2C3, ENABLE);

	// First write address that you want to read from

	writePoll:
	// Since no one is using the I2C bus, take control
	I2C_GenerateSTART(I2C3, ENABLE);
	// Wait until start edge event occurred
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	// Select device to talk to
	I2C_Send7bitAddress(I2C3, deviceAddr, I2C_Direction_Transmitter);		// Sets RW bit to 0
	

	// THIS IS WHERE WE GOT STUCK
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
		if(I2C3->SR1 & 0x0400) {
			I2C3->SR1 &= ~0x0400;
			I2C_GenerateSTOP(I2C3, ENABLE);
			goto writePoll;
		}
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	// Send start address (MSB first)
	I2C_SendData(I2C3, (uint8_t)((regAddr & 0xFF00) >> 8));
	// Wait until transmit event occurred.
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}
	// Send rest of start address (LSB)
	I2C_SendData(I2C3, (uint8_t)(regAddr & 0x00FF));
	// Wait until transmit event occurred.
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	// Wait until byte transmission finished
	timeout_count = 0;
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	// Start writing data sequentially
	while(txLen > 0){

		// Wait until BTF flag is set
		timeout_count = 0;
		while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET);
		I2C_SendData(I2C3, *txData);
		txData++;
		txLen--;
		
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	I2C_GenerateSTOP(I2C3, ENABLE);
	return SUCCESS;
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   deviceAddr : the device/IC that the data needs to be read from.
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, other if success
 */
uint8_t BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
    int timeout_count = 0;
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY)){
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
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
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
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
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	// Send start address (MSB first)
	I2C_SendData(I2C3, (uint8_t)((regAddr & 0xFF00) >> 8));
	// Wait until transmit event occurred.
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}
	// Send rest of start address (LSB)
	I2C_SendData(I2C3, (uint8_t)(regAddr & 0x00FF));
	// Wait until transmit event occurred.
	timeout_count = 0;
	while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTING)) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	// Wait until byte transmission finished
	timeout_count = 0;
	while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET) {
		// Assume running at 80 MHz
		timeout_count++;
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
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
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
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
		// Returns and breaks after timeout threshold
		if(timeout_count > TIMEOUT_THRESHOLD) {
			return ERROR;
		}
	}

	while(rxLen > 0){
		if(rxLen > 1){
			// Wait until byte transmission finished
			timeout_count = 0;
			while(I2C_GetFlagStatus(I2C3, I2C_FLAG_BTF) == RESET) {
				// Assume running at 80 MHz
				timeout_count++;
				// Returns and breaks after timeout threshold
				if(timeout_count > TIMEOUT_THRESHOLD) {
					return ERROR;
				}
			}
			*rxData = I2C_ReceiveData(I2C3);
			rxData++;
			rxLen--;
		} else {
			// Disable ack, since this is the last byte
			I2C_AcknowledgeConfig(I2C3, DISABLE);
		
			timeout_count = 0;
			while(I2C_GetFlagStatus(I2C3, I2C_FLAG_RXNE) == RESET) {
				// Assume running at 80 MHz
				timeout_count++;
				// Returns and breaks after timeout threshold
				if(timeout_count > TIMEOUT_THRESHOLD) {
					return ERROR;
				}
			}

			*rxData = I2C_ReceiveData(I2C3);
			rxData++;
			rxLen = 0;
		}
	}

	// Generate the stop
	I2C_GenerateSTOP(I2C3, ENABLE);
	
	// Return Success if all executed properly
	return SUCCESS;
}
