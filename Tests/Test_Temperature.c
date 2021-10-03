/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "Temperature.h"
#include "LTC6811.h"
#include "BSP_UART.h"

/******************************************************************************
 * Temperature App Test Plan
 * 
 * 1. The datasheet has a list of mappings from temperatures to millivoltages.
 * I have listed a few of them below. Apply the following voltages to the 
 * temperatures sensor probes, and verify the the relationship holds.
 * 
 * Temperature (Degrees C)      Voltage (mV)
 * 20	                        2365
 * 25	                        2298
 * 30	                        2231
 * 35	                        2163
 * 40	                        2095
 * 45	                        2026
 * 50	                        1958
 * 55	                        1888
 * 60	                        1819
 * 65	                        1749
 * 70	                        1679
 * 75	                        1609
 * 80	                        1539
 * 85	                        1469
 * 
 * 2. Connect the temperature sensors and see if the measurements match the
 * ambient temperature (verify with a themometer or by checking the thermostat).
 * 
 * 3. Put the temperature sensors in the oven? and see if they match the 
 * temperature at 45 degrees C, 60 degrees C, and 75 degrees C
 * 
 *****************************************************************************/

cell_asic minions[NUM_MINIONS];

static void sleep(int n) {
    for (volatile int i = 0; i < n * 1000000; ++i);
}

int main() {

    BSP_UART_Init(NULL, NULL, UART_USB);    // Initialize printf

    printf("Testing Temperature functions.\r\n");

    Temperature_Init(minions);

    Temperature_UpdateAllMeasurements();

    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("Minion %d:\r\n", i);
        printf("\tTemperature:\r\n");

        for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
            printf("\t%d: %ldmC\r\n", j, Temperature_GetSingleTempSensor(i, j));
        }
    }

    sleep(1);

    printf("Testing Temperature functions in loop.\r\n");

    uint8_t isCharging = 0;

    while(1) {
        printf("All good!\r");

        Temperature_UpdateAllMeasurements();

        if(Temperature_CheckStatus(isCharging) != SAFE) {
            printf("DANGER!! Temperature levels in danger :(\r\n");
            break;
        }
    }

    printf("Printing temperature values.\r\n");
    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("Minion %d:\r\n", i);
        printf("\tTemperature:\r\n");

        for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
            printf("\t%d: %ldmC\r\n", j, Temperature_GetSingleTempSensor(i, j));
        }
    }
}
