/** EEPROM.c
 * Driver for EEPROM
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

/** EEPROM_Save
 * Save some information to the EEPROM
 */
void EEPROM_Save(uint8_t logType, uint8_t data){

}

/** EEPROM_WriteByte
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned byte of data
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data){
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

/** EEPROM_ReadByte
 * Gets single bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @return unsigned 8-bit data
 */
uint8_t EEPROM_ReadByte(uint16_t address){
	return I2C3_Read(EEPROM_ADDRESS, address);
}
