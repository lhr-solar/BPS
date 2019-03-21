// Voltage.c
/**
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#include "Voltage.h"
#include "LTC6811.h"
#include "Settings.h"
#include <stdlib.h>

cell_asic Modules[NUM_VOLTAGE_BOARDS];

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 */
void Voltage_Init(void){
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
	LTC6811_Init(Modules);
	LTC6811_wrcfg(NUM_VOLTAGE_BOARDS, Modules);
}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Voltage_UpdateMeasurements(){
	int32_t error;
	wakeup_sleep(NUM_VOLTAGE_BOARDS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();
	error = LTC6811_rdcv(0, NUM_VOLTAGE_BOARDS, Modules); // Set to read back all cell voltage registers
}

/** Voltage_IsSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Voltage_IsSafe(void){
	for(int32_t i = 0; i < NUM_BATTERY_MODULES; i++){
		uint16_t voltage = Modules[i / 12].cells.c_codes[i % 12];
		if(voltage < MAX_VOLTAGE_LIMIT || voltage < MIN_VOLTAGE_LIMIT){
			return 0;
		}
	}

	return 1;
}

/** Voltage_SetLimits
 * Sets the max and min voltage limit the cells can reach before danger
 * @param max voltage limit
 * @param min voltage limit
 */
void Voltage_SetLimits(uint16_t ceiling, uint16_t floor){
	
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

	/*
	// TODO: Figure out if this is bad practice
	uint16_t endangeredModules[;
	int j = 0;
	for(int i = 0; i < sum; ++i){
		if(checks[i]){
			endangeredModules[j] = i;
			++j;
		}
	}

	return endangeredModules;
	*/
	return checks;
}

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

