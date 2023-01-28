/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "EEPROM.h"
#include "BSP_UART.h"
#include "Tasks.h"
#include <stdio.h>

int main(void) {
    uint32_t charge;
    static uint32_t errors[10];
    uint16_t numErrors;

    BSP_UART_Init(NULL, NULL, UART_USB);

    printf("Hello World!\n\r");

    EEPROM_Init();

    printf("EEPROM Initialized\n\r");

    // read state of charge
    charge = EEPROM_GetCharge();

    // read errors
    numErrors = EEPROM_GetErrors(errors, 10);

    // print to UART
    printf("state of charge: %lu.%.6lu\n\r", charge / 1000000, charge % 1000000);
    printf("number of errors: %u\n\r", numErrors);
    for (uint16_t i = 0; i < numErrors; ++i) {
        printf("error: %lx\n\r", errors[i]);
    }

    while(1);

    return 0;
}
