#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "common.h"

void BSP_SPI_Init(void);

void BSP_SPI_Write(uint8_t *txBuf, uint32_t txLen);

void BSP_SPI_Read(uint8_t *rxBuf, uint32_t rxLen);

void BSP_SPI_SetStateCS(uint8_t state);

#endif
