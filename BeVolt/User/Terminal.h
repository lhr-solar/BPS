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
#include "Contactor.h"

void Terminal_Halt();

void Terminal_CheckInput(void);

void Terminal_HandleInput(char ** op);

void Terminal_CurrentStatus(void);

void Terminal_VoltageStatus(void);

void Terminal_TemperatureStatus(uint8_t isCharging);

void Terminal_ContactorStatus(void);

void Terminal_SetContactor(uint8_t status);

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
void Terminal_GyroStatus(uint8_t axes);

void Terminal_WatchdogStatus(void);

void Terminal_EepromStatus(void);

void Terminal_HelpMenu(void);

#endif
