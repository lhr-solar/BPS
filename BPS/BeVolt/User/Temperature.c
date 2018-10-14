// Temperature.c
/**
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

// NOTE: All units are in Celsius

#include "Temperature.h"
#include "LTC2983.h"
#include "Definition.h"
#include <stdlib.h>

uint16_t *ModuleTemperatures;				// list of voltages of all modules
uint16_t MaxTemperatureLimit;				// Max temperature the battery can reach before danger

/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 * @param max temperature limit of the lithium ion cells
 */
void Temperature_Init(uint16_t ceiling){
	Temperature_SetLimits(ceiling);
}

/** setLimits
 * Sets the max temperature limit the cells can reach before danger
 * @param max temperature limit
 */
void Temperature_SetLimits(uint16_t ceiling){
	MaxTemperatureLimit = ceiling;
}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Temperature_UpdateMeasurements(){
	ModuleTemperatures = LTC2983_Measure();

	if(sizeof(ModuleTemperatures)/sizeof(uint16_t) == NUM_BATTERY_MODULES){
		return 1;
	}else{
		return 0;
	}
}

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Temperature_IsSafe(void){
	for(int i = 0; i < sizeof(ModuleTemperatures)/sizeof(uint16_t); ++i){
		if(Temperature_ModuleTemperature(i) > MaxTemperatureLimit){
			return 0;
		}
	}

	return 1;
}

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Temperature_ModulesInDanger(void){
	uint8_t checks[NUM_BATTERY_MODULES];
	for(int i = 0; i < NUM_BATTERY_MODULES; ++i){
		if(Temperature_ModuleTemperature(i) > MaxTemperatureLimit){
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

/** moduleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
uint16_t Temperature_ModuleTemperature(uint16_t moduleIdx){
	return ModuleTemperatures[moduleIdx];
}

/** totalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
uint16_t Temperature_TotalPackAvgTemperature(void){
	int sum = 0;
	for(int i = 0; i < sizeof(ModuleTemperatures)/sizeof(uint16_t); ++i){
		sum += Temperature_ModuleTemperature(i);
	}
	return (sum / (sizeof(ModuleTemperatures)/sizeof(uint16_t)));
}
