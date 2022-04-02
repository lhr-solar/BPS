/* Copyright (c) 2022 UT Longhorn Racing Solar */

/** M24128.c
 * Driver for STM's M24128
 * The EEPROM is 128Kbits (16k x 8 bits) and uses I2C
 */

#include <stdint.h>
#include "BSP_I2C.h"

#define M24128_ADDRESS (0xA0)

/** M24128_Init
 * Initializes I2C to communicate with M24128
 */
void M24128_Init(void) {
    BSP_I2C_Init();
}

/** M24128_Write
 * Saves data to the M24128
 * @param address to write data to
 * @param bytes number of bytes to send
 * @param buffer pointer to buffer that contains data
 */
void M24128_Write(uint16_t address, uint32_t bytes, uint8_t* buffer) {
    BSP_I2C_Write(M24128_ADDRESS, address, buffer, bytes);
}

/** M24128_Read
 * Gets multiple bytes of data sequentially from M24128 beginning at specified address
 * @param address to read data from
 * @param bytes number of bytes to read
 * @param buffer pointer to buffer to store data in
 */
void M24128_Read(uint16_t address, uint32_t bytes, uint8_t* buffer) {
    BSP_I2C_Read(M24128_ADDRESS, address, buffer, bytes);
}
