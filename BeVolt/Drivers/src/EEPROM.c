/** EEPROM.c
 * Driver for EEPROM
 */

#include "EEPROM.h"
#include "stm32f4xx.h"
#include "I2C.h"
#include <stdlib.h>
#include "config.h"
#include "Voltage.h"
#include "Temperature.h"
#include "UART.h"
#include <stdio.h>

//starting addresses for errors
const uint16_t EEPROM_FAULT_CODE_ADDR = 0x0000;
const uint16_t EEPROM_TEMP_FAULT  		= 0x0100;
const uint16_t EEPROM_VOLT_FAULT		  = 0x0400;
const uint16_t EEPROM_CURRENT_FAULT	  = 0x0600;
const uint16_t EEPROM_WWDG_FAULT		  = 0x0700;
const uint16_t EEPROM_CAN_FAULT			  = 0x0800;
const uint16_t EEPROM_SOC_DATA				= 0x0900;

static uint16_t faultCodePtr,
								tempFaultPtr,
								voltFaultPtr,
								currentFaultPtr,
								watchdogFaultPtr,
								canFaultPtr,
								socDataPtr;

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 * This involves updating the fault code pointers based on the
 * addresses specified in the EEPROM.
 */
void EEPROM_Init(void){
	// Initialize the I2C driver
	I2C3_Init();
	
	EEPROM_Load();
}

void EEPROM_Load(void) {
	// Load the pointers
	EEPROM_ReadMultipleBytes(EEPROM_FAULT_PTR_LOC,   2, (uint8_t*)&faultCodePtr);
	EEPROM_ReadMultipleBytes(EEPROM_TEMP_PTR_LOC,    2, (uint8_t*)&tempFaultPtr);
	EEPROM_ReadMultipleBytes(EEPROM_VOLT_PTR_LOC,    2, (uint8_t*)&voltFaultPtr);
	EEPROM_ReadMultipleBytes(EEPROM_CURRENT_PTR_LOC, 2, (uint8_t*)&currentFaultPtr);
	EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC,2, (uint8_t*)&watchdogFaultPtr);
	EEPROM_ReadMultipleBytes(EEPROM_CAN_PTR_LOC,     2, (uint8_t*)&canFaultPtr);
	EEPROM_ReadMultipleBytes(EEPROM_SOC_PTR_LOC,		 2, (uint8_t*)&socDataPtr);
}

/** EEPROM_Reset
 * Resets all EEPROM messages
 *
 * This SHOULD NOT be called from EEPROM_Init()
 */
void EEPROM_Reset(void) {
	// Reset the error pointers in the EEPROM memory
	EEPROM_WriteMultipleBytes(EEPROM_FAULT_PTR_LOC,   2, (uint8_t*)&EEPROM_FAULT_CODE_ADDR);
	EEPROM_WriteMultipleBytes(EEPROM_TEMP_PTR_LOC,    2, (uint8_t*)&EEPROM_TEMP_FAULT);
	EEPROM_WriteMultipleBytes(EEPROM_VOLT_PTR_LOC,    2, (uint8_t*)&EEPROM_VOLT_FAULT);
	EEPROM_WriteMultipleBytes(EEPROM_CURRENT_PTR_LOC, 2, (uint8_t*)&EEPROM_CURRENT_FAULT);
	EEPROM_WriteMultipleBytes(EEPROM_WATCHDOG_PTR_LOC,2, (uint8_t*)&EEPROM_WWDG_FAULT);
	EEPROM_WriteMultipleBytes(EEPROM_CAN_PTR_LOC,     2, (uint8_t*)&EEPROM_CAN_FAULT);
	EEPROM_WriteMultipleBytes(EEPROM_SOC_PTR_LOC,			2, (uint8_t*)&EEPROM_SOC_DATA);
	
	// Reintialize pointers in the program, etc.
	EEPROM_Load();
	
	// Reterminate the EEPROM
	EEPROM_WriteByte(faultCodePtr,    EEPROM_TERMINATOR);
	EEPROM_WriteByte(tempFaultPtr,    EEPROM_TERMINATOR);
	EEPROM_WriteByte(voltFaultPtr,    EEPROM_TERMINATOR);
	EEPROM_WriteByte(currentFaultPtr, EEPROM_TERMINATOR);
	EEPROM_WriteByte(watchdogFaultPtr,EEPROM_TERMINATOR);
	EEPROM_WriteByte(canFaultPtr,     EEPROM_TERMINATOR);
	EEPROM_WriteByte(socDataPtr, 			EEPROM_TERMINATOR);
}

