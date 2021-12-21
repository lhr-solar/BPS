#ifndef LTC2315_H
#define LTC2315_H

// Driver for LTC2315 ADC used for sensing current

#include "common.h"
#include "config.h"
#include "BSP_OS.h"

/* Initialize communication LTC2315
 */
void LTC2315_Init(bsp_os_t spi_os);

/* Enter sleep mode
 */
void LTC2315_Sleep();

/* Read value from LTC2315
 * LTC2315 outputs 2 bits of 0 + 12 data bits + 2 bits of 0
 * This function returns just the data bits
 * @return Raw 12-bit value read from the ADC
 */
uint16_t LTC2315_Read();

/* Gets value from LTC2315
 * @return current in milliamps
 */
int32_t LTC2315_GetCurrent();

#endif