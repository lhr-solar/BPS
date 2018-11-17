// ICM-20600
/**
 * Driver for ICM-20600 chip related functions.
 * @authors Chase Block
 * @lastRevised 10/13/2018
 */

#ifndef GYRO_H__
#define GYRO_H__

#include "stm32f4xx.h"
#include "SPI.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define DRDY_CONF	0x37
#define OUT_X_MSB	0x43
#define OUT_X_LSB	0x44
#define OUT_Y_MSB	0x45
#define OUT_Y_LSB	0x46
#define OUT_Z_MSB	0x47
#define OUT_Z_LSB	0x48
#define GNRL_CONF       0x1A
#define GYRO_CONF       0x1B
#define ACCL_CONF       0x1C

/** ICM20600_Init
 * Initializes SPI for FXAS21002CQR1
 * Initializes and configures LTC6811
 */
void ICM20600_Init(void);

/** ICM20600_SendCmd
 * Sends command data to FXAS21002CQR1
 * @param unsigned int 16-bit data
 */
uint16_t *ICM20600_SendCmd(uint8_t address, uint8_t data, uint8_t rw, uint32_t readSize);

/** ICM20600_Measure
 * Sends command to FXAS21002CQR1 to gather and save all heading values
 * @return unsigned int 16-bit measurements from all axes {x,y,z}
 */
uint16_t *ICM20600_Measure(void);

/** ICM20600_CurrentHeading
 * Updates the heading before returning
 * This should be called periodically
 * @return the current heading of the car
 */
double *ICM20600_CurrentHeading(void);

/** ICM20600_ResetHeadings
 * Resets the heading values
 */
void ICM20600_ResetHeadings(void);

/**
 * @return 0 if the car is not flipped over, 1 if it is
 */
uint8_t ICM20600_IsFlipped(void);

#endif
