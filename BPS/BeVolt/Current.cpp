// Current.cpp
/**
 * Current class that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo
 * @lastRevised 9/3/2018
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

// TODO: Think of better naming convention of current

#include <stdint.h>
#include "Current.h"

/** Constructor
 * Creates Current (Amps) instance
 */
Current::Current(){
	
}

/** Constructor
 * Creates Current (Amps) instance
 * @param max current the battery can handler before danger
 */
Current::Current(int ceiling){

}

/** Destructor
 * Destroys Current(Amps) instance
 */
Current::~Current(){

}

/** setLimits
 * Sets the max current limit the cells can reach before danger
 * @param max current limit
 */
void Current:setLimits(int ceiling){

}

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param Amps of high precision hall effect sensor
 * @param Amps of low precision hall efect sensor
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Current::updateMeasurements(int highPrecision, int lowPrecision){

}

/** isSafe
 * Checks if pack does not have a short circuit
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Current::isSafe(void){

}

/** highPrecisionCurrent
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return Amperes value
 */
int Current::highPrecisionAmperes(void){

}

/** lowPrecisionCurrent
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return Amperes value
 */
int Current::lowPrecisionAmperes(void){

}
