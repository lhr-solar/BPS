/** EEPROM.h
 * Driver for EEPROM (STM's M24128). Required by the American Solar Challanged
 * in the regulations, all safety critical data is stored in here. This includes'
 * over voltage, under voltage, max current, max temperature.
 * The EEPROM is 128Kbits (16k x 8 bits) and uses I2C
 */

#ifndef EEPROM_H__
#define EEPROM_H__

#include <stdint.h>

#define EEPROM_ADDRESS 0x50 << 1

//starting addresses for errors
#define EEPROM_FAULT_CODE_ADDR 0x00000
#define EEPROM_TEMP_FAULT  		 0x00100
#define EEPROM_VOLT_FAULT		   0x04000
#define EEPROM_CURRENT_FAULT	 0x08000
#define EEPROM_WWDG_FAULT		   0x08100
#define EEPROM_CAN_FAULT			 0x08200

//error codes
#define FAULT_HIGH_TEMP			0x01
#define FAULT_HIGH_VOLT			0x02
#define FAULT_LOW_VOLT			0x04
#define FAULT_HIGH_CURRENT	0x08
#define FAULT_WATCHDOG			0x10
#define FAULT_CAN_BUS				0x20

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void);

/** EEPROM_Save
 * Save some information to the EEPROM
 * logType is type of fault (error code)
 * data is additional information (which sensor tripped fault)
 */
void EEPROM_Save(uint8_t logType, uint8_t data);

/** EEPROM_Write
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned 8-bit address
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data);

/** EEPROM_ReadMultipleBytes
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @param number of bytes to read
 * @return unsigned 8-bit list of data
 */
void EEPROM_ReadMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer);

/** EEPROM_ReadByte
 * Gets single bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @return unsigned 8-bit data
 */
uint8_t EEPROM_ReadByte(uint16_t address);

#endif
