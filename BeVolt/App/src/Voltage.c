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
	
	//Voltage Init
	Voltage_UpdateMeasurements();
	
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
	wakeup_idle(NUM_VOLTAGE_BOARDS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();	// In case you want to time the length of the conversion time
	
	// Read Cell Voltage Registers
	wakeup_idle(NUM_VOLTAGE_BOARDS); // Not sure if wakeup is necessary if you start conversion then read consecutively
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
		if(voltage > MAX_VOLTAGE_LIMIT * 10000){
			return OVERVOLTAGE;
		}
		else if(voltage < MIN_VOLTAGE_LIMIT * 10000){
			return UNDERVOLTAGE;
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




