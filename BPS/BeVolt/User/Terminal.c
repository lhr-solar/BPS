/** FIFO.h
 * Software Fifo used for transmitting data and not take up CPU resources
 * @authors Chase Block
 * @lastRevised 11/10/2018
 */
#include "Terminal.h"

void Terminal_CheckInput(){
	char * op;
	if(scanf("%s", op) > 0){
		char * arg;
		scanf("%s", arg);
		Terminal_HandleInput(op, arg);
	}
}

void Terminal_HandleInput(char * op, char * arg){

	if(strcmp(op, "i") == 0){			// Current status
		Terminal_currentStatus();
	}else if(strcmp(op, "v") == 0){		// Voltage status
		Terminal_voltageStatus();
	}else if(strcmp(op, "t") == 0){		// Temperature status
		Terminal_temperatureStatus();
	}else if(strcmp(op, "st") == 0){		// General status
		Terminal_currentStatus();
		Terminal_voltageStatus();
		Terminal_temperatureStatus();
		Terminal_contactorStatus();
		Terminal_gyroStatus(0);
	}else if(strcmp(op, "c") == 0){		// Contactor status
		if(arg != NULL){
			int stat = arg[0] - '0';
			Terminal_setContactor(stat);
		}
		Terminal_contactorStatus();
	}else if(strcmp(op, "gyro") == 0){		// Gyro status
		if(arg == NULL){
			Terminal_gyroStatus(0);
		}else{
			switch(arg[0]){
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
	}else if(strcmp(op, "wd") == 0){		// Watchdog status
		Terminal_watchdogStatus();
	}else if(strcmp(op, "ee") == 0){		// EEPROM status
		Terminal_eepromStatus();
//	}else if(strcmp(op, "can") == 0){		// CAN bus status
//		Terminal_canStatus();
//	}else if(strcmp(op, "spi") == 0){		// SPI bus status
//		Terminal_spiStatus();
//	}else if(strcmp(op, "i2c") == 0){		// I2C status
//		Terminal_i2cStatus();
	}else if(strcmp(op, "h") == 0){		// Print help menu
		Terminal_helpMenu();
	}else{
		printf("\"%s\" is not recognized as a command.", op);
	}

	printf("\n\r"); // Do a newline for the next command
}

/*
void Terminal_currentStatus(void){
	const char* stat = Current_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The current level is %s\n\r", stat);
	printf("High-precision: %u\n\r", Current_HighPrecisionAmperes());
	printf("Low-precision: %u\n\r", Current_GetLowPrecAmps());
}

void Terminal_voltageStatus(void){
	const char * stat = Voltage_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The voltage level is %s\n\r", stat);
	printf("Total pack voltage: %u\n\r", Voltage_GetTotalPackVoltage());
	printf("Modules in danger: ");
	uint16_t * modules = Voltage_ModulesInDanger();
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		printf("%u ", modules[i]);
	}
	printf("\n\r");
}

void Terminal_temperatureStatus(void){
	const char * stat = Temperature_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The temperature level is %s\n\r", stat);
	printf("Average temperature: %u\n\r", Temperature_TotalPackAvgTemperature());
	printf("Modules in danger: ");
	uint16_t * modules = Temperature_ModulesInDanger();
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		printf("%u ", modules[i]);
	}
	printf("\n\r");
}
*/
void Terminal_contactorStatus(void){

}

void Terminal_setContactor(int status){

}

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
void Terminal_gyroStatus(uint8_t axes){
	const char * stat = ICM20600_IsFlipped() ? "FLIPPED" : "NOT FLIPPED";
	printf("The car is currently %s", stat);
}

void Terminal_watchdogStatus(){

}

void Terminal_eepromStatus(){

}

void Terminal_canStatus(){
	// Not used
}

void Terminal_spiStatus(){
	// Not used
}

void Terminal_i2cStatus(){
	// Not used
}

char ** Terminal_splitOps (char * input){
	char ** ops;
	ops[0] = strtok(input, " ");
	ops[1] = strtok(NULL, " ");
	return ops;
}

void Terminal_helpMenu(){
	printf(
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
		"h -- print the help menu\n");
}
