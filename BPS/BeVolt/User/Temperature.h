// Temperature.h
/**
 * Temperature file that holds all Temperature related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

// NOTE: All units are in Celsius

#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#include <stdint.h>

void Temperature_Init(uint16_t ceiling);

/** setLimits
 * Sets the max temperature limit the cells can reach before danger
 * @param max temperature limit
 */
void Temperature_SetLimits(uint16_t ceiling);

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Temperature_UpdateMeasurements();

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Temperature_IsSafe(void);

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Temperature_ModulesInDanger(void);

/** moduleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
uint16_t Temperature_ModuleTemperature(uint16_t moduleIdx);

/** totalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
uint16_t Temperature_TotalPackAvgTemperature(void);

#endif
