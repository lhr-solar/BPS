// Current.c
/**
 * Current wrapper that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

// TODO: Think of better naming convention of current

#include "Current.h"

uint16_t HighPrecisionCurrent;		// Amp measurement of hall effect sensor of high precision
uint16_t LowPrecisionCurrent;		// Amp measurement of hall effect sensor of high precision
uint16_t MaxCurrentLimit;				// Max current (amps) limit the battery can handle before danger

/** Current_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Current_Init(void){
	
}

/** Current_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param Amps of high precision hall effect sensor
 * @param Amps of low precision hall efect sensor
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Current_UpdateMeasurements(void){

}

/** Current_IsSafe
 * Checks if pack does not have a short circuit
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Current_IsSafe(void){
	return HighPrecisionCurrent < MaxCurrentLimit && LowPrecisionCurrent < MaxCurrentLimit;
}

/** Current_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
uint8_t Current_IsCharging(void){
	
}

/** Current_SetLimits
 * Sets the max current (Amps) limit the cells can reach before danger
 * @param max current limit
 */
void Current_SetLimits(uint16_t ceiling){
	MaxCurrentLimit = ceiling;
}

/** Current_GetHighPrecReading
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_GetHighPrecReading(void){
	return HighPrecisionCurrent;
}

/** Current_GetLowPrecReading
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_GetLowPrecReading(void){
	return LowPrecisionCurrent;
}
