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

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void){
	I2C3_Init();
}

/** EEPROM_Save
 * Save some information to the EEPROM
 * logType is type of fault (fault code)
 * data is additional information (currently not being used)
 */
void EEPROM_Save(uint8_t logType, uint8_t data){
	//initialize pointers to data in EEPROM
	static uint16_t faultCodePtr = EEPROM_FAULT_CODE_ADDR;
	static uint16_t tempFaultPtr = EEPROM_TEMP_FAULT;
	static uint16_t voltFaultPtr = EEPROM_VOLT_FAULT;
	static uint16_t currentFaultPtr = EEPROM_CURRENT_FAULT;
	static uint16_t watchdogFaultPtr = EEPROM_WWDG_FAULT;
	static uint16_t canFaultPtr = EEPROM_CAN_FAULT;
	uint16_t *battery_modules;
//	uint16_t *temperature_modules;
	uint16_t fake_battery_modules[] = {1,2,3,4,8,9,10};
	if (faultCodePtr < (EEPROM_TEMP_FAULT - 1)) {		//only store errors if there have been less than 256 faults (so buffers don't overflow)

		EEPROM_WriteByte(faultCodePtr, logType);
		//delay(2);
		faultCodePtr++;
		EEPROM_WriteByte(faultCodePtr, 0xff);		//terminate array with 0xff
		//delay(2);
		switch (logType) {
			
			case FAULT_HIGH_TEMP:
				//write which temperature sensor faulted to EEPROM
//				battery_modules = Temperature_GetModulesInDanger();			//commented out for tesing purposes
				battery_modules = fake_battery_modules;
				// iterate through array and store bad modules
				for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++)
				{
					if (battery_modules[i] == 1)
					{
						EEPROM_WriteByte(tempFaultPtr, i);
						//delay(2);
						tempFaultPtr ++;
					}
				}
				//terminate each entry with 0xfe 
				EEPROM_WriteByte(tempFaultPtr, 0xfe);
				//delay(2);
				tempFaultPtr++;
				//terminate entire array with 0xff
				EEPROM_WriteByte(tempFaultPtr, 0xff);
				//delay(2);
				break;
			
			case FAULT_HIGH_VOLT:
				//write which voltage sensor faulted to EEPROM
//				battery_modules = Voltage_GetModulesInDanger();		//commented out for testing
				battery_modules = fake_battery_modules;			//for testing purposes only
				//iterate through array and store bad modules
				for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){
					if (battery_modules[i] == 1){		//if module is in danger, write module index to array
						EEPROM_WriteByte(voltFaultPtr, i);
						//delay(2);
						voltFaultPtr++;
					}
				}
				//terminate each entry with 0xfe 
				EEPROM_WriteByte(voltFaultPtr, 0xfe);
				//delay(2);
				voltFaultPtr++;
				//terminate entire array with 0xff
				EEPROM_WriteByte(voltFaultPtr, 0xff);
				//delay(2);
				break;
				
			case FAULT_LOW_VOLT:
				//write which voltage sensor faulted to EEPROM
//				battery_modules = Voltage_GetModulesInDanger();			//commented out for testing
				battery_modules = fake_battery_modules;			//for testing purposes only
				//iterate through array and store bad modules
				for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){
					if (battery_modules[i] == 1){		//if module is in danger, write module index to array
						EEPROM_WriteByte(voltFaultPtr, i);
						//delay(2);
						voltFaultPtr++;
					}
				}
				//terminate each entry with 0xfe 
				EEPROM_WriteByte(voltFaultPtr, 0xfe);
				//delay(2);
				voltFaultPtr++;
				//terminate entire array with 0xff
				EEPROM_WriteByte(voltFaultPtr, 0xff);
				//delay(2);
				break;
				
			case FAULT_HIGH_CURRENT:
				//write info about current sensor to EEPROM
				break;
			
			case FAULT_WATCHDOG:
				//write information about watchdog timer to EEPROM
				break;
			
			case FAULT_CAN_BUS:
				// write info about CAN bus to EEPROM
				break;
		}
	}
}

/** EEPROM_SerialPrintData
 * Prints saved data from EEPROM to serial terminal
 */
void EEPROM_SerialPrintData(void){
	//UART1_Init(115200);
	uint8_t fault_code;
	uint8_t data;
	printf("z1\n");
	uint16_t fault_ptr = EEPROM_FAULT_CODE_ADDR;
	uint16_t data_ptr;
	fault_code = EEPROM_ReadByte(fault_ptr);	//read first fault code
	printf("%d\n", fault_code);
	while (fault_code != 0xff) {
		printf("%d\n", fault_code);
		switch (fault_code) {
			case FAULT_HIGH_TEMP:
				printf("fault, high temperature\nmodules in danger: ");
				data_ptr = EEPROM_TEMP_FAULT;
			  data = EEPROM_ReadByte(data_ptr);
				while (data != 0xfe) {
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
				while (data != 0xfe) {
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
				while (data != 0xfe) {
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
		fault_code = EEPROM_ReadByte(fault_ptr);
	}
	printf("done\n\r");
}

/** EEPROM_Tester
 * sends fake error messages to test EEPROM
*/
void EEPROM_Tester(void){
	//fake error messages
	//note: the second parameter does not do anything currently
	printf("x0\n");
	EEPROM_Save(FAULT_HIGH_TEMP, 0xff);
	printf("x1\n");
	EEPROM_Save(FAULT_HIGH_VOLT, 0x04);
	printf("x2\n");
	EEPROM_Save(FAULT_LOW_VOLT, 0x00);
	printf("x3\n");
	EEPROM_Save(FAULT_HIGH_CURRENT, 0x45);
	printf("x4\n");
	EEPROM_Save(FAULT_WATCHDOG, 0x64);
	printf("x5\n");
	EEPROM_Save(FAULT_CAN_BUS, 0x9e);
	printf("x6\n");
	EEPROM_Save(FAULT_HIGH_VOLT, 0xd3);
	printf("x7\n");
	EEPROM_Save(FAULT_HIGH_TEMP, 0xc5);
	
	//test to see if it will let me overflow the buffer
	// for (uint16_t i = 0; i < 1000; i++) {
		// EEPROM_Save(FAULT_CAN_BUS, 0x00);
	// }
}
	
/** EEPROM_WriteByte
 * Saves data to the EEPROM at the specified address
 * @param unsigned 16-bit address
 * @param unsigned byte of data
 */
void EEPROM_WriteByte(uint16_t address, uint8_t data){
	I2C3_Write(EEPROM_ADDRESS, address, data);
	printf("byte written\n\r");
	for(uint32_t delay = 0; delay < 10000; delay++){};
}

/** EEPROM_ReadMultipleBytes
 * Gets multiple bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @param number of bytes to read
 * @return unsigned 8-bit list of data
 */
void EEPROM_ReadMultipleBytes(uint16_t address, uint32_t bytes, uint8_t* buffer){
	I2C3_ReadMultiple(EEPROM_ADDRESS, address, buffer, bytes);
	printf("byte read\n\r");
	for(uint32_t delay = 0; delay < 10000; delay++){};
	
}

/** EEPROM_ReadByte
 * Gets single bytes of data sequentially from EEPROM beginning at specified address
 * @param unsigned 16-bit address
 * @return unsigned 8-bit data
 */
uint8_t EEPROM_ReadByte(uint16_t address){
	return I2C3_Read(EEPROM_ADDRESS, address);
}


