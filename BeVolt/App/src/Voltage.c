/** Voltage.c
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 */

#include "Voltage.h"
#include "LTC6811.h"
#include "config.h"
#include <stdlib.h>

cell_asic Modules[NUM_VOLTAGE_BOARDS];

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_Init(void){
	int8_t error = 0;
	
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
	LTC6811_Init(Modules);
	
	// Write Configuration Register
	LTC6811_wrcfg(NUM_VOLTAGE_BOARDS, Modules);

	// Read Configuration Register
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
	error = LTC6811_rdcfg(NUM_VOLTAGE_BOARDS, Modules);
	
	if(error == 0){
		return SUCCESS;
	}else{
		return ERROR;
	}
}

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_UpdateMeasurements(void){
	int8_t error = 0;
	
	// Start Cell ADC Measurements
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	//conv_time = LTC6811_pollAdc();		 // In case you want to time the length of the conversion time
	
	// Read Cell Voltage Registers
	wakeup_sleep(NUM_VOLTAGE_BOARDS); // Not sure if wakeup is necessary if you start conversion then read consecutively
	error = LTC6811_rdcv(0, NUM_VOLTAGE_BOARDS, Modules); // Set to read back all cell voltage registers

	if(error == 0){
		return SUCCESS;
	}else{
		return ERROR;
	}
}

/** Voltage_IsSafe
 * Checks if all modules are safe
 * @return SAFE or FAIL
 */
SafetyStatus Voltage_IsSafe(void){
	for(int32_t i = 0; i < NUM_BATTERY_MODULES; i++){
		uint16_t voltage = Modules[i / 12].cells.c_codes[i % 12];
		if(voltage > MAX_VOLTAGE_LIMIT || voltage < MIN_VOLTAGE_LIMIT){
			return DANGER;
		}
	} 
	return SAFE;
}

/** Voltage_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Voltage_GetModulesInDanger(void){
	static uint16_t checks[NUM_BATTERY_MODULES];
	for(int i = 0; i < NUM_BATTERY_MODULES; ++i){
		if(Voltage_GetModuleVoltage(i) > MAX_VOLTAGE_LIMIT || Voltage_GetModuleVoltage(i) < MIN_VOLTAGE_LIMIT){
			checks[i] = 1;	// 1 shows that the unit is in danger
		}else{
			checks[i] = 0;	// 0 shows that the unit is not in danger
		}
	}

	int sum = 0;
	for(int i = 0; i < NUM_BATTERY_MODULES; ++i){
		sum += checks[i];
	}

	return checks;
}

/** Voltage_OpenWire
 * Uses the LTC6811_run_openwire_multi() function to check for open wires
 * @return ErrorStatus
 */
long Voltage_OpenWire(void){
	wakeup_idle(NUM_VOLTAGE_BOARDS);
	LTC6811_run_openwire_multi(NUM_VOLTAGE_BOARDS, Modules, true);
	for(uint8_t i = 0; i < NUM_VOLTAGE_BOARDS; i++){
		if(Modules[i].system_open_wire != 0){
			return ERROR;
		} else {
			return SUCCESS;
		}
	}
}

/** *Voltage_GetOpenWire
 * Finds the pin locations of the open wires
 * @return array of battery modules (1 means disconnect, 0 means safe)
 */
uint8_t *Voltage_GetOpenWire(void){
	wakeup_idle(NUM_VOLTAGE_BOARDS);
	LTC6811_run_openwire_multi(NUM_VOLTAGE_BOARDS, Modules, true);
	static uint8_t open_wires[NUM_BATTERY_MODULES];
	uint8_t count = 0;
	for(uint8_t i = 0; i < NUM_VOLTAGE_BOARDS; i++){
		long bin_openwire = Modules[i].system_open_wire;
		while(count < NUM_BATTERY_MODULES){
			if(bin_openwire & 1){
				open_wires[count] = 1;
			} else {
				open_wires[count] = 0;
			}
			bin_openwire>>=1;
			count++;
		}
	}
	return open_wires;
} //TODO: Test with BPS System

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain module in the battery pack
 * @param index of module
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleVoltage(uint16_t moduleIdx){
	int32_t boardIdx = moduleIdx / 12;
	return Modules[boardIdx].cells.c_codes[moduleIdx % 12];
}

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint16_t Voltage_GetTotalPackVoltage(void){
	int sum = 0;
	for(int32_t i = 0; i < NUM_BATTERY_MODULES; i++){
		sum += Modules[i / 12].cells.c_codes[i % 12];
	}

	return sum;
}




