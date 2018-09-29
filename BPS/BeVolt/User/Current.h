// Current.h
/**
 * Current file that holds all Current (Amps) related information of BeVolt's
 * battery pack.
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

// NOTE: Current does not mean current as in time. It is the flow of charge through a wire

#ifndef __CURRENT_H__
#define __CURRENT_H__

#include <stdint.h>
 
void Current_Init(uint16_t ceiling);
 
/** setLimits
 * Sets the max current (Amps) limit the cells can reach before danger
 * @param max current limit
 */
void Current_SetLimits(uint16_t ceiling);

/** updateMeasurements
 * Stores and updates the new measurements received
 * @param Amps of high precision hall effect sensor
 * @param Amps of low precision hall efect sensor
 * @return 1 if successfully stored, 0 if failed
 */
uint8_t Current_UpdateMeasurements();

/** isSafe
 * Checks if pack does not have a short circuit
 * @return 1 if pack is safe, 0 if in danger
 */
uint8_t Current_IsSafe(void);

/** highPrecisionCurrent
 * Gets the Ampere measurement the high precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_HighPrecisionAmperes(void);

/** lowPrecisionCurrent
 * Gets the Ampere measurement the low precision hall effect sensor recorded
 * @return Amperes value
 */
uint16_t Current_LowPrecisionAmperes(void);

#endif
