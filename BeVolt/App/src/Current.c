/** Current.c
 * Current wrapper that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

// TODO: Think of better naming convention of current

#include "Current.h"
#include "ADC.h"
#include "config.h"

uint16_t HighPrecisionCurrent;		// Amp measurement of hall effect sensor of high precision
uint16_t LowPrecisionCurrent;		// Amp measurement of hall effect sensor of high precision

/** Current_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Current_Init(void){
	ADC_InitHilo();	// Initialize the ADCs
}

/** Current_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Current_UpdateMeasurements(void){
	HighPrecisionCurrent = ADC_Conversion(ADC_ReadHigh(), HIGH_PRECISION);
	LowPrecisionCurrent = ADC_Conversion(ADC_ReadLow(), LOW_PRECISION);
	
	return ERROR;	// TODO: Once this has been tested, stop returning errors
}

/** Current_IsSafe
 * Checks if pack does not have a short circuit
 * @return SAFE or DANGER
 */
SafetyStatus Current_IsSafe(void){
	
	if(HighPrecisionCurrent > MAX_HIGH_PRECISION_CURRENT) {
		return (LowPrecisionCurrent < MAX_CURRENT_LIMIT)
			? SAFE 
			: DANGER;
	} else {
		return SAFE; 
	}
	
	// This line should never be reached
	return DANGER;
}

/** Current_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Current_IsCharging(void){
	// TODO: Make sure that the current board is installed in such a way that negative => charging
	return HighPrecisionCurrent < 1;
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
