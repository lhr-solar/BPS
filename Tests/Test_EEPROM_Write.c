/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "EEPROM.h"
#include "Tasks.h"
#include "BSP_UART.h"

int main(void) {

    BSP_UART_Init(NULL, NULL, UART_USB);
    EEPROM_Init();

    // log dummy state of charge
    EEPROM_SetCharge(50 * 1000000);

    // log some dummy faults
    EEPROM_LogError(Fault_OVOLT);
    EEPROM_LogError(Fault_OTEMP);
    EEPROM_LogError(Fault_ESTOP);

    printf("Finished writing to EEPROM\n\r");

    while(1);

    return 0;
}
