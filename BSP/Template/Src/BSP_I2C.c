#include "BSP_I2C.h"

/**
 * @brief   Initializes the I2C port that interfaces with the EEPROM.
 * @param   None
 * @return  None
 */
void BSP_I2C_Init(void) {
    // TODO: Initialize the I2C pins connected to the EEPROM.
    //      By default, the code assumes the I2C bus is running at 100kHz
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param   deviceAddr : the device/IC that the data is intended for.
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t  BSP_I2C_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
    // TODO: Transmit the data onto the I2C bus. Packet the data as needed.

    return 0;
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   deviceAddr : the device/IC that the data needs to be read from.
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t BSP_I2C_Read(uint8_t deviceAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
    // TODO: Get the data from the other device/IC.
    //      The I2C bus has only two lines, the Clock and Data pins. The process for reading data
    //      is to first transmit the regAddr then immediately read.

    return 0;
}
