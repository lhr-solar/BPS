/* Copyright (c) 2022 UT Longhorn Racing Solar */

/** EEPROM.c
 * Interface for the EEPROM. Used for keeping track of state of charge and errors across resets.
 */

#include "EEPROM.h"
#include "M24128.h"

// addresses for EEPROM data segments
#define EEPROM_CHARGE_ADDR  0x0
static const uint16_t EEPROM_FAULT_ADDR  = (EEPROM_CHARGE_ADDR + sizeof(uint32_t));
static const uint16_t MAX_FAULTS = 100;

// terminating character for fault array
static const uint32_t EEPROM_TERMINATOR = 0xFFFFFFFF;

// cached value of end of fault array
static uint16_t faultArrayEndAddress;

// handle errors from M24128 driver
static uint32_t EEPROM_driverErrorCount = 0;
#define EEPROM_RETRY(func) while ((func) == ERROR) { ++EEPROM_driverErrorCount; }

/** EEPROM_Init
 * Initializes EEPROM application
 */
void EEPROM_Init(void) {
    // initialize the EEPROM
    M24128_Init();

    // find the end of the fault array
    uint32_t data = 0;
    faultArrayEndAddress = EEPROM_FAULT_ADDR - sizeof(EEPROM_TERMINATOR);
    while (data != EEPROM_TERMINATOR) {
        if (faultArrayEndAddress > EEPROM_FAULT_ADDR + sizeof(EEPROM_TERMINATOR) * MAX_FAULTS) {
            EEPROM_Reset();
            faultArrayEndAddress = EEPROM_FAULT_ADDR;
            break;
        }
        faultArrayEndAddress += sizeof(EEPROM_TERMINATOR);
        EEPROM_RETRY(M24128_Read(faultArrayEndAddress, sizeof(data), (uint8_t *) &data)); // retry if unsuccessful
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
    EEPROM_RETRY(M24128_Write(EEPROM_FAULT_ADDR, sizeof(EEPROM_TERMINATOR), (uint8_t *) &EEPROM_TERMINATOR)); // retry if unsuccessful

    faultArrayEndAddress = EEPROM_FAULT_ADDR;
}

/**
 * @brief Gets the stored state of charge from the EEPROM
 * 
 * @return uint32_t stored charge value
 */
uint32_t EEPROM_GetCharge(void) {
    uint32_t charge;
    EEPROM_RETRY(M24128_Read(EEPROM_CHARGE_ADDR, sizeof(charge), (uint8_t *) &charge)); // retry if unsuccessful
    return charge;
}

/**
 * @brief Set the stored state of charge value in the EEPROM
 * 
 * @param charge The value to set the EEPROM's stored state of charge to
 */
void EEPROM_SetCharge(uint32_t charge) {
    // this gets called a lot during normal operation, and I think it is ok if it fails sometimes, so I won't make it retry
    ErrorStatus result = M24128_Write(EEPROM_CHARGE_ADDR, sizeof(charge), (uint8_t *) &charge);
    if (result == ERROR) {
        EEPROM_driverErrorCount++; // log that we had an error
    }
}

/**
 * @brief Log an error to the EEPROM
 * 
 * @param error The error to log
 */
void EEPROM_LogError(uint32_t error) {
    // assumes sizeof(EEPROM_TERMINATOR) == sizeof(error)
    // retry writes if unsuccessful
    EEPROM_RETRY(M24128_Write(faultArrayEndAddress + sizeof(EEPROM_TERMINATOR), sizeof(EEPROM_TERMINATOR), (uint8_t *) &EEPROM_TERMINATOR));
    EEPROM_RETRY(M24128_Write(faultArrayEndAddress, sizeof(error), (uint8_t *) &error));
    faultArrayEndAddress += sizeof(error);
}

/**
 * @brief Get all of the errors store in the EEPROM
 * 
 * @param errors        a buffer to store all of the errors to
 * @param maxErrors     the maximum number of errors that can fit in the buffer
 * @return              the number of errors read
 */
uint16_t EEPROM_GetErrors(uint32_t *errors, uint16_t maxErrors) {
    uint16_t numErrors = 0;
    uint16_t addr = EEPROM_FAULT_ADDR;
    for (; (addr < faultArrayEndAddress) && (numErrors < maxErrors); ++numErrors) {
        EEPROM_RETRY(M24128_Read(addr, sizeof(*errors), (uint8_t *) (errors + numErrors))); // retry if unsuccessful
        addr += sizeof(*errors);
    }
    return numErrors;
}
