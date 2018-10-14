// FXAS21002CQR1.h
/**
 * Driver for FXAS21002CQR1 chip related functions. 
 * @authors Chase Block
 * @lastRevised 10/13/2018
 */

#ifndef FXAS2100_H__
#define FXAS2100_H__

#include "stm32f4xx.h"
#include <stdint.h>

#define STATUS		0x00
#define OUT_X_MSB	0x01
#define OUT_X_LSB	0x02
#define OUT_Y_MSB	0x03
#define OUT_Y_LSB	0x04
#define OUT_Z_MSB	0x05
#define OUT_Z_LSB	0x06
#define CTRL_REG1	0x13
#define CTRL_REG2	0x14
#define CTRL_REG3	0x15

double headings [3]; // x,y,z

/** FXAS21002CQR1_Init
 * Initializes SPI for FXAS21002CQR1
 * Initializes and configures LTC6811
 */
void FXAS21002CQR1_Init(void);

/** FXAS21002CQR1_SendCmd
 * Sends command data to FXAS21002CQR1
 * @param unsigned int 16-bit data
 */
uint16_t *FXAS21002CQR1_SendCmd(uint8_t address, uint8_t data, uint8_t rw, uint32_t readSize);

/** FXAS21002CQR1_Measure
 * Sends command to FXAS21002CQR1 to gather and save all heading values
 * @return unsigned int 16-bit measurements from all ADC's
 */
uint16_t *FXAS21002CQR1_Measure(void);

/** FXAS21002CQR1_CurrentHeading
 * Updates the heading before returning
 * This should be called periodically
 * @return the current heading of the car
 */
double *FXAS21002CQR1_CurrentHeading(void);
 
/** FXAS21002CQR1_ResetHeadings
 * Resets the heading values
 */
void FXAS21002CQR1_ResetHeadings(void);

/**
 * @return 0 if the car is not flipped over, 1 if it is
 */
uint8_t isFlipped(void);

#endif
