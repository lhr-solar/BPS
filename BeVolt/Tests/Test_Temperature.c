#include "common.h"
#include "config.h"
#include "Temperature.h"
#include "LTC6811.h"
#include "BSP_UART.h"

cell_asic minions[NUM_MINIONS];

int main() {

    BSP_UART_Init();    // Initialize printf

    printf("Testing Temperature\r\n");

    Temperature_Init(minions);

    Temperature_UpdateSingleChannel(0);
    Temperature_UpdateSingleChannel(1);
    Temperature_UpdateSingleChannel(2);

    Temperature_UpdateAllMeasurements();

    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("Minion %d:\r\n", i);
        printf("\tTemperature:\r\n");

        for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
            printf("\t%d: %dmC\r\n", j, Temperature_GetSingleTempSensor(i, j));
        }
    }

    while(1) {
        
    }
}
