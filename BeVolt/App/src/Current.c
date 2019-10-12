/** Current.c
 * Current wrapper that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

// TODO: Think of better naming convention of current

#include "Current.h"
#include "ADC.h"
#include "config.h"

uint32_t HighPrecisionCurrent;	// Amp measurement of hall effect sensor of high precision
uint32_t LowPrecisionCurrent;	// Amp measurement of hall effect sensor of high precision

typedef enum {
	HIGH_PRECISION,
	LOW_PRECISION
} CurrentSensor;

static uint32_t Current_Conversion(uint32_t ADC_Reading, CurrentSensor s);

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
	HighPrecisionCurrent = Current_Conversion(ADC_ReadHigh(), HIGH_PRECISION);
	LowPrecisionCurrent  = Current_Conversion(ADC_ReadLow(), LOW_PRECISION);

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

/** ADC_Conversion
 * Returns the converted value of the current read by the sensor
 * @returns converted voltage - fixed point notation of 0.001
 */
static uint32_t Current_Conversion (uint32_t ADC_Reading, CurrentSensor s){
	const int maxReading = 0x0FFF; // 12-bit value
	const int operationMilliVoltage = 3300; // 3.3V
	const int opAmpOffset = 4096;  // The offset in millivolts for the op-amp stage
	const int opAmpGain = 3;       // Gain applied after the offset

	// The actual reading on the ADC pin
	uint32_t milliVolts = ADC_Reading * operationMilliVoltage / maxReading;

	// The output of the hall sensor (prior to the op-amp stage)
	uint32_t sensorOutput = milliVolts * opAmpGain - opAmpOffset;

	// The actual current measured by the sensors
	switch(s) {
		case HIGH_PRECISION: // Rated for currents between -50 and 50 A.
			return sensorOutput * 50 / 4;	// In mA.
		case LOW_PRECISION:  // Rated for currents between -100 and 100 A.
		default:
			return sensorOutput * 100 / 4;	// In mA.
	}
}
