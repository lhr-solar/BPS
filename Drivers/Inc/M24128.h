/* Copyright (c) 2022 UT Longhorn Racing Solar */

/** M24128.h
 * Driver for STM's M24128
 * The EEPROM is 128Kbits (16k x 8 bits) and uses I2C
 */

#ifndef M24128_H
#define M24128_H

#include <stdint.h>
#include "config.h"

/** M24128_Init
 * Initializes I2C to communicate with M24128
 */
void M24128_Init(void);

/** M24128_Write
 * Saves data to the M24128
 * @param address to write data to
 * @param bytes number of bytes to send
 * @param buffer pointer to buffer that contains data
 * @return ERROR or SUCCESS
 */
ErrorStatus M24128_Write(uint16_t address, uint32_t bytes, uint8_t* buffer);

/** M24128_Read
 * Gets multiple bytes of data sequentially from M24128 beginning at specified address
 * @param address to read data from
 * @param bytes number of bytes to read
 * @param buffer pointer to buffer to store data in
 * @return ERROR or SUCCESS
 */
ErrorStatus M24128_Read(uint16_t address, uint32_t bytes, uint8_t* buffer);

#endif
