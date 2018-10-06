// EEPROM.c
/**
 * Driver for EEPROM. Required by the American Solar Challanged in the regulations,
 * all safety critical data is stored in here. This includes over voltage, under
 * voltage, max current, max temperature.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/4/2018
 */

#include "EEPROM.h"
#include "stm32f4xx.h"
#include <stdlib.h>

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO port A clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  // 1.5) Initialize the I2C port clock
	
	// I2C GPIO initializaion
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;		// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;							// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;					// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);		// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);		// SDA
	
	// GPIO device selection initialization
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
																											// 7) Initialize pins for device selection address
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						// 8) PA10-PA12 set to regular pins
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 9) Set to push-pull
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2C_InitStruct.I2C_ClockSpeed = 400000;								// 10) Set i2c to operate at 400000 Hz (probably)
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;								// 11) regular I2C
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;				// 12) Duty cycle of 2
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;							// 13) Enable acknowledge option
	I2C_InitStruct.I2C_OwnAddress1 = 0x004;								// 14) Set own address equal to 4
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
																											// 15) 7-bit acknowledge address
	I2C_Init(I2C2, &I2C_InitStruct);
	I2C_Cmd(I2C2, ENABLE);
}

/** EEPROM_Write
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned 8-bit address
 */
void EEPROM_Write(uint16_t address, uint8_t data){
//	I2C_Send7bitAddress(I2C3, EEPROM_ADDRESS, I2C_Direction_Transmitter);	// Address the EEPROM
	I2C_SendData(I2C2, EEPROM_ADDRESS << 1);	// This sends the address plus a write bit (0x0)
			// I don't know if these two lines do the same thing or not.
	I2C_SendData(I2C2, address >> 8);					// Send the first byte for the address
	I2C_SendData(I2C2, address & 0xFF);				// Send the second byte for the address
	I2C_SendData(I2C2, data);									// Send the byte of data
}

/** EEPROM_ReadMultipleBytes
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @param number of bytes to read
 * @return unsigned 8-bit list of data
 */
uint8_t *EEPROM_ReadMultipleBytes(uint16_t address, uint32_t bytes){
	for(int i = 0; i < bytes; ++i){
	//	data[i] = EEPROM_ReadSingleByte(address + i);
	}
	
	//return data;
}

/** EEPROM_ReadSingleByte
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @return unsigned 8-bit data
 */
uint8_t EEPROM_ReadSingleByte(uint16_t address){
	return I2C_ReceiveData(I2C3);
}
