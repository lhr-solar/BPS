// Current.h
/**
 * Current class that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

#ifndef __CURRENT_H__
#define __CURRENT_H__

/** Current (Amps)
 * This class holds functions related to the temperature of BeVolt's
 * battery pack.
 */
class Current{
public:

	/** Constructor
	 * Creates Current (Amps) instance
	 */
	Current();

	/** Constructor
	 * Creates Current (Amps) instance
	 * @param max current the battery can handler before danger
	 */
	Current(int ceiling);

	/** Destructor
	 * Destroys Current (Amps) instance
	 */
	~Current();

	/** setLimits
	 * Sets the max current (Amps) limit the cells can reach before danger
	 * @param max current limit
	 */
	void setLimits(int ceiling);

	/** updateMeasurements
	 * Stores and updates the new measurements received
	 * @param Amps of high precision hall effect sensor
	 * @param Amps of low precision hall efect sensor
	 * @return 1 if successfully stored, 0 if failed
	 */
	uint8_t updateMeasurements(int highPrecision, int lowPrecision);

	/** isSafe
	 * Checks if pack does not have a short circuit
	 * @return 1 if pack is safe, 0 if in danger
	 */
	uint8_t isSafe(void);

	/** highPrecisionCurrent
	 * Gets the Ampere measurement the high precision hall effect sensor recorded
	 * @return Amperes value
	 */
	int highPrecisionAmperes(void);

	/** lowPrecisionCurrent
	 * Gets the Ampere measurement the low precision hall effect sensor recorded
	 * @return Amperes value
	 */
	int lowPrecisionAmperes(void);

private:
	int highPrecision;		// Amp measurement of hall effect sensor of high precision
	int lowPrecision;		// Amp measurement of hall effect sensor of high precision
	int maxCurrentLimit;	// Max current (amps) limit the battery can handle before danger
};

#endif
