#include "common.h"
#include "config.h"
#include "Voltage.h"
#include "BSP_UART.h"
#include <unistd.h>

cell_asic minions[NUM_MINIONS];

int main() {

    BSP_UART_Init();    // Initialize printf

    printf("Testing Voltage functions.\r\n");

    Voltage_Init(minions);

    Voltage_UpdateMeasurements();

    for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
        printf("\t%d: %dmV\r\n", i, Voltage_GetModuleMillivoltage(i));
    }

    sleep(1);

    printf("Testing Voltage functions in loop.\r\n");

    while(1) {

        printf("All good!\r");

        Voltage_UpdateMeasurements();
        
        if(Voltage_CheckStatus() != SAFE) {
            printf("DANGER!! Voltage Levels in Danger :(\r\n");
            break;
        }

        if(Voltage_OpenWire() != SAFE) {
            printf("DANGER!! There is an open wire :(\r\n");
            break;
        }

    }

    printf("Printing voltage modules.\r\n");
    for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
        printf("\t%d: %dmV\r\n", i, Voltage_GetModuleMillivoltage(i));
    }

    printf("Printing modules that failed.\r\n");
    SafetyStatus *dangerBatt = Voltage_GetModulesInDanger();
    for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
        printf("\t%d: ", i);
        if(dangerBatt[i] == SAFE) {
            printf("SAFE\r\n");

            // TODO: once Voltage_GetModulesInDanger is updated, add the specific cases for UNDERVOLTAGE, OVERVOLTAGE, OPENWIRE
        } else {
            printf("DANGER\r\n");
        }
    }
}
