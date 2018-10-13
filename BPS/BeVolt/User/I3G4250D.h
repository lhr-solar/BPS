// I3G4250D.h
/**
 * Driver for I3G4250D chip related functions. The IC uses SPI to communicate to
 * to MCU.
 *
 * @authors Chase Block
 * @lastRevised 10/13/2018
 */

#ifndef __I3G4250D_H__
#define __I3G4250D_H__

#include <stdint.h>
#include "stm32f4xx.h"

// Register list to draw from
#define OUT_TEMP  0x26
#define STAUS_REG 0x27
#define OUT_X_L   0x28 // X angular rate data registers
#define OUT_X_H   0x29
#define OUT_Y_L   0x2A // etc.
#define OUT_Y_H   0x2B
#define OUT_Z_L   0x2C
#define OUT_Z_H   0x2D

// These pins are on GPIOB
#define CLOCK_PIN GPIO_Pin_13
#define CLOCK_PIN_SRC GPIO_PinSource13
#define MOSI_PIN  GPIO_Pin_14
#define MOSI_PIN_SRC  GPIO_PinSource14
#define MISO_PIN  GPIO_Pin_15
#define MISO_PIN_SRC  GPIO_PinSource15
#define CHIP_SEL  GPIO_Pin_12

/** I3G4250D_Init
 * Initializes SPI to communicate with the temperature slave boards (I3G4250D chip)
 * Initializes and configures I3G4250D chip 
 */
void I3G4250D_Init(void);

/** I3G4250D_SendCmd
 * Sends command data to I3G4250D
 * @param unsigned int 16-bit data
 */
void I3G4250D_SendCmd(uint16_t *data);

/** I3G4250D_Measure
 * Sends command to I3G4250D to gather and save all ADC values
 * @return unsigned int 16-bit measurements from all ADCs
 */
uint16_t *I3G4250D_Measure(void);

#endif
