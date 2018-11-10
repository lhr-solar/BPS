/** FIFO.h
 * Software Fifo used for transmitting data and not take up CPU resources
 * @authors Chase Block
 * @lastRevised 11/10/2018
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "Terminal.h"

int _read(int file, char *data, int len){

}

int _write(int file, char *data, int len){

}

void * Terminal_HandleInput(char * input){
	char ** ops = Terminal_splitOps(input);

	if(strcmp(ops[0], "i") == 0){			// Current status
		Terminal_currentStatus();
	}else if(strcmp(ops[0], "v") == 0){		// Voltage status
		Terminal_voltageStatus();
	}else if(strcmp(ops[0], "t") == 0){		// Temperature status
		Terminal_temperatureStatus();
	}else if(strcmp(ops[0], "st") == 0){		// General status
		Terminal_currentStatus());
		Terminal_voltageStatus();
		Terminal_temperatureStatus();
		Terminal_contactorStatus();
		Terminal_gyroStatus(0);
	}else if(strcmp(ops[0], "c") == 0){		// Contactor status
		if(ops[1] != NULL){
			int stat = ops[1][0] - '0';
			Terminal_setContactor(stat);
		}
		Terminal_contactorStatus();
	}else if(strcmp(ops[0], "gyro") == 0){		// Gyro status
		if(ops[1] == NULL){
			Terminal_gyroStatus(0);
		}else{
			switch(ops[1][0]){
			case 'x':
				Terminal_gyroStatus(1);
				break;
			case 'y':
				Terminal_gyroStatus(2);
				break;
			case 'z':
				Terminal_gyroStatus(3);
			}
		}
	}else if(strcmp(ops[0], "wd") == 0){		// Watchdog status
		Terminal_watchdogStatus();
	}else if(strcmp(ops[0], "ee") == 0){		// EEPROM status
		Terminal_eepromStatus();
	}else if(strcmp(ops[0], "can") == 0){		// CAN bus status
		Terminal_canStatus();
	}else if(strcmp(ops[0], "spi") == 0){		// SPI bus status
		Terminal_spiStatus();
	}else if(strcmp(ops[0], "i2c") == 0){		// I2C status
		Terminal_i2cStatus();
	}else if(strcmp(ops[0], "h") == 0){		// Print help menu
		Terminal_helpMenu();
	}else{
		printf("\"%s\" is not recognized as a command.", ops[0]);
	}

	printf("\n\r"); // Do a newline for the next command
}

void * Terminal_currentStatus(void){
	string stat = Current_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The current level is %s\n\r", stat);
	printf("High-precision: %u\n\r", Current_HighPrecisionAmperes());
	printf("Low-precision: %u\n\r", Current_LowPrecisionAmperes());
}

void * Terminal_voltageStatus(void){
	string stat = Voltage_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The voltage level is %s\n\r", stat);
	printf("Total pack voltage: %u\n\r", Voltage_TotalPackVoltage());
	printf("Modules in danger: ");
	uint16_t * modules = Voltage_ModulesInDanger();
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		printf("%u ", modules[i]);
	}
	printf("\n\r");
}

char * Terminal_temperatureStatus(void){
	string stat = Temperature_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The temperature level is %s\n\r", stat);
	printf("Average temperature: %u\n\r", Temperature_TotalPackAvgTemperature());
	printf("Modules in danger: ");
	uint16_t * modules = Temperature_ModulesInDanger();
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		printf("%u ", modules[i]);
	}
	printf("\n\r");
}

char * Terminal_contactorStatus(void){

}

char * Terminal_setContactor(char * status){

}

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
char * Terminal_gyroStatus(uint8_t axes){
	stat = ICM20600_IsFlipped() ? "FLIPPED" : "NOT FLIPPED";
	printf("The car is currently %s", stat);
}

char * Terminal_watchdogStatus(){

}

char * Terminal_eepromStatus(){

}

char * Terminal_canStatus(){

}

char * Terminal_spiStatus(){

}

char * Terminal_i2cStatus(){

}

char ** Terminal_splitOps (char * input){
	char ** ops;
	ops[0] = strtok(input, " ");
	ops[1] = strtok(NULL, " ");
	return ops;
}

char * Terminal_helpMenu(){
	return
		"This is the help menu for the BeVolt CLI.\n"
		"The availible commands are listed below.\n"
		"-----------------------------------------\n"
		"i [n] -- access the current readings\n"
		"\n"
		"v [n] -- access the voltage readings\n"
		"\n"
		"t [n] -- access the temperature readings\n"
		"\n"
		"st -- status overview\n"
		"\n"
		"c [1,0] -- contactor information\n"
		"\n"
		"gyro -- gyro overview\n"
		"gyro x -- x-axis reading\n"
		"gyro y -- y-axis reading\n"
		"gyro z -- z-axis reading\n"
		"\n"
		"wd -- output any errors or status \n"
		"\tinformation relating to watchdog timer\n"
		"\n"
		"ee -- output status of the eeprom\n"
		"\t(conected, error, etc.)\n"
		"\n"
		"can -- list information relating to \n"
		"\tthe CAN bus\n"
		"\n"
		"spi -- list information relating to the \n"
		"\tSPI communication buses\n"
		"\n"
		"i2c -- list information relating to the \n"
		"\ti2c communication buses\n"
		"\n"
		"h -- print the help menu\n";
}
