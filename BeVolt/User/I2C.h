#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include "stm32f4xx.h"

// PLEASE NOTE: Make sure deviceAddr is left shifted by 1

void I2C3_Init(void);

uint8_t I2C3_Read(uint8_t deviceAddr, uint16_t regAddr);

void I2C3_ReadMultiple(uint8_t deviceAddr, uint16_t startAddr, uint8_t *rxData, uint32_t rxSize);

void I2C3_Write(uint8_t deviceAddr, uint16_t regAddr, uint8_t data);

void I2C3_WriteMultiple(uint8_t deviceAddr, uint16_t startAddr, uint8_t *txData, uint32_t txSize);

#endif
