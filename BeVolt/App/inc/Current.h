/** Current.h
 * Current file that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire.

#ifndef CURRENT_H__
#define CURRENT_H__

#include <stdint.h>
 
/** Current_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Current_Init(void);

/** Current_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param Amps of high precision hall effect sensor
 * @param Amps of low precision hall efect sensor
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Current_UpdateMeasurements(void);

/** Current_IsSafe
 * Checks if pack does not have a short circuit
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Current_IsSafe(void);

/** Current_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
uint8_t Current_IsCharging(void);

/** Current_SetLimits
 * Sets the max current (Amps) limit the cells can reach before danger
 * @param max current limit
 */
void Current_SetLimits(uint16_t ceiling);

/** Current_GetHighPrecReading
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_GetHighPrecReading(void);

/** Current_GetLowPrecReading
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_GetLowPrecReading(void);

#endif
