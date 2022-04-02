/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "EEPROM.h"

/** EEPROM_SerialPrintData
 * Prints saved data from EEPROM to serial terminal (putty)
 */
void EEPROM_SerialPrintData(void){
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
			case FAULT_OPEN_WIRE:
				printf("fault, open wire\n\rmodules in danger: ");
				data_ptr = EEPROM_OW_FAULT;
				data = EEPROM_ReadByte(data_ptr);
				while (data != EEPROM_TERMINATOR){
					printf("%d ", data);
					data_ptr++;
					data = EEPROM_ReadByte(data_ptr);
				}
				printf("\n\r");
				break;
			case FAULT_OS_ERROR:
				printf("fault, os error\n\r");
				break;
			case FAULT_HANDLER:
				printf("fault, handler\n\r");
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
