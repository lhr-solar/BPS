/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** EEPROM.c
 * Interface for the EEPROM. Used for keeping track of state of charge and errors across resets.
 */

#include "EEPROM.h"
#include "M24128.h"

// addresses for EEPROM data segments
#define EEPROM_DYNAMIC_INIT_ADDR 0x400
#define EEPROM_ERRORS_INIT_ADDR 0x4
#define EEPROM_MAX_ADDR 0x4000
#define EEPROM_PAGE_SIZE_BYTES 64
#define EEPROM_LINKED_LIST_SENTINEL 0x0000

static uint16_t dynamic_addr = EEPROM_DYNAMIC_INIT_ADDR;

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

void EEPROM_Retrieve_Last_Addr(void) {
    for (int i = 0; i < (EEPROM_MAX_ADDR - EEPROM_DYNAMIC_INIT_ADDR) / EEPROM_PAGE_SIZE_BYTES; i++) {
        uint16_t data[EEPROM_PAGE_SIZE_BYTES/2];
        EEPROM_RETRY(M24128_Read(EEPROM_DYNAMIC_INIT_ADDR + EEPROM_PAGE_SIZE_BYTES * i, EEPROM_PAGE_SIZE_BYTES, (uint8_t*) data));

        if (data[EEPROM_PAGE_SIZE_BYTES / 2 - 1] == EEPROM_LINKED_LIST_SENTINEL) {
            dynamic_addr = EEPROM_DYNAMIC_INIT_ADDR + EEPROM_PAGE_SIZE_BYTES * i;
            return;
        }
    }
}

/** EEPROM_Init
 * Initializes EEPROM application
 */
void EEPROM_Init(void) {
    // initialize the EEPROM
    M24128_Init();

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
    // On initialization, set our starting charge address to the one we saved on shutdowns.
    EEPROM_Retrieve_Last_Addr();
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
 * @param data    Address of uint32_t* that stores the 64 bytes; set to NULL if no available previous charge; else data returned in void* of size EEPROM_PAGE_SIZE_BYTES - 2 with last byte containing the length of array
 */

void EEPROM_GetLastData(void* data) {
    uint16_t prev_data[EEPROM_PAGE_SIZE_BYTES / 2];
    uint16_t read_addr = (dynamic_addr - EEPROM_DYNAMIC_INIT_ADDR + (EEPROM_MAX_ADDR - EEPROM_DYNAMIC_INIT_ADDR - EEPROM_PAGE_SIZE_BYTES)) % (EEPROM_MAX_ADDR - EEPROM_PAGE_SIZE_BYTES) + EEPROM_DYNAMIC_INIT_ADDR;;

    EEPROM_RETRY(M24128_Read(read_addr, EEPROM_PAGE_SIZE_BYTES, (uint8_t *) &prev_data)); // retry if unsuccessful

    uint16_t last_addr = prev_data[EEPROM_PAGE_SIZE_BYTES / 2 - 1];
    if (last_addr != dynamic_addr) {
        return NULL;
    }
    
    memcpy(data, prev_data, EEPROM_PAGE_SIZE_BYTES - 2);
    
    return data;
}

/**
 * @brief Write new data to next page of EEPROM
 * 
 * @param data  pointer to array of integers
 * @param len   defines length of data array; range is up to (EEPROM_PAGE_SIZE_BYTES - EEPROM_ADDRESS_BITS / 8) / 4
 * 
 * @return      error code; -1 indicates an error in the EEPROM write; -2 indicates an out of range len input; 0 indicates successful write
 */

int8_t EEPROM_Write(void* data, uint8_t len) {
    // this gets called a lot during normal operation, and I think it is ok if it fails sometimes, so I won't make it retry

    // Sets available space for data with the total page size - the space for the length of the data - the space for the next address
    uint32_t data_space = EEPROM_PAGE_SIZE_BYTES - 3;

    // Checks len to see if in range
    if (len > data_space) {
        return -2;
    }

    // Move dynamic_addr to next page
    dynamic_addr = (dynamic_addr + EEPROM_PAGE_SIZE_BYTES - EEPROM_DYNAMIC_INIT_ADDR) % (EEPROM_MAX_ADDR - EEPROM_DYNAMIC_INIT_ADDR) + EEPROM_DYNAMIC_INIT_ADDR;

    // Add sentinel and length to end of data
    uint8_t* end_len = (uint8_t *) data;
    end_len += data_space;
    *end_len = len;
    uint16_t* end_addr = (uint16_t *) end_len + 1;
    *end_addr = EEPROM_LINKED_LIST_SENTINEL;

    // Write data
    ErrorStatus result = M24128_Write(dynamic_addr, EEPROM_PAGE_SIZE_BYTES, (uint8_t *) data);

    if (result == ERROR) {
        EEPROM_driverErrorCount++; // log that we had an error
        return -1;
    } else {
        // increment the location we write to every time if we succeed
        /*
            To cyclically update the write address from 0x400 -> 0x3FFC, we apply the following:
            1) Subtract 0x3FC (-0x400, + 0x4)
            2) Modulo 0x3C00 (0x4000 total bytes we can write - 0x400 [starting addr for SOC writes])
            3) Add 0x400
            4) This is next write address
            
            1) Subtract 0x400
            2) Add 0x3BC0
            3) Modulo 3C00
            4) Add 0x400.
        */

        uint16_t last_address_ptr = (dynamic_addr - EEPROM_DYNAMIC_INIT_ADDR + (EEPROM_MAX_ADDR - EEPROM_DYNAMIC_INIT_ADDR - EEPROM_PAGE_SIZE_BYTES)) % (EEPROM_MAX_ADDR - EEPROM_PAGE_SIZE_BYTES) + EEPROM_DYNAMIC_INIT_ADDR;
        result = M24128_Write(last_address_ptr, sizeof(dynamic_addr), (uint8_t *) &dynamic_addr);
        if (result == ERROR) EEPROM_driverErrorCount++; // log that we had an error
        else return 0;
    }
    return -1;
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
    uint16_t addr = EEPROM_ERRORS_INIT_ADDR;
    for (; (addr < faultArrayEndAddress) && (numErrors < maxErrors); ++numErrors) {
        EEPROM_RETRY(M24128_Read(addr, sizeof(*errors), (uint8_t *) (errors + numErrors))); // retry if unsuccessful
        addr += sizeof(*errors);
    }
    return numErrors;
}