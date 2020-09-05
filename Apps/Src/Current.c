/** Current.c
 * Current wrapper that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

// TODO: Think of better naming convention of current

#include "Current.h"
#include "BSP_ADC.h"

int32_t HighPrecisionCurrent;	// Milliamp measurement of hall effect sensor of high precision
int32_t LowPrecisionCurrent;	// Milliamp measurement of hall effect sensor of low precision

typedef enum {
	HIGH_PRECISION,
	LOW_PRECISION
} CurrentSensor;

static int32_t Current_Conversion(uint32_t milliVolts, CurrentSensor s);

/** Current_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Current_Init(void){
	BSP_ADC_Init();	// Initialize the ADCs
}

/** Current_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Current_UpdateMeasurements(void){
	HighPrecisionCurrent = Current_Conversion(BSP_ADC_High_GetMilliVoltage(), HIGH_PRECISION);
	LowPrecisionCurrent  = Current_Conversion(BSP_ADC_Low_GetMilliVoltage(), LOW_PRECISION);

	return SUCCESS;	// TODO: Once this has been tested, stop returning errors
}

/** Current_CheckStatus
 * Checks if pack does not have a short circuit
 * @return SAFE or DANGER
 */
SafetyStatus Current_CheckStatus(bool override) {

	if((LowPrecisionCurrent > MAX_CHARGING_CURRENT)&&(LowPrecisionCurrent < MAX_CURRENT_LIMIT)&&(!override))
		return SAFE;
	else if((LowPrecisionCurrent <= 0)&&(LowPrecisionCurrent > MAX_CHARGING_CURRENT)&&override)
		return SAFE;
	else
		return DANGER;
}

/** Current_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Current_IsCharging(void) {
	// TODO: Make sure that the current board is installed in such a way that negative => charging
	return LowPrecisionCurrent < 0;
}

/** Current_GetHighPrecReading
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return milliamperes value
 */
int32_t Current_GetHighPrecReading(void) {
	return HighPrecisionCurrent;
}

/** Current_GetLowPrecReading
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return milliamperes value
 */
int32_t Current_GetLowPrecReading(void) {
	return LowPrecisionCurrent;
}

/** Current_Conversion
 * Returns the converted value of the current read by the sensor
 * @returns current in mA
 */
static int32_t Current_Conversion (uint32_t milliVolts, CurrentSensor s) {
	// These are the ideal values that we should expect
	const int opAmpOffset = 4096;  // The offset in millivolts for the op-amp stage
	const int opAmpGain = 3;       // Gain applied after the offset

	// The output of the hall sensor (prior to the op-amp stage)
	int32_t sensorOutput = milliVolts * opAmpGain - opAmpOffset;

	// The actual current measured by the sensors
	switch(s) {
		case HIGH_PRECISION: // Rated for currents between -50 and 50 A.
			return sensorOutput * (50 / 4);	// In mA.
		case LOW_PRECISION:  // Rated for currents between -100 and 100 A.
		default:
			return sensorOutput * (100 / 4);	// In mA.
	}
}