/*
 * This only logs the data, not the actual error
 */
void EEPROM_LogData(uint8_t logType, uint8_t data) {
	// Write the data byte,
	// Write the terminator byte,
	// Write the new ptr to memory
	
	switch(logType) {
	case FAULT_HIGH_TEMP: 
		EEPROM_WriteByte(tempFaultPtr++, data);
		EEPROM_WriteByte(tempFaultPtr, EEPROM_TERMINATOR);
		EEPROM_WriteByte(EEPROM_TEMP_PTR_LOC, tempFaultPtr);
		break;
	case FAULT_HIGH_VOLT:
	case FAULT_LOW_VOLT:
	case FAULT_VOLT_MISC:
		EEPROM_WriteByte(voltFaultPtr++, data);
		EEPROM_WriteByte(voltFaultPtr, EEPROM_TERMINATOR);
		EEPROM_WriteByte(EEPROM_VOLT_PTR_LOC, voltFaultPtr);
		break;
	case FAULT_HIGH_CURRENT:
		EEPROM_WriteByte(currentFaultPtr++, data);
		EEPROM_WriteByte(currentFaultPtr, EEPROM_TERMINATOR);
		EEPROM_WriteByte(EEPROM_CURRENT_PTR_LOC, currentFaultPtr);
		break;
	case FAULT_WATCHDOG:
		EEPROM_WriteByte(watchdogFaultPtr++, data);
		EEPROM_WriteByte(watchdogFaultPtr, EEPROM_TERMINATOR);
		EEPROM_WriteByte(EEPROM_WATCHDOG_PTR_LOC, watchdogFaultPtr);
		break;
	case FAULT_CAN_BUS:
		EEPROM_WriteByte(canFaultPtr++, data);
		EEPROM_WriteByte(canFaultPtr, EEPROM_TERMINATOR);
		EEPROM_WriteByte(EEPROM_CAN_PTR_LOC, canFaultPtr);
		break;
	case DATA_SOC:
		EEPROM_WriteByte(socDataPtr++, data);
		EEPROM_WriteByte(socDataPtr, EEPROM_TERMINATOR);
		EEPROM_WriteByte(EEPROM_CAN_PTR_LOC, socDataPtr);
		break;
	}
}

/** EEPROM_LogError
 * Save some information to the EEPROM
 * logType is type of fault (fault code)
 * data is additional information to be used; if this data is 0,
 *		nothing will be logged.
 */
void EEPROM_LogError(uint8_t logType){	
	if (faultCodePtr < (EEPROM_TEMP_FAULT - 1)) {		//only store errors if there have been less than 256 faults (so buffers don't overflow)
		// log the data
		EEPROM_WriteByte(faultCodePtr, logType);
		
		// Increment the ptr and write the terminator byte
		faultCodePtr++;
		EEPROM_WriteByte(faultCodePtr, EEPROM_TERMINATOR);
		
		// Update the pointer in the EEPROM for future boots
		EEPROM_WriteByte(EEPROM_FAULT_PTR_LOC, faultCodePtr);
	}
}

/** EEPROM_SerialPrintData
 * Prints saved data from EEPROM to serial terminal (putty)
 */
