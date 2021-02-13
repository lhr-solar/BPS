/* Copyright (c) 2020 UT Longhorn Racing Solar */

/** EEPROM.h
 * Driver for EEPROM (STM's M24128). Required by the American Solar Challanged
 * in the regulations, all safety critical data is stored in here. This includes'
 * over voltage, under voltage, max current, max temperature.
 * The EEPROM is 128Kbits (16k x 8 bits) and uses I2C
 */

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

#define EEPROM_ADDRESS (0xA0)

//error codes
#define FAULT_HIGH_TEMP			0x01
#define FAULT_HIGH_VOLT			0x02
#define FAULT_LOW_VOLT			0x04
#define FAULT_HIGH_CURRENT	    0x08
#define FAULT_WATCHDOG			0x10
#define FAULT_CAN_BUS			0x20
#define FAULT_VOLT_MISC			0x40
#define DATA_SOC				0x80
#define FAULT_OPEN_WIRE         0x03
#define FAULT_HANDLER           0x05
#define FAULT_OS_ERROR          0x06

#define EEPROM_TERMINATOR		0xFF

#define EEPROM_FAULT_PTR_LOC    0x1000
#define EEPROM_TEMP_PTR_LOC     0x1002
#define EEPROM_VOLT_PTR_LOC	    0x1004
#define EEPROM_CURRENT_PTR_LOC  0x1006
#define EEPROM_WATCHDOG_PTR_LOC 0x1008
#define EEPROM_CAN_PTR_LOC		0x100A
#define EEPROM_SOC_PTR_LOC		0x100C
#define EEPROM_OW_PTR_LOC       0X100E
#define EEPROM_HANDLER_PTR_LOC  0X1010
#define EEPROM_OS_PTR_LOC       0x1012

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void);

/** EEPROM_Load
 * Loads the pointers from memory. Should be done after initialization and before reading or 
 * writing to the EEPROM.
 */
void EEPROM_Load(void);

/** EEPROM_Reset
 * Resets the EEPROM. This will clear all data that has been logged so far. This is required for 
 * clearing data and setting up new EEPROMs. This CMD really should be reserved for specialized
 * set-up programs and the CLI.
 */
void EEPROM_Reset(void);

/** EEPROM_LogData
 * This logs the given data at an appropriate spot in memory. This does not log an actual error, 
 * but allows more specific information about failed modules to be logged.
 * @param logType is the type of fault that occured (fault code)
 * @param data is the data corresponding to the logType, 0 if nothing to be logged
 */
void EEPROM_LogData(uint8_t logType, uint8_t data);

/** EEPROM_Save
 * Save some information to the EEPROM
 * @param logType is type of fault (fault code)
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
 * @param address to write data to
 * @param bytes number of bytes to send
 * @param buffer pointer to buffer that contains data
 */
void EEPROM_WriteMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer);

/** EEPROM_Write
 * Saves data to the EEPROM at the specified address
 * @param unsigned to write data to
 * @param unsigned data to send
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data);

/** EEPROM_ReadMultipleBytes
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param address to read data from
 * @param bytes number of bytes to read
 * @param buffer pointer to buffer to store data in
 */
void EEPROM_ReadMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer);

/** EEPROM_ReadByte
 * Gets single bytes of data sequentially from EEPROM beginning at specified address
 * @param address to read data from
 * @return unsigned 8-bit data that was read
 */
uint8_t EEPROM_ReadByte(uint16_t address);

/** DelayMS
 * Delays for specified number of milliseconds (not accurate)
 * @param ms is number of milliseconds to delay
 */
void DelayMs(uint32_t ms);

#endif
