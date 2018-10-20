#include <stdint.h>
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"

char * Terminal_HandleInput(char * input);

char ** Terminal_splitOps (char * input);

char * Terminal_currentStatus(void);

char * Terminal_voltageStatus(void);

char * Terminal_temperatureStatus(void);

char * Terminal_contactorStatus(void);

char * Terminal_setContactor(char * status);

char * Terminal_generalStatus(void);

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
char * Terminal_gyroStatus(uint8_t axes);

char * Terminal_watchdogStatus();

char * Terminal_eepromStatus();

char * Terminal_canStatus();

char * Terminal_spiStatus();

char * Terminal_i2cStatus();

char * Terminal_helpMenu();

char * Terminal_concatInt(char * str, uint16_t n);