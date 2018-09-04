#ifndef I2CDEV_H
#define I2CDEV_H

#include "I2C.h"

unsigned char I2C_WriteRTC(unsigned char address, unsigned char data);
unsigned char I2C_ReadRTC(unsigned char address);
unsigned char I2C_WriteEEPROM(unsigned long address, unsigned char data);
unsigned char I2C_ReadEEPROMRandom(unsigned long address);
unsigned char I2C_WriteRTC_EEPROM(unsigned char address, unsigned char data);
unsigned char I2C_ReadRTC_EEPROMRandom(unsigned char address);

#endif
