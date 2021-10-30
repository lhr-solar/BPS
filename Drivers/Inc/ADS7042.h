#ifndef ADS7042_H
#define ADS7042_H

// Driver for ADS7042 ADC used for sensing current

#include "common.h"
#include "config.h"
#include "BSP_OS.h"

/* Initialize communication ADS7042
 * and run startup offset calibration
 */
void ADS7042_Init(bsp_os_t spi_os);

/* Read value from ADS7042
 * ADS7042 outputs 2 bits of 0 + 12 data bits + 2 bits of 0
 * This function returns just the data bits
 * @param data pointer to data buffer
 * @return SUCCESS or ERROR
 */
uint16_t ADS7042_Read();

/* Gets value from ADS7042
 * @return current in milliamps
 */
int32_t ADS7042_GetCurrent();

#endif