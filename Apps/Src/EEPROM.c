/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** EEPROM.c
 * Interface for the EEPROM. Used for keeping track of state of charge and errors across resets.
 */

#include "EEPROM.h"
#include "M24128.h"

// addresses for EEPROM data segments
#define EEPROM_CHARGE_INIT_ADDR 0x400
#define EEPROM_ERRORS_INIT_ADDR 0x4
#define EEPROM_MAX_ADDR 0x3FFC
#define EEPROM_MAX_CHARGE_ENTRIES 0xEFF

static uint16_t charge_dynamic_addr = EEPROM_CHARGE_INIT_ADDR;
static uint16_t performedWrites = 0;

static const uint16_t MAX_FAULTS = 100;

// terminating character for fault array
static const uint32_t EEPROM_TERMINATOR = 0xFFFFFFFF;

// cached value of end of fault array
static uint16_t faultArrayEndAddress;

// variables to keep track of repeated EEPROM communcation failures and to disable it if there are too many failures
static bool EEPROM_errorLoggingDisabled = false;
static uint16_t EEPROM_consecutiveFaults = 0;
static const uint16_t EEPROM_MAX_CONSECUTIVE_FAULTS = 5;

// handle errors from M24128 driver
static uint32_t EEPROM_driverErrorCount = 0;
#define EEPROM_RETRY(func) while ((func) == ERROR) { \
        ++EEPROM_driverErrorCount; \
        ++EEPROM_consecutiveFaults; \
        EEPROM_errorLoggingDisabled = (EEPROM_consecutiveFaults > EEPROM_MAX_CONSECUTIVE_FAULTS); \
        if (EEPROM_errorLoggingDisabled) break; \
    } \
    EEPROM_consecutiveFaults = 0;

/** EEPROM_Init
 * Initializes EEPROM application
 */
void EEPROM_Init(void) {
    // initialize the EEPROM
    M24128_Init();
    performedWrites = 0;
    // find the end of the fault array
    uint32_t data = 0;
    faultArrayEndAddress = EEPROM_ERRORS_INIT_ADDR - sizeof(EEPROM_TERMINATOR);
    while (data != EEPROM_TERMINATOR) {
        if (EEPROM_errorLoggingDisabled) {
            return;
        }
        if (faultArrayEndAddress > EEPROM_ERRORS_INIT_ADDR + sizeof(EEPROM_TERMINATOR) * MAX_FAULTS) {
            EEPROM_Reset();
            faultArrayEndAddress = EEPROM_ERRORS_INIT_ADDR;
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
    EEPROM_RETRY(M24128_Write(EEPROM_ERRORS_INIT_ADDR, sizeof(EEPROM_TERMINATOR), (uint8_t *) &EEPROM_TERMINATOR)); // retry if unsuccessful

    faultArrayEndAddress = EEPROM_ERRORS_INIT_ADDR;
}

/**
 * @brief Gets the stored state of charge from the EEPROM
 * 
 * @return uint32_t stored charge value
 */
uint32_t EEPROM_GetLastCharge(void) {
    uint32_t charge = -1;
    uint16_t read_addr;
    // dont read out of bounds on edge cases
    if (performedWrites == 0) {
        read_addr = charge_dynamic_addr;
    } else {
        charge_dynamic_addr += (0x3BFC - EEPROM_CHARGE_INIT_ADDR);
        charge_dynamic_addr %= 0x3C00;
        charge_dynamic_addr += EEPROM_CHARGE_INIT_ADDR;
        // (charge_dynamic_addr - 400) + 3BFC) % 3C00 ) + 400 
    }
    EEPROM_RETRY(M24128_Read(charge_dynamic_addr, sizeof(charge), (uint8_t *) &charge)); // retry if unsuccessful
    return charge;
}

/**
 * @brief Get a specific state of charge entry from the EEPROM
 * 
 * @param numEntry Which state of charge entry to read (i.e. entry 0, entry 1)
 * @return uint32_t 
 */
uint32_t EEPROM_GetChargeEntry(uint16_t entry_num) {
    uint32_t charge = -1;
    // EEPROM memory arranged as 16 K * 8 bit chunks
    // SOC vals are 4 bytes each so shift "entry num" left 2 to * 4
    // [0x3ffc (max addr we can use) - 0x400] >> 2 = 0xEFF # of entries in the SOC write region
    uint16_t offset = (entry_num << 2) % 0xEFF;
    uint16_t readAddress = EEPROM_CHARGE_INIT_ADDR + offset;
    EEPROM_RETRY(M24128_Read(readAddress, sizeof(charge), (uint8_t *) &charge));
    return charge;
}

/**
 * @brief Set the stored state of charge value in the EEPROM
 * 
 * @param charge The value to set the EEPROM's stored state of charge to
 */
void EEPROM_SetCharge(uint32_t charge) {
    // this gets called a lot during normal operation, and I think it is ok if it fails sometimes, so I won't make it retry
    ErrorStatus result = M24128_Write(charge_dynamic_addr, sizeof(charge), (uint8_t *) &charge);
    if (result == ERROR) {
        EEPROM_driverErrorCount++; // log that we had an error
    } else {
        // increment the location we write to every time if we succeed
        /*
            To cyclically update the write address from 0x400 -> 0x3FFC, we apply the following:
            1) Subtract 0x3FC (-0x400, + 0x4)
            2) Modulo 0x3C00 (0x4000 total bytes we can write - 0x400 [starting addr for SOC writes])
            3) Add 0x400
            4) This is next write address.
        */
        charge_dynamic_addr -= 0x3FC;
        charge_dynamic_addr %= 0x3C00;
        charge_dynamic_addr += 0x400;
        performedWrites++;
    }
}

/**
 * @brief Log an error to the EEPROM
 * 
 * @param error The error to log
 */
void EEPROM_LogError(uint32_t error) {
    //TODO: THIS IS UNTIL WE FIX THESE DRIVERS. ISSUE TICKET ON GITHUB
    if (EEPROM_errorLoggingDisabled) return;
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
    //TODO: THIS IS UNTIL WE FIX THESE DRIVERS. ISSUE TICKET ON GITHUB
    if (EEPROM_errorLoggingDisabled) return 0;

    uint16_t numErrors = 0;
    uint16_t addr = EEPROM_ERRORS_INIT_ADDR;
    for (; (addr < faultArrayEndAddress) && (numErrors < maxErrors); ++numErrors) {
        EEPROM_RETRY(M24128_Read(addr, sizeof(*errors), (uint8_t *) (errors + numErrors))); // retry if unsuccessful
        addr += sizeof(*errors);
    }
    return numErrors;
}
