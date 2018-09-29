// Voltage.h
/**
 * Voltage file that holds all voltage related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__

#include <stdint.h>

enum VoltageStatus {OverVoltage, UnderVoltage};

	/** Constructor
	 * Creates Voltage instance
	 */
void Voltage_Init(uint16_t ceiling, uint16_t floor);

/** setLimits
 * Sets the max and min voltage limit the cells can reach before danger
 * @param max voltage limit
 * @param min voltage limit
 */
void Voltage_SetLimits(uint16_t ceiling, uint16_t floor);

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Voltage_UpdateMeasurements();

/** isSafe
 * Checks if all modules are safe
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Voltage_IsSafe(void);

/** modulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Voltage_ModulesInDanger(void);

/** moduleVoltage
 * Gets the voltage of a certain module in the battery pack
 * @param index of module
 * @return voltage of module at specified index
 */
uint16_t Voltage_ModuleVoltage(uint16_t moduleIdx);

/** totalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint16_t Voltage_TotalPackVoltage(void);

#endif
