/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_I2C.h"
#include "EEPROM.h"
#include "config.h"
#include "Simulator.h"

static bool initialized = false;

/**
 * @brief   Initializes the I2C port that interfaces with the EEPROM. Creates EEPROM txt file if it does not already exist
 * @param   None
 * @return  None
 */
void BSP_I2C_Init(void) {
    initialized = true;
    Simulator_log("EEPROM Initialized\n");
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t  BSP_I2C_Write(uint8_t devAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
    if (!initialized) {
        Simulator_log("Error: used I2C without initialization!\n");
        exit(-1);
    }

    char buffer[100];
    for (uint32_t i = 0; i < txLen; ++i) {
        sprintf(buffer, "Wrote 0x%x to I2C device 0x%x at address 0x%x\n", txData[i], devAddr, regAddr);
        Simulator_log(buffer);
    }

    return SUCCESS;
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t BSP_I2C_Read(uint8_t devAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
    if (!initialized) {
        Simulator_log("Error: used I2C without initialization!\n");
        exit(-1);
    }

    if (rxLen != sizeof(uint32_t)) {
        Simulator_log("Error: simulator only supports reading state of charge!\n");
        exit(-1);
    }
    
    // blindly hope that this is only ever used to read state of charge
    uint32_t charge = Simulator_getCharge();
    memcpy(rxData, &charge, sizeof(charge));

    return SUCCESS;
}