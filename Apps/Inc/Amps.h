/* Copyright (c) 2022 UT Longhorn Racing Solar */
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
 */
void Amps_UpdateMeasurements(void);

/** Amps_CheckStatus
 * Checks if pack does not have a short circuit
 * @param maxTemperature the maximum temperature measured in the most recent batch of temperature readings
 * @return SAFE or DANGER
 */
SafetyStatus Amps_CheckStatus(int32_t maxTemperature);

/** Amps_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return true if charge, false if discharge
 */
bool Amps_IsCharging(void);

/** Amps_GetReading
 * Measures the electrical current from the Ampere minion board
 * @return milliamperes value
 */
int32_t Amps_GetReading(void);

/**
 * @brief calibrate the amperes module. Must be called when the contactors are open
 * 
 */
void Amps_Calibrate(void);

#endif
