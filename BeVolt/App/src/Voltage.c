/** Voltage.c
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 */

#include "Voltage.h"
#include "LTC6811.h"
#include "config.h"
#include <stdlib.h>

static cell_asic *Minions;

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_Init(cell_asic *boards){
	// Record pointer
	Minions = boards;

	int8_t error = 0;
	
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(Minions);
	
	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, Minions);

	// Read Configuration Register
	wakeup_sleep(NUM_MINIONS);
	error = LTC6811_rdcfg(NUM_MINIONS, Minions);
	
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
	wakeup_idle(NUM_MINIONS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();	// In case you want to time the length of the conversion time
	
	// Read Cell Voltage Registers
	wakeup_idle(NUM_MINIONS); // Not sure if wakeup is necessary if you start conversion then read consecutively
	error = LTC6811_rdcv(0, NUM_MINIONS, Minions); // Set to read back all cell voltage registers

	if(error == 0){
		return SUCCESS;
	}else{
		return ERROR;
	}
}

/** Voltage_IsSafe
 * Checks if all battery modules are safe
 * @return SAFE or danger: UNDERVOLTAGE or OVERVOLTAGE
 */
SafetyStatus Voltage_IsSafe(void){
	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		uint16_t voltage = Voltage_GetModuleVoltage(i);
			
		// VOLTAGE_LIMITS are in floating point. The LTC6811 sends the voltage data
		// as unsigned 16-bit fixed point integers with a resolution of 0.00001
		if(voltage > MAX_VOLTAGE_LIMIT * LTC6811_SCALING_FACTOR){
			return OVERVOLTAGE;
		}
		
		else if(voltage < MIN_VOLTAGE_LIMIT * LTC6811_SCALING_FACTOR){
			return UNDERVOLTAGE;
		}
	}
	
	return SAFE;
}

/** Voltage_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list.
 * Each module corresponds to and index of the array. If the element in the
 * array is 1, then it means that module in the index is in danger.
 * @return pointer to index of modules that are in danger
 */
uint8_t *Voltage_GetModulesInDanger(void){
	static uint8_t checks[NUM_BATTERY_MODULES];
	
	for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
		
		// Check if battery is in range of voltage limit
		if (Voltage_GetModuleVoltage(i) > MAX_VOLTAGE_LIMIT || Voltage_GetModuleVoltage(i) < MIN_VOLTAGE_LIMIT) {
			checks[i] = 1;	// 1 shows that the unit is in danger
			
		} else {
			checks[i] = 0;	// 0 shows that the unit is not in danger
		}
	}
	
	return checks;
}

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery (0-indexed)
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleVoltage(uint8_t moduleIdx){
	return Minions[moduleIdx / MAX_VOLT_SENSORS_PER_MINION_BOARD].cells.c_codes[moduleIdx % MAX_VOLT_SENSORS_PER_MINION_BOARD];
}

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint32_t Voltage_GetTotalPackVoltage(void){
	int sum = 0;
	for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
		sum += Voltage_GetModuleVoltage(i);
	}
	return sum;
}




