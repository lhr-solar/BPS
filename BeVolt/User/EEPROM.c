// EEPROM.c
/**
 * Driver for EEPROM
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/4/2018
 */

#include "EEPROM.h"
#include "stm32f4xx.h"
#include "I2C.h"
#include <stdlib.h>

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void){
	I2C3_Init();
}

/** EEPROM_Log
 * Logs some information to the EEPROM
 */
void EEPROM_Log(uint8_t logType, uint8_t data){
	uint8_t fault_stat = EEPROM_Read(EEPROM_FAULT_CODE_ADDR);
	
	switch(logType){
		case EEPROM_TEMP_FAULT:
			fault_stat |= 0x01;
			break;
		case EEPROM_VOLT_FAULT:
			fault_stat |= 0x06;
			break;
		case EEPROM_CURRENT_FAULT:
			fault_stat |= 0x08;
			break;
		case EEPROM_WWDG_FAULT:
			fault_stat |= 0x10;
			break;
		case EEPROM_CAN_FAULT:
			fault_stat |= 0x20;
			break;
		default:
			return;
	}
	EEPROM_Write(EEPROM_FAULT_CODE_ADDR, fault_stat);	// Update the fault address
	
	EEPROM_Write(logType + (data / 8), 1 << (data % 8)); // Update the more specific address.
}

/** EEPROM_Write
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned byte of data
 */
void EEPROM_Write(uint16_t address, uint8_t data){
	I2C3_Write(EEPROM_ADDRESS, address, data);
}

/** EEPROM_ReadMultipleBytes
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @param number of bytes to read
 * @return unsigned 8-bit list of data
 */
void EEPROM_ReadMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer){
	I2C3_ReadMultiple(EEPROM_ADDRESS, address, buffer, bytes);
	
}

/** EEPROM_ReadSingleByte
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @return unsigned 8-bit data
 */
uint8_t EEPROM_ReadSingleByte(uint16_t address){
	return I2C3_Read(EEPROM_ADDRESS, address);
}
