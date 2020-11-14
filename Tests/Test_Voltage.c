/* Copyright (c) 2020 UT Longhorn Racing Solar */

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

    printf("Testing Voltage functions in loop.\r\n");

    while(1) {

        printf("All good!\r");

        Voltage_UpdateMeasurements();
        
        printf("Printing voltage values.\r\n");
        for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
            printf("\t%d: %dmV\r\n", i, Voltage_GetModuleMillivoltage(i));
        }

        if(Voltage_CheckStatus() != SAFE) {
            printf("DANGER!! Voltage Levels in Danger :(\r\n");
            break;
        }
        
        if(Voltage_OpenWire() != SAFE) {
            printf("DANGER!! There is an open wire :(\r\n");
            break;
        }

    }

    printf("Printing voltage values.\r\n");
    for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
        printf("\t%d: %dmV\r\n", i, Voltage_GetModuleMillivoltage(i));
    }

    printf("Printing pins open wires.\r\n");
    for(int i = 0; i < NUM_MINIONS; i++) {
        printf("\tMinion %d: 0x%lx\r\n", i, minions[i].system_open_wire);
    }

    printf("Printing modules that failed.\r\n");
    Safety_Info dangerBatt = Voltage_GetModulesInDanger();
    for(int i = 0; i < TOTAL_VOLT_WIRES; i++) {
        printf("\t%d: ", i);
        if(i < NUM_BATTERY_MODULES){
            if(dangerBatt.module_checks[i] == SAFE && dangerBatt.wire_checks[i] == SAFE){
                printf("SAFE\r\n");
            }
            else{
                printf("DANGER\r\n");
            }
        }
        else{
            if(dangerBatt.wire_checks[i] == SAFE) {
                printf("SAFE\r\n");

            // TODO: once Voltage_GetModulesInDanger is updated, add the specific cases for UNDERVOLTAGE, OVERVOLTAGE, OPENWIRE
            } else {
                printf("DANGER\r\n");
            }
        }
        
    }

    while(1);
}
