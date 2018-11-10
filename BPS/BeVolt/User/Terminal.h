/** FIFO.h
 * Software Fifo used for transmitting data and not take up CPU resources
 * @authors Chase Block
 * @lastRevised 11/10/2018
 */
#include <stdint.h>
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"

void * Terminal_HandleInput(char * input);

char ** Terminal_splitOps (char * input);

void * Terminal_currentStatus(void);

void * Terminal_voltageStatus(void);

void * Terminal_temperatureStatus(void);

void * Terminal_contactorStatus(void);

void * Terminal_setContactor(char * status);

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
void * Terminal_gyroStatus(uint8_t axes);

void * Terminal_watchdogStatus();

void * Terminal_eepromStatus();

void * Terminal_canStatus();

void * Terminal_spiStatus();

void * Terminal_i2cStatus();

void * Terminal_helpMenu();
