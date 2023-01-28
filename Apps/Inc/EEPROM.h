/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** EEPROM.h
 * Interface for the EEPROM. Used for keeping track of state of charge and errors across resets.
 */

#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

/** EEPROM_Init
 * Initializes I2C to communicate with EEPROM (M24128)
 */
void EEPROM_Init(void);

/** EEPROM_Reset
 * Resets the EEPROM. This will clear all data that has been logged so far. This is required for 
 * clearing data and setting up new EEPROMs. This CMD really should be reserved for specialized
 * set-up programs and the CLI.
 */
void EEPROM_Reset(void);

/**
 * @brief Gets the stored state of charge from the EEPROM
 * 
 * @return uint32_t stored charge value
 */
uint32_t EEPROM_GetCharge(void);

/**
 * @brief Set the stored state of charge value in the EEPROM
 * 
 * @param charge The value to set the EEPROM's stored state of charge to
 */
void EEPROM_SetCharge(uint32_t charge);

/**
 * @brief Log an error to the EEPROM
 * 
 * @param error The error to log
 */
void EEPROM_LogError(uint32_t error);

/**
 * @brief Get all of the errors store in the EEPROM
 * 
 * @param errors        a buffer to store all of the errors to
 * @param maxErrors     the maximum number of errors that can fit in the buffer
 * @return              the number of errors read
 */
uint16_t EEPROM_GetErrors(uint32_t *errors, uint16_t maxErrors);

#endif
