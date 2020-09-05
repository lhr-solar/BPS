/** Current.h
 * Current file that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire.

#ifndef CURRENT_H__
#define CURRENT_H__

#include "common.h"
#include "config.h"
 
/** Current_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Current_Init(void);

/** Current_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Current_UpdateMeasurements(void);

/** Current_CheckStatus
 * Checks if pack does not have a short circuit
 * @return SAFE or DANGER
 */
SafetyStatus Current_CheckStatus(bool override);

/** Current_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Current_IsCharging(void);

/** Current_GetHighPrecReading
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return milliamperes value
 */
int32_t Current_GetHighPrecReading(void);

/** Current_GetLowPrecReading
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return milliamperes value
 */
int32_t Current_GetLowPrecReading(void);

#endif