void EEPROM_SerialPrintData(void){
	UART1_Init(115200);
	uint8_t fault_code;
	uint8_t data;
	uint16_t fault_ptr = EEPROM_FAULT_CODE_ADDR;
	uint16_t data_ptr;
	fault_code = EEPROM_ReadByte(fault_ptr);	//read first fault code
	//printf("fault code: %d\n\r", fault_code);
	while (fault_code != EEPROM_TERMINATOR) {
		printf("fault code: %d\n\r", fault_code);
		switch (fault_code) {
			case FAULT_HIGH_TEMP:
				printf("fault, high temperature\n\rmodules in danger: ");
				data_ptr = EEPROM_TEMP_FAULT;
			  data = EEPROM_ReadByte(data_ptr);
				while (data != EEPROM_TERMINATOR) {
					printf("%d, ", data);
					//read next module in danger
					data_ptr++;
					data = EEPROM_ReadByte(data_ptr);
				}
				printf("\n\r");
				break;
			case FAULT_LOW_VOLT:
				printf("fault, low voltage\n\rmodules in danger: ");
				data_ptr = EEPROM_VOLT_FAULT;
				data = EEPROM_ReadByte(data_ptr);
				while (data != EEPROM_TERMINATOR) {
					printf("%d, ", data);
					//read next module in danger
					data_ptr++;
					data = EEPROM_ReadByte(data_ptr);
				}
				printf("\n\r");
				break;
			case FAULT_HIGH_VOLT:
				printf("fault, high voltage\n\rmodules in danger: ");
				data_ptr = EEPROM_VOLT_FAULT;
				data = EEPROM_ReadByte(data_ptr);
				while (data != EEPROM_TERMINATOR) {
					printf("%d, ", data);
					//read next module in danger
					data_ptr++;
					data = EEPROM_ReadByte(data_ptr);
				}
				printf("\n\r");
				break;
			case FAULT_HIGH_CURRENT:
				printf("fault, high current\n\r");
				break;
			case FAULT_CAN_BUS:
				printf("fault, CAN bus\n\r");
				break;
			case FAULT_WATCHDOG:
				printf("fault, watchdog timer\n\r");
				break;
		}
		//read next fault code
		fault_ptr++;
		printf("fault_ptr: %d\n\r", fault_ptr);
		fault_code = EEPROM_ReadByte(fault_ptr);
	}
	printf("done\n\r");
}

/** EEPROM_Tester
 * sends fake error messages to test EEPROM
* TODO: we don't need a tester function in the drive rmodule itself
*/
void EEPROM_Tester(void){
	//fake error messages
	//note: the second parameter does not do anything currently
	printf("x0\n\r");
	//EEPROM_LogError(FAULT_HIGH_TEMP, 0xff);
	EEPROM_LogError(FAULT_HIGH_CURRENT);
	printf("x1\n\r");
	//EEPROM_LogError(FAULT_HIGH_VOLT, 0x04);
	EEPROM_LogError(FAULT_WATCHDOG);
	printf("x2\n\r");
	EEPROM_LogError(FAULT_HIGH_CURRENT);
	//EEPROM_LogError(FAULT_LOW_VOLT, 0x00);
	printf("x3\n\r");
	EEPROM_LogError(FAULT_HIGH_CURRENT);
	printf("x4\n\r");
	EEPROM_LogError(FAULT_WATCHDOG);
	printf("x5\n\r");
	EEPROM_LogError(FAULT_CAN_BUS);
	printf("x6\n\r");
	EEPROM_LogError(FAULT_HIGH_VOLT);
	printf("x7\n\r");
	EEPROM_LogError(FAULT_HIGH_TEMP);
}
	
/** EEPROM_WriteByte
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned byte of data
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data){
	I2C3_Write(EEPROM_ADDRESS, address, data);
	for(uint32_t delay = 0; delay < 50000; delay++){};
}

void EEPROM_WriteMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer){
	I2C3_WriteMultiple(EEPROM_ADDRESS, address, buffer, bytes);
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
	uint8_t result = I2C3_Read(EEPROM_ADDRESS, address);
	//printf("read %d from %x\n\r", result, address);
	return result;
	//return I2C3_Read(EEPROM_ADDRESS, address);
}
