// Voltage.c
/**
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#include "Voltage.h"
#include "LTC6811.h"
#include "Definition.h"
#include <stdlib.h>

uint16_t static Modules[NUM_BATTERY_MODULES];		// Holds voltage values of modules
uint16_t static MaxVoltageLimit;	// maximum voltage limit the battery can reach before danger
uint16_t static MinVoltageLimit;	// minimum voltage limit the battery can reach before danger

/** Constructor
 * Creates Voltage instance with NULL pointer to voltage list
 * @param max voltage limit of the lithium ion cells
 * @param min voltage limit of the lithium ion cells
 */
void Voltage_Init(uint16_t ceiling, uint16_t floor){
	Voltage_SetLimits(ceiling, floor);
}

/** setLimits
 * Sets the max and min voltage limit the cells can reach before danger
 * @param max voltage limit
 * @param min voltage limit
 */
void Voltage_SetLimits(uint16_t ceiling, uint16_t floor){
	MaxVoltageLimit = ceiling;
	MinVoltageLimit = floor;
}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Voltage_UpdateMeasurements(){
//	Modules = LTC6811_Measure();
	
	if(sizeof(Modules)/sizeof(uint16_t) == NUM_BATTERY_MODULES){
		return 1;
	}else{
		return 0;
	}
}

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Voltage_IsSafe(void){
	for(int i = 0; i < sizeof(Modules)/sizeof(uint16_t); ++i){
		if(Voltage_ModuleVoltage(i) > MaxVoltageLimit || Voltage_ModuleVoltage(i) < MinVoltageLimit){
			return 0;
		}
	}

	return 1;
}

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Voltage_ModulesInDanger(void){
	int checks[NUM_BATTERY_MODULES];
	for(int i = 0; i < NUM_BATTERY_MODULES; ++i){
		if(Voltage_ModuleVoltage(i) > MaxVoltageLimit || Voltage_ModuleVoltage(i) < MinVoltageLimit){
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

/** moduleVoltage
 * Gets the voltage of a certain module in the battery pack
 * @param index of module
 * @return voltage of module at specified index
 */
uint16_t Voltage_ModuleVoltage(uint16_t moduleIdx){
	return Modules[moduleIdx];
}

/** totalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint16_t Voltage_TotalPackVoltage(void){
	int sum = 0;
	for(int i = 0; i < sizeof(Modules)/sizeof(uint16_t); ++i){
		sum += Modules[i];
	}

	return sum;
}

