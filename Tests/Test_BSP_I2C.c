/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "common.h"
#include "config.h"
#include "BSP_I2C.h"
#include "EEPROM.h"

#define ADDRESS 0x0005

int main() {
	BSP_I2C_Init();
	printf("EEPROM initialized\n");
	uint8_t buffer[4] = {0xfe, 0xed, 0xbe, 0xef};
	EEPROM_WriteMultipleBytes(ADDRESS, 4, buffer);
	uint8_t readBuffer[4];
	BSP_I2C_Read(EEPROM_ADDRESS, ADDRESS, readBuffer, 4);
	printf("read EEPROM\n");
	for (int i = 0; i < 4; i++){
		printf("%x\n", readBuffer[i]);
	}
	printf("--------------------\n");
	EEPROM_Reset();
	EEPROM_Load();
	EEPROM_Tester();
	EEPROM_SerialPrintData();
	return 0;
}

