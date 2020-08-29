/** Amps.c
 * Wrapper that holds all electrical current related information of BeVolt's
 * battery pack.
 */

#include "Amps.h"
#include "AS8510.h"

static int16_t latestMeasureMilliAmps;

/** Amps_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Amps_Init(void) {
}

/** Amps_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Amps_UpdateMeasurements(void) {
	latestMeasureMilliAmps = AS8510_GetCurrent();	// TODO: verify that there is no conversion required here

	return SUCCESS;
}

/** Amps_CheckStatus
 * Checks if pack does not have a short circuit
 * @param chargingOnly a flag that indicates whether we should allow
 * 		  only charging or both charging and discharging.
 * @return SAFE or DANGER
 */
SafetyStatus Amps_CheckStatus(bool chargingOnly) {
	if(chargingOnly) {
		// Only allow charging status
		if((latestMeasureMilliAmps <= 0) 
		 &&(latestMeasureMilliAmps > MAX_CHARGING_CURRENT)) {
			return SAFE;
		}
	} else {
		// Constrain between the max charging allowed (negative) and the max draw (postive)
		if((latestMeasureMilliAmps > MAX_CHARGING_CURRENT) 
		 &&(latestMeasureMilliAmps < MAX_CURRENT_LIMIT)) {
			 return SAFE;
		}
	}

	return DANGER;
}

/** Amps_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Amps_IsCharging(void) {
	return latestMeasureMilliAmps < 0;
}

/** Amps_GetReading
 * Measures the electrical current from the Ampere minion board
 * @return milliamperes value
 */
int32_t Amps_GetReading(void) {
	return latestMeasureMilliAmps;
}
