#include "common.h"
#include "config.h"
#include "Temperature.h"
#include "LTC6811.h"
#include "BSP_UART.h"
#include <unistd.h>

cell_asic minions[NUM_MINIONS];

int main() {

    BSP_UART_Init();    // Initialize printf

    printf("Testing Temperature functions.\r\n");

    Temperature_Init(minions);

    Temperature_UpdateAllMeasurements();

    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("Minion %d:\r\n", i);
        printf("\tTemperature:\r\n");

        for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
            printf("\t%d: %dmC\r\n", j, Temperature_GetSingleTempSensor(i, j));
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
            printf("\t%d: %dmC\r\n", j, Temperature_GetSingleTempSensor(i, j));
        }
    }
}
