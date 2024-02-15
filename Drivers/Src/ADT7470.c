/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
/** ADT740.h
 *  Functions for using the ADT740
 */

#include "BSP_I2C.h"
#include "ADT7470.h"

#define AddressLength 8
#define ADT740_ADDRESS (0x5E) // left justified
#define CFG_1 (0x40)
#define CFG_2 (0x74)


/*
* ADT740_Init initializes I2C to communicate with ADT740
*/
void ADT740_Init(void) {
    BSP_I2C_Init();
    BSP_I2C_Write(ADT740_ADDRESS, CFG_1, (CFG_1 | 0x01), AddressLength); //set the ADT740 to monitoring mode
}

// to do: read the register first then OR it with bit 0
void enterShutdown(){
    BSP_I2C_Write(ADT740_ADDRESS, CFG_2, (CFG_2 | 0x01), AddressLength);
}