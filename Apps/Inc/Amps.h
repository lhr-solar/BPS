/* Copyright (c) 2020 UT Longhorn Racing Solar */
/** Amps.h
 * File that holds all electrical current related information of BeVolt's battery pack.
 */

#ifndef AMPS_H
#define AMPS_H

#include "common.h"
#include "config.h"
 
/** Amps_Init
 * Initializes two ADCs to begin current monitoring.
 */
void Amps_Init(void);

/** Amps_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Amps_UpdateMeasurements(void);

/** Amps_CheckStatus
 * Checks if pack does not have a short circuit
 * @param isCharging is 1 if the battery pack is charging and 0 if it is not charging
 * @return SAFE or DANGER
 */
SafetyStatus Amps_CheckStatus(bool isCharging);

/** Amps_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Amps_IsCharging(void);

/** Amps_GetReading
 * Measures the electrical current from the Ampere minion board
 * @return milliamperes value
 */
int32_t Amps_GetReading(void);

#endif
