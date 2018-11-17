#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "Terminal.h"

int _read(int file, char *data, int len){
	
}

int _write(int file, char *data, int len){
	
}

char * Terminal_HandleInput(char * input){
	char ** ops = Terminal_splitOps(input);
	
	if(strcmp(ops[0], "i") == 0){
		return Terminal_currentStatus();
	}else if(strcmp(ops[0], "v") == 0){
		return Terminal_voltageStatus();
	}else if(strcmp(ops[0], "t") == 0){
		return Terminal_temperatureStatus();
	}else if(strcmp(ops[0], "st") == 0){
		char * output;
		output = strcat(Terminal_currentStatus(), Terminal_voltageStatus());
		output = strcat(output, Terminal_temperatureStatus());
		output = strcat(output, Terminal_contactorStatus());
		output = strcat(output, Terminal_gyroStatus(0));
	}else if(strcmp(ops[0], "c") == 0){
		Terminal_setContactor(ops[1]);
		return Terminal_contactorStatus();
	}else if(strcmp(ops[0], "gyro") == 0){
		
	}else if(strcmp(ops[0], "wd") == 0){
		
	}else if(strcmp(ops[0], "ee") == 0){
		
	}else if(strcmp(ops[0], "can") == 0){
		
	}else if(strcmp(ops[0], "spi") == 0){
		
	}else if(strcmp(ops[0], "i2c") == 0){
		
	}else if(strcmp(ops[0], "h") == 0){
		return Terminal_helpMenu();
	}else{
		return strcat(ops[0], " is not recognized as a command.");
	}
}

char * Terminal_concatInt(char * str, uint16_t n){
	char * conv;
	sprintf(conv, "%d", n);
	return strcat(str, conv);
}

char * Terminal_currentStatus(){
	char * output;
//	output = strcat(
//		Current_IsSafe() ? "The current level is SAFE." : "The cureent level is NOT SAFE.",
//		"The high-precision reading is ");
	output = Terminal_concatInt(output, Current_HighPrecisionAmperes());
	output = strcat(output, " and the low-precision reading is ");
	output = Terminal_concatInt(output, Current_LowPrecisionAmperes());
	output = strcat(output, ".\n");
	
	return output;
}

char * Terminal_voltageStatus(void){
	
}

char * Terminal_temperatureStatus(void){
	
}

char * Terminal_contactorStatus(void){
	
}

char * Terminal_setContactor(char * status){
	
}

char * Terminal_generalStatus(void){
	
}

/**
 * @param the axes to read from, where 0 = all, 1 = x, 2 = y, 3 = z
 */
char * Terminal_gyroStatus(uint8_t axes){
	
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
		"c -- contactor information\n"
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
