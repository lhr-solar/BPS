/** EEPROM.h
 * Driver for EEPROM (STM's M24128). Required by the American Solar Challanged
 * in the regulations, all safety critical data is stored in here. This includes'
 * over voltage, under voltage, max current, max temperature.
 * The EEPROM is 128Kbits (16k x 8 bits) and uses I2C
 */

#ifndef EEPROM_H__
#define EEPROM_H__

#include <stdint.h>

#define EEPROM_ADDRESS (0xA0)

//error codes
#define FAULT_HIGH_TEMP			0x01
#define FAULT_HIGH_VOLT			0x02
#define FAULT_LOW_VOLT			0x04
#define FAULT_HIGH_CURRENT	0x08
#define FAULT_WATCHDOG			0x10
#define FAULT_CAN_BUS				0x20
#define FAULT_VOLT_MISC			0x40
#define DATA_SOC						0x80

#define EEPROM_TERMINATOR		0xFF

#define EEPROM_FAULT_PTR_LOC    0x1000
#define EEPROM_TEMP_PTR_LOC     0x1002
#define EEPROM_VOLT_PTR_LOC	    0x1004
#define EEPROM_CURRENT_PTR_LOC  0x1006
#define EEPROM_WATCHDOG_PTR_LOC 0x1008
#define EEPROM_CAN_PTR_LOC		  0x100A
#define EEPROM_SOC_PTR_LOC			0x100C

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void);

/** EEPROM_Load
 * Loads the pointers from memory. Should be done after
 * initialization and before reading or writing to the
 * EEPROM.
 */
void EEPROM_Load(void);

/** EEPROM_Reset
 * Resets the EEPROM. This will clear all data that has
 * been logged so far.
 *
 * This is required for clearing data and setting up new
 * EEPROMs. This cmd really should be reserved for specialized
 * set-up programs and the cli.
 */
void EEPROM_Reset(void);

/** EEPROM_LogData
 * This logs the given data at an appropriate spot in memory.
 * This does not log an actual error, but allows more specific
 * information about failed modules to be logged.
 */
void EEPROM_LogData(uint8_t logType, uint8_t data);

/** EEPROM_Save
 * Save some information to the EEPROM
 * logType is type of fault (fault code)
 */
void EEPROM_LogError(uint8_t logType);

/** EEPROM_SerialPrintData
 * Prints saved data from EEPROM to serial terminal
 */
void EEPROM_SerialPrintData(void);

/** EEPROM_Tester
 * sends fake error messages to test EEPROM
 * TODO: take this out of the EEPROM files and with the rest of the test files
*/
void EEPROM_Tester(void);

/** EEPROM_WriteMultipleBytes
 * Saves data to the EEPROM
 * @param unsigned 16-bit address
 * @param unsigned 8-bit data
 */
void EEPROM_WriteMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer);

/** EEPROM_Write
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned 8-bit data
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
