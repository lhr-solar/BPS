#include <string.h>
#include <stdlib.h>
#include "Terminal.h"

char * Terminal_HandleInput(char * input){
	char ** ops = splitOps(input);
	
	if(strcmp(ops[0], "i") == 0){
		
	}
}

char ** splitOps (char * input){
	char * ops[2];
	ops[0] = strtok(input, " ");
	ops[1] = strtok(NULL, " ");
	return ops;
}

char * helpMenu(){
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