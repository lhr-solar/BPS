// Temperature.h
/**
 * Temperature class that holds all Temperature related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

// NOTE: All units are in Celsius

#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#define NUM_MODULES 31

/** Temperature
 * This class holds functions related to the temperature of BeVolt's
 * battery pack.
 */
class Temperature{
public:

	/** Constructor
	 * Creates Temperature instance with NULL pointer to temperature list
	 */
	Temperature();

	/** Constructor
	 * Creates Temperature instance with NULL pointer to temperature list
	 * @param max temperature limit of the lithium ion cells
	 */
	Temperature(int ceiling);

	/** Destructor
	 * Destroys Temperature instance
	 */
	~Temperature();

	/** setLimits
	 * Sets the max temperature limit the cells can reach before danger
	 * @param max temperature limit
	 */
	void setLimits(int ceiling);

	/** updateMeasurements
	 * Stores and updates the new measurements received
	 * @param pointer to new temperature measurements
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
	int *modulesInDanger(void);

	/** moduleTemperature
	 * Gets the temperature of a certain module in the battery pack
	 * @param index of module
	 * @return temperature of module at specified index
	 */
	int moduleTemperature(int moduleIdx);

	/** totalPackAvgTemperature
	 * Gets the average temperature of the whole battery pack
	 * @return average temperature of battery pack
	 */
	int totalPackAvgTemperature(void);

private:
	int *modules;				// list of voltages of all modules
	int maxTemperatureLimit;	// Max temperature the battery can reach before danger

};

#endif
