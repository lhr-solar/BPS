// Voltage.cpp
/**
 * Voltage class that holds all voltage related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

#include <stdint.h>
#include "Voltage.h"

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
Voltage::Voltage(int ceiling, int floor){

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
void Voltage::setLimits(int ceiling, int floor){

}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Voltage::updateMeasurements(int *modules){

}

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Voltage::isSafe(void){

}

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
int *Voltage::modulesInDanger(void){

}

/** moduleVoltage
 * Gets the voltage of a certain module in the battery pack
 * @param index of module
 * @return voltage of module at specified index
 */
int Voltage::moduleVoltage(int moduleIdx){

}

/** totalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
int Voltage::totalPackVoltage(void){

}

