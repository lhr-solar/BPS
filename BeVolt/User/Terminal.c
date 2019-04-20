/** FIFO.h
 * Software Fifo used for transmitting data and not take up CPU resources
 * @authors Chase Block
 * @lastRevised 11/10/2018
 */
#include "Terminal.h"

void Terminal_HandleInput(char ** op){

	char* cmd = op[0];
	
	if(strcmp(cmd,"halt") == 0){				// Kill the car
		Terminal_Halt();
  }else if(strcmp(cmd, "i") == 0){	 	// Current status
		Terminal_CurrentStatus();
	}else if(strcmp(cmd, "v") == 0){		// Voltage status
		Terminal_VoltageStatus();
	}else if(strcmp(cmd, "t") == 0){		// Temperature status
		Terminal_TemperatureStatus(Current_IsCharging());
	}else if(strcmp(cmd, "st") == 0){		// General status
		Terminal_CurrentStatus();
		Terminal_VoltageStatus();
		Terminal_TemperatureStatus(Current_IsCharging());
		Terminal_ContactorStatus();
		Terminal_GyroStatus(0);
	}else if(strcmp(cmd, "c") == 0){		// Contactor status
		if(op[1] != NULL){
			Terminal_SetContactor(!strcmp(op[1], "on"));	// If op == "on", then strcmp returns 0, which when inverted is 1.
		}
		Terminal_ContactorStatus();
	}else if(strcmp(cmd, "gyro") == 0){		// Gyro status
		if(op[1] == NULL){
			Terminal_GyroStatus(0);
		}else{
			switch(op[1][0]){
			case 'x':
				Terminal_GyroStatus(1);
				break;
			case 'y':
				Terminal_GyroStatus(2);
				break;
			case 'z':
				Terminal_GyroStatus(3);
			}
		}
	}else if(strcmp(cmd, "wd") == 0){		// Watchdog status
		Terminal_WatchdogStatus();
	}else if(strcmp(cmd, "ee") == 0){		// EEPROM status
		Terminal_EepromStatus();
	}else if(strcmp(cmd, "h") == 0){		// Print help menu
		Terminal_HelpMenu();
	}else{
		printf("\"%s\" is not recognized as a command.", cmd);
	}

	printf("\n\r"); // Do a newline for the next command
}

void Terminal_Halt(){
	Terminal_SetContactor(0);
}

void Terminal_CurrentStatus(void){
	const char* stat = Current_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The current level is %s\n\r", stat);
	printf("High-precision: %u\n\r", Current_GetHighPrecReading());
	printf("Low-precision: %u\n\r", Current_GetLowPrecReading());
}

void Terminal_VoltageStatus(void){
	const char * stat = Voltage_IsSafe() ? "SAFE" : "NOT SAFE";
	printf("The voltage level is %s\n\r", stat);
	printf("Total pack voltage: %u\n\r", Voltage_GetTotalPackVoltage());
	printf("Modules in danger: ");
	uint16_t * modules = Voltage_GetModulesInDanger();
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		printf("%u ", modules[i]);
	}
	printf("\n\r");
}

void Terminal_TemperatureStatus(uint8_t isCharging){
	const char * stat = Temperature_IsSafe(isCharging) ? "SAFE" : "NOT SAFE";
	printf("The temperature level is %s\n\r", stat);
	printf("Average temperature: %u\n\r", Temperature_GetTotalPackAvgTemperature());
	printf("Modules in danger: ");
	uint16_t * modules = Temperature_GetModulesInDanger();
	for(int i = 0; i < sizeof(modules)/sizeof(uint16_t); ++i){
		printf("%u ", modules[i]);
	}
	printf("\n\r");
}

void Terminal_ContactorStatus(void){
	printf("The contactor is currently %s", Contactor_Flag() == 0 ? "off" : "on");
}

void Terminal_SetContactor(uint8_t status){
	if(status == 1)
		Contactor_On();
	else
		Contactor_Off();
}

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
void Terminal_GyroStatus(uint8_t axes){
	const char * stat = ICM20600_IsFlipped() ? "FLIPPED" : "NOT FLIPPED";
	printf("The car is currently %s", stat);
}

void Terminal_WatchdogStatus(){
	// WDTimer_GetStatus();
}

void Terminal_EepromStatus(){
	// EEPROM_GetStatus();
}

void Terminal_HelpMenu(){
	printf(
		"This is the help menu for the BeVolt CLI.\n"
		"The availible commands are listed below.\n"
		"-----------------------------------------\n"
		"halt -- kill everything; equivalent to\n"
	  "\tc 0\n"
	  "\n"
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
