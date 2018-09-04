// Temperature.cpp
/**
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

// NOTE: All units are in Celsius

#include <stdint.h>
#include "Temperature.h"

/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 */
Temperature::Temperature(){
	
}


/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 * @param max temperature limit of the lithium ion cells
 */
Temperature::Temperature(int ceiling){

}

/** Constructor
 * Creates Temperature instance with NULL pointer to temperature list
 * @param max temperature limit of the lithium ion cells
 */
Temperature::Temperature(int ceiling, int *modules){

}

/** Destructor
 * Destroys Temperature instance
 */
Temperature::~Temperature(){

}

/** setLimits
 * Sets the max temperature limit the cells can reach before danger
 * @param max temperature limit
 */
void Temperature::setLimits(int ceiling){

}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Temperature::updateMeasurements(int *modules){

}

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Temperature::isSafe(void){

}

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
int *Temperature::modulesInDanger(void){

}

/** moduleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
int Voltage::moduleTemperature(int moduleIdx){

}

/** totalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int Voltage::totalPackAvgTemperature(void){

}
