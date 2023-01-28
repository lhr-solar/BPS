/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_I2C.h"
#include "EEPROM.h"

#define EEPROM_ADDRESS (0xA0)
#define ADDRESS 0x0005

int main() {
	BSP_I2C_Init();
	printf("EEPROM initialized\n");
	uint8_t buffer[4] = {0xfe, 0xed, 0xbe, 0xef};
	BSP_I2C_Write(EEPROM_ADDRESS, ADDRESS, buffer, 4);
	uint8_t readBuffer[4];
	BSP_I2C_Read(EEPROM_ADDRESS, ADDRESS, readBuffer, 4);
	printf("read EEPROM\n");
	for (int i = 0; i < 4; i++){
		printf("%x\n", readBuffer[i]);
	}
	printf("--------------------\n");
	while(1){}
	return 0;
}

