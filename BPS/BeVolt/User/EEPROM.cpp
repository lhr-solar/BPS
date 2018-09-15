// EEPROM.cpp
/**
 * Driver for EEPROM. Required by the American Solar Challanged in the regulations,
 * all safety critical data is stored in here. This includes over voltage, under
 * voltage, max current, max temperature.
 * @authors Sijin Woo
 * @lastRevised 9/4/2018
 */

#include "EEPROM.h"

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
 void EEPROM_Init(void){
	
}

/** EEPROM_Save
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned 8-bit address
 */
void EEPROM_Save(uint16_t address, uint8_t data){
	
}

/** EEPROM_ReadMultipleBytes
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @param number of bytes to read
 * @return unsigned 8-bit list of data
 */
uint8_t *EEPROM_ReadMultipleBytes(uint16_t address, uint32_t bytes){
	
}

/** EEPROM_ReadSingleByte
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @return unsigned 8-bit data
 */
uint8_t EEPROM_ReadSingleByte(uint16_t address){
	
}
