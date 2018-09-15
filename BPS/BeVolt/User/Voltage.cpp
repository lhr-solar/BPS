// Voltage.cpp
/**
 * Voltage class that holds all voltage related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

#include <stdint.h>
#include "Voltage.h"
#include "LTC6813.h"

/** Constructor
 * Creates Voltage instance with NULL pointer to voltage list
 */
Voltage::Voltage(){

}

/** Constructor
 * Creates Voltage instance with NULL pointer to voltage list
 * @param max voltage limit of the lithium ion cells
 * @param min voltage limit of the lithium ion cells
 */
Voltage::Voltage(uint16_t ceiling, uint16_t floor){
	setLimits(ceiling, floor);
}

/** Destructor
 * Destroys Voltage instance
 */
Voltage::~Voltage(){

}

/** setLimits
 * Sets the max and min voltage limit the cells can reach before danger
 * @param max voltage limit
 * @param min voltage limit
 */
void Voltage::setLimits(uint16_t ceiling, uint16_t floor){
	maxVoltageLimit = ceiling;
	minVoltageLimit = floor;
}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Voltage::updateMeasurements(){
	modules = LTC6813_Measure();
	
	if(sizeof(modules)/sizeof(uint16_t) == NUM_MODULES){
		return 1;
	}else{
		return 0;
	}
}

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Voltage::isSafe(void){
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		if(moduleVoltage(i) > maxVoltageLimit || moduleVoltage(i) < minVoltageLimit){
			return 0;
		}
	}

	return 1;
}

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Voltage::modulesInDanger(void){
	int checks[NUM_MODULES];
	for(int i = 0; i < NUM_MODULES; ++i){
		if(moduleVoltage(i) > maxVoltageLimit || moduleVoltage(i) < minVoltageLimit){
			checks[i] = 1;	// 1 shows that the unit is in danger
		}else{
			checks[i] = 0;	// 0 shows that the unit is not in danger
		}
	}

	int sum = 0;
	for(int i = 0; i < NUM_MODULES; ++i){
		sum += checks[i];
	}

	int endangeredModules[sum];
	int j = 0;
	for(int i = 0; i < NUM_MODULES; ++i){
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
int Voltage::moduleVoltage(int moduleIdx){
	return modules[moduleIdx];
}

/** totalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
int Voltage::totalPackVoltage(void){
	int sum = 0;
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		sum += modules[i];
	}

	return sum;
}

