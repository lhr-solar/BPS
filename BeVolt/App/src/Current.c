/** Current.c
 * Current wrapper that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

// TODO: Think of better naming convention of current

#include "Current.h"

uint16_t HighPrecisionCurrent;		// Amp measurement of hall effect sensor of high precision
uint16_t LowPrecisionCurrent;		// Amp measurement of hall effect sensor of high precision

/** Current_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Current_Init(void){
	
}

/** Current_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Current_UpdateMeasurements(void){
	return ERROR;
}

/** Current_IsSafe
 * Checks if pack does not have a short circuit
 * @return SAFE or DANGER
 */
SafetyStatus Current_IsSafe(void){
	
	return DANGER;
}

/** Current_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Current_IsCharging(void){
	return 0;
}

/** Current_GetHighPrecReading
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_GetHighPrecReading(void){
	return 0;
}

/** Current_GetLowPrecReading
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_GetLowPrecReading(void){
	return 0;
}
