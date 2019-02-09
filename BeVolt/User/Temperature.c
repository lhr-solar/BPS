// Temperature.c
/**
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

// NOTE: All units are in Celsius

#include "Temperature.h"

uint16_t *ModuleTemperatures;				// list of voltages of all modules
uint16_t MaxTemperatureLimit;				// Max temperature the battery can reach before danger

/** Temperature_Init
 * Initializes device drivers including SPI and LTC2983 for Temperature Monitoring
 */
void Temperature_Init(void){
	
}

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return 1 if successfully stored, 0 if failed
 */
bool Temperature_UpdateMeasurements(){
	//ModuleTemperatures = LTC2983_Measure();

	if(sizeof(ModuleTemperatures)/sizeof(uint16_t) == NUM_BATTERY_MODULES){
		return 1;
	}else{
		return 0;
	}
}

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return 1 if pack is safe, 0 if in danger
 */
bool Temperature_IsSafe(uint8_t isCharging){
	
	/* TODO: Change to accomodate for charge and discharge limits
	for(int i = 0; i < sizeof(ModuleTemperatures)/sizeof(uint16_t); ++i){
		if(Temperature_GetModuleTemperature(i) > MaxTemperatureLimit){
			return 0;
		}
	}
	*/
	return 1;
}

/** Temperature_SetLimits
 * Sets the max temperature limit the cells can reach before danger
 * @param max temperature limit
 */
void Temperature_SetLimits(uint16_t ceiling){
	MaxTemperatureLimit = ceiling;
}


/** Temperature_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Temperature_GetModulesInDanger(void){
	uint8_t checks[NUM_BATTERY_MODULES];
	for(int i = 0; i < NUM_BATTERY_MODULES; ++i){
		if(Temperature_GetModuleTemperature(i) > MaxTemperatureLimit){
			checks[i] = 1;	// 1 shows that the unit is in danger
		}else{
			checks[i] = 0;	// 0 shows that the unit is not in danger
		}
	}

	int sum = 0;
	for(int i = 0; i < NUM_BATTERY_MODULES; ++i){
		sum += checks[i];
	}
	
	// TODO: Figure out if this is bad practice
	uint16_t * endangeredModules = (uint16_t *) malloc(sum);
	int j = 0;
	for(int i = 0; i < sum; ++i){
		if(checks[i]){
			endangeredModules[j] = i;
			++j;
		}
	}

	return endangeredModules;
}

/** Temperature_GetModuleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
uint16_t Temperature_GetModuleTemperature(uint16_t moduleIdx){
	return ModuleTemperatures[moduleIdx];
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
uint16_t Temperature_GetTotalPackAvgTemperature(void){
	int sum = 0;
	for(int i = 0; i < sizeof(ModuleTemperatures)/sizeof(uint16_t); ++i){
		sum += Temperature_GetModuleTemperature(i);
	}
	return (sum / (sizeof(ModuleTemperatures)/sizeof(uint16_t)));
}
