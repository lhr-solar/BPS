

#include <stdint.h>

char * Terminal_HandleInput(char * input);

char ** splitOps (char * input);

char * currentStatus(void);

char * voltageStatus(void);

char * temperatureStatus(void);

char * contactorStatus(void);

char * setContactor(uint16_t status);

char * generalStatus(void);

char * gyroStatus(uint8_t axes);

char * watchdogStatus();

char * eepromStatus();

char * canStatus();

char * spiStatus();

char * i2cStatus();

char * helpMenu();