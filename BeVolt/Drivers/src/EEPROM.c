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
	uint16_t *temperature_modules;
	
	if (faultCodePtr < (EEPROM_TEMP_FAULT - 1)) {		//only store errors if there have been less than 256 faults (so buffers don't overflow)
		EEPROM_WriteByte(faultCodePtr, logType);
		faultCodePtr++;
		EEPROM_WriteByte(faultCodePtr, 0xff);		//terminate array with 0xff
	
		switch (logType) {
			
			case FAULT_HIGH_TEMP:
				//write which temperature sensor faulted to EEPROM
				temperature_modules = Temperature_GetModulesInDanger();
				// iterate through array and store bad modules
				for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++)
				{
					if (battery_modules[i] == 1)
					{
						EEPROM_WriteByte(tempFaultPtr, i);
						tempFaultPtr ++;
					}
				}
				//terminate each entry with 0xfe 
				EEPROM_WriteByte(tempFaultPtr, 0xfe);
				tempFaultPtr++;
				//terminate entire array with 0xff
				EEPROM_WriteByte(tempFaultPtr, 0xff);
				break;
			
			case FAULT_HIGH_VOLT:
				//write which voltage sensor faulted to EEPROM
				battery_modules = Voltage_GetModulesInDanger();
				//iterate through array and store bad modules
				for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){
					if (battery_modules[i] == 1){		//if module is in danger, write module index to array
						EEPROM_WriteByte(voltFaultPtr, i);
						voltFaultPtr++;
					}
				}
				//terminate each entry with 0xfe 
				EEPROM_WriteByte(voltFaultPtr, 0xfe);
				voltFaultPtr++;
				//terminate entire array with 0xff
				EEPROM_WriteByte(voltFaultPtr, 0xff);
				break;
				
			case FAULT_LOW_VOLT:
				//write which voltage sensor faulted to EEPROM
				battery_modules = Voltage_GetModulesInDanger();
				//iterate through array and store bad modules
				for (uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){
					if (battery_modules[i] == 1){		//if module is in danger, write module index to array
						EEPROM_WriteByte(voltFaultPtr, i);
						voltFaultPtr++;
					}
				}
				//terminate each entry with 0xfe 
				EEPROM_WriteByte(voltFaultPtr, 0xfe);
				voltFaultPtr++;
				//terminate entire array with 0xff
				EEPROM_WriteByte(voltFaultPtr, 0xff);
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
