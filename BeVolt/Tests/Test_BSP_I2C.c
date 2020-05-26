#include "common.h"
#include "config.h"
#include "BSP_I2C.h"
#include "EEPROM.h"

int main() {
	BSP_I2C_Init();
	printf("EEPROM initialized\n");
	uint8_t buffer[4] = {0xfe, 0xed, 0xbe, 0xef};
	EEPROM_WriteMultipleBytes(0x0000, 4, buffer);
	return 0;
}

