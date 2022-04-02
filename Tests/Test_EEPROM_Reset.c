/* Copyright (c) 2022 UT Longhorn Racing Solar */

/**
 * @file Test_Reset_EEPROM.c
 * @author Clark Poon
 * @brief Test program to reset the EEPROM, so it can be used by other programs
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "EEPROM.h"

int main(void) {
    EEPROM_Reset();

    while(1);

    return 0;
}
