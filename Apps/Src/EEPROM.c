/* Copyright (c) 2022 UT Longhorn Racing Solar */

/** EEPROM.c
 * Interface for the EEPROM. Used for keeping track of state of charge and errors across resets.
 */

#include "EEPROM.h"
#include "M24128.h"

// addresses for EEPROM data segments
static const uint16_t EEPROM_CHARGE_ADDR = 0x0;
static const uint16_t EEPROM_FAULT_ADDR  = (EEPROM_CHARGE_ADDR + sizeof(uint32_t));

// terminating character for fault array
static const uint32_t EEPROM_TERMINATOR = 0xFFFFFFFF;

// cached value of end of fault array
static uint16_t faultArrayEndAddress;

/** EEPROM_Init
 * Initializes EEPROM application
 */
void EEPROM_Init(void) {
    // initialize the EEPROM
    M24128_Init();

    // find the end of the fault array
    uint16_t data = 0;
    faultArrayEndAddress = EEPROM_FAULT_ADDR - sizeof(EEPROM_TERMINATOR);
    while (data != EEPROM_TERMINATOR) {
        faultArrayEndAddress += sizeof(EEPROM_TERMINATOR);
        M24128_Read(faultArrayEndAddress, sizeof(data), (uint8_t *) &data);
    }
}

/** EEPROM_Reset
 * Resets the EEPROM. This will clear all data that has been logged so far. This is required for 
 * clearing data and setting up new EEPROMs. This CMD really should be reserved for specialized
 * set-up programs and the CLI.
 */
void EEPROM_Reset(void) {
    // initialize the EEPROM
    M24128_Init();

    // initialize the fault array
    M24128_Write(EEPROM_FAULT_ADDR, sizeof(EEPROM_TERMINATOR), (uint8_t *) &EEPROM_TERMINATOR);
}

/**
 * @brief Gets the stored state of charge from the EEPROM
 * 
 * @return uint32_t stored charge value
 */
uint32_t EEPROM_GetCharge(void) {
    uint32_t charge;
    M24128_Read(EEPROM_CHARGE_ADDR, sizeof(charge), (uint8_t *) &charge);
    return charge;
}

/**
 * @brief Set the stored state of charge value in the EEPROM
 * 
 * @param charge The value to set the EEPROM's stored state of charge to
 */
void EEPROM_SetCharge(uint32_t charge) {
    M24128_Write(EEPROM_CHARGE_ADDR, sizeof(charge), (uint8_t *) &charge);
}

/**
 * @brief Log an error to the EEPROM
 * 
 * @param error The error to log
 */
void EEPROM_LogError(Fault_Set error) {
    // assumes sizeof(EEPROM_TERMINATOR) == sizeof(Fault_Set)
    M24128_Write(faultArrayEndAddress + sizeof(EEPROM_TERMINATOR), sizeof(EEPROM_TERMINATOR), (uint8_t *) &EEPROM_TERMINATOR);
    M24128_Write(faultArrayEndAddress, sizeof(Fault_Set), (uint8_t *) &error);
}

/**
 * @brief Get all of the errors store in the EEPROM
 * 
 * @param errors        a buffer to store all of the errors to
 * @param maxErrors     the maximum number of errors that can fit in the buffer
 */
void EEPROM_GetErrors(Fault_Set *errors, uint16_t maxErrors) {
    uint16_t numErrors = 0;
    for (uint16_t addr = EEPROM_FAULT_ADDR; (addr < faultArrayEndAddress) && (numErrors < maxErrors);
            addr += sizeof(Fault_Set), ++numErrors) {
        M24128_Read(addr, sizeof(Fault_Set), (uint8_t *) (errors + numErrors));
    }
}
