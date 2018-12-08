/** FIFO.h
 * Software Fifo used for transmitting data and not take up CPU resources
 * @authors Chase Block
 * @lastRevised 11/10/2018
 */
 
#ifndef TERMINAL_H__
#define TERMINAL_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Gyro.h"

void Terminal_CheckInput(void);

void Terminal_HandleInput(char * op, char * arg);

char ** Terminal_splitOps (char * input);

void Terminal_currentStatus(void);

void Terminal_voltageStatus(void);

void Terminal_temperatureStatus(void);

void Terminal_contactorStatus(void);

void Terminal_setContactor(int status);

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
void Terminal_gyroStatus(uint8_t axes);

void Terminal_watchdogStatus(void);

void Terminal_eepromStatus(void);

void Terminal_canStatus(void);

void Terminal_spiStatus(void);

void Terminal_i2cStatus(void);

void Terminal_helpMenu(void);

#endif
