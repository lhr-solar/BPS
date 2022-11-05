/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "BSP_ADC.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>

// Path relative to the executable
static const char* file = GET_CSV_PATH(ADC_CSV_FILE);

/**
 * @brief   Initializes the ADC module. This is to measure the hall effect sensors
 *          on the Current Monitor Board.
 * @param   None
 * @return  None
 */
void BSP_ADC_Init(void) {
    // Check if simulator is running i.e. were the csv files created?
    if(access(file, F_OK) != 0) {
        // File doesn't exit if true
        perror(ADC_CSV_FILE);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_High_GetMilliVoltage(void) {
    FILE* fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(ADC_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/ADC.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    // Get raw CSV string
    char csv[16];
    fgets(csv, 16, fp);
    // Get only 2nd (high precision) value
    char* dataString = csv;
    dataString += 5;
    // Convert to int
    uint16_t data = atoi(dataString);
    // Convert to millivoltage
    uint16_t mV = (data * 3300) >> 12;   // For 12-bit ADCs

    // Unlock the lock so the simulator can write to ADC.csv again
    flock(fno, LOCK_UN);

    fclose(fp);
    return mV;
}

/**
 * @brief   Gets converted ADC value in units of mV.
 * @param   None
 * @return  millivoltage value ADC measurement
 */
uint16_t BSP_ADC_Low_GetMilliVoltage(void) {
    FILE* fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(ADC_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/ADC.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    // Get raw CSV string
    char csv[16];
    fgets(csv, 16, fp);
    // Get only 1st (low precision) value
    char* dataString = csv;
    *(dataString+5) = 0;
    dataString = csv;
    // Convert to int
    uint16_t data = atoi(dataString);
    // Convert to millivoltage
    uint16_t mV = (data * 3300) >> 12;   // For 12-bit ADCs

    // Unlock the lock so the simulator can write to ADC.csv again
    flock(fno, LOCK_UN);

    fclose(fp);

    return mV;
}

