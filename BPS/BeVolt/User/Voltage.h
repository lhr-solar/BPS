// Voltage.h
/**
 * Voltage class that holds all voltage related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__

#include <stdint.h>

enum VoltageStatus {OverVoltage, UnderVoltage};

/** Voltage
 * This class holds functions related to the voltage of BeVolt's
 * battery pack.
 */
class Voltage{
public:

	/** Constructor
	 * Creates Voltage instance
	 */
	Voltage();

	/** Constructor
	 * Creates Voltage instance with NULL pointer to voltage list
	 * @param max voltage limit of the lithium ion cells
	 * @param min voltage limit of the lithium ion cells
	 */
	Voltage(uint16_t ceiling, uint16_t floor);

	/** Destructor
	 * Destroys Voltage instance
	 */
	~Voltage();

	/** setLimits
	 * Sets the max and min voltage limit the cells can reach before danger
	 * @param max voltage limit
	 * @param min voltage limit
	 */
	void setLimits(uint16_t ceiling, uint16_t floor);

	/** updateMeasurements
	 * Stores and updates the new measurements received
	 * @param pointer to new voltage measurements
	 * @return 1 if successfully stored, 0 if failed
	 */
	uint8_t updateMeasurements();

	/** isSafe
	 * Checks if all modules are safe
	 * @return 1 if pack is safe, 0 if in danger
	 */
	uint8_t isSafe(void);

	/** modulesInDanger
	 * Finds all modules that in danger and stores them into a list
	 * @return pointer to index of modules that are in danger
	 */
	uint16_t *modulesInDanger(void);

	/** moduleVoltage
	 * Gets the voltage of a certain module in the battery pack
	 * @param index of module
	 * @return voltage of module at specified index
	 */
	uint16_t moduleVoltage(int moduleIdx);

	/** totalPackVoltage
	 * Gets the total voltage of the battery pack
	 * @return voltage of whole battery pack
	 */
	uint16_t totalPackVoltage(void);

private:
	uint16_t *modules;			// list of voltages of all modules
	uint16_t maxVoltageLimit;	// maximum voltage limit the battery can reach before danger
	uint16_t minVoltageLimit;	// minimum voltage limit the battery can reach before danger
};

#endif
