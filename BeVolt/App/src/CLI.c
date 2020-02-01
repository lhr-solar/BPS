/* CLI.c
 * Command Line Interface wrapper to 
 * define and route commands
 */

#include "CLI.h"
#include <ctype.h>
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#define MAX_TOKEN_SIZE 4

char* tokens[MAX_TOKEN_SIZE];
char buffer[100];
// Fifo CLI_Fifo

void CLI_Init(void) {
	UART3_Init(115200);
}

void CLI_Handler(void) {
	static char command[128] = "led ovolt on";
	CLI_Commands(command);
}

/** CLI_InputParse
 * Parses the input string
 * @return ptr to token
 */
void CLI_InputParse(char *input) {
	char *tokenized;
	char *split = strtok_r(input, " ", &tokenized);
	for(int i = 0; i < MAX_TOKEN_SIZE && split != NULL; i++) {
		for(int j = 0; j < strlen(split); j++) {
			split[j] = tolower(split[j]);
		}
		tokens[i] = split;
		split = strtok_r(NULL, " ", &tokenized);
	}
}

/** CLI_GetToken
 * @param index of desired token
 * @return token at desired index
 */
char* CLI_GetToken(uint8_t idx) {
	return tokens[idx];
}

/** CLI_Startup
 * Asks if batteries needs to be charged
 * @return true or false
 */
bool CLI_Startup(void) {
	printf("Do you need to charge the batteries? (y/n)\r\n");
	//TODO: Fill in UART input
}
/** CLI_Help
 * Displays the help menu
 */
void CLI_Help() {
	printf("-------------------Help Menu-------------------\r\n");
	printf("The available commands are: \r\n");
	printf("Voltage (v)\tCurrent (i)\tTemperature (t)\r\n");
	printf("Contactor/Switch (sw)\tCharge (q)\tError Light (l)\r\n");
	printf("CAN (c)\tEEPROM (ee)\tDisplay(d)\r\n");
	printf("LTC (ltc)\tWatchdog(w)\tADC(adc)\r\n");
	printf("Critical/Abort (!)\tAll(a)\r\n");
	printf("Keep in mind: all values are 1-indexed\r\n");
	printf("-----------------------------------------------\r\n");
}	// TODO: Put a logo into ASCII as part of the help menu

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 * @param input command
 */
void CLI_Voltage(char *input) {
	if(tokens[1] == NULL) {
		for(int i = 0; i < NUM_BATTERY_MODULES; i++){
			printf("Module number ");
			printf("%d", i+1);
			printf(": ");
			printf("%.3fV",Voltage_GetModuleMillivoltage(i)/1000.0);
			printf("\n\r");
		}
		return;
	}
	int modNum = 0;
	sscanf(tokens[2], "%d", &modNum);		// Converts the string number to int
	switch(tokens[1][0]){		
		// Specific module
		case 'm':
			if (modNum == NULL || modNum > NUM_BATTERY_MODULES || modNum < 0){
				printf("Invalid module number");
			}
			else {
				printf("Module number %d: %.3fV\n\r", modNum, (Voltage_GetModuleMillivoltage(modNum-1)/1000.0));
			}
			break;
		
		// Total
		case 't':
			printf("Total voltage: %.3fV\n\r",Voltage_GetTotalPackVoltage()/1000.0);
			break;
		
		// Safety Status
		case 's':	
			printf("Safety Status: ");
				SafetyStatus voltage = Voltage_IsSafe();
				switch(voltage) {
					case SAFE: 
						printf("SAFE\n\r");
						break;
					case DANGER: 
						printf("DANGER\n\r");
						break;
					case OVERVOLTAGE:
						printf("OVERVOLTAGE\n\r");
						break;
					case UNDERVOLTAGE: 
						printf("UNDERVOLTAGE\n\r");
						break;
					default:
						break;
				}		
				break;
		default:
			printf("Invalid voltage command");
			printf("\n\r");
	}
}

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 * @param input command
 */
void CLI_Current(char *input) {
	switch (CLI_GetToken(1)[0]) {
		case NULL : 
			printf("\n\rHigh: %4fA\n\r", Current_GetHighPrecReading()/1000.0);//prints 4 digits, number, and A
			printf("\n\rLow: %4fA\n\r", Current_GetLowPrecReading()/1000.0);
		case 'h' : 
			printf("\n\rHigh: %4fA\n\r", Current_GetHighPrecReading()/1000.0);
		case 'l' : 
			printf("\n\rLow: %4fA\n\r", Current_GetLowPrecReading()/1000.0);
		case 's' : 
			if (Current_IsSafe() == 0) {
				printf("\n\rCurrentState: SAFE\n\r");
			}
			else {
				printf("\n\rCurrentState: DANGER\n\r");
			}
		case 'c' : 
			if (Current_IsCharging() == 0) {
				printf("\n\rBatteryState: CHARGING\n\r");
			}
			else {
				printf("\n\rBatteryState: NOT CHARGING\n\r");
			}
		default: printf("\n\rInvalid Command\n\r");
		}
}


// TODO: Reconfigure the temperature CLI functions
/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 * @param input command
 */
void CLI_Temperature(char *input) {
	if(tokens[1] == NULL) {
		for(int i = 0; i < NUM_BATTERY_MODULES; i++){
			printf("Module number ");
			printf("%d", i+1);
			printf(": ");
			printf("%.3f�C",Temperature_GetModuleTemperature(i)/1000.0);
			printf("\n\r");
		}
	}
	switch(CLI_GetToken(1)[0]){			
		// All temperature sensors
		case 'a':
			// Print out temperature of all the temperatures sensors on every module 
			break;
			
		// Temperature of specific module
		char modNum = CLI_GetToken(2)[0]-1;
		char sensNum = CLI_GetToken(3)[0]-1;
		case 'm':
			if (modNum == NULL || modNum > NUM_BATTERY_MODULES || modNum < 0){
				printf("Invalid module number");
			}
			else {
				printf("Module number ");
				printf("%c", modNum+1);
				printf(": ");
				printf("%.3f�C",Temperature_GetModuleTemperature(modNum)/1000.0);
				printf("\n\r");
				// Should also print out temperature of sensor if specified
			}
			break;
			
		// Average temperature of the whole pack
		case 't':
			printf("Total average temperature: ");
			printf("%.3f�C", Temperature_GetTotalPackAvgTemperature()/1000.0); 
			printf("\n\r");
			break;
			
		default:
			printf("Invalid temperature command");
			printf("\n\r");
			break;
	}
}

/** CLI_Contactor
 * Interacts with contactor status
 * @param input command
 */
void CLI_Contactor(char *input) {
	switch(tokens[1][0]) {
		FunctionalState contactor = Contactor_Flag();
			if(contactor == ENABLE) {
				printf("Contactor is Enabled");
			} else {
				printf("Contactor is Disabled");
			break;
		default:
			printf("Invalid contactor command\r\n");
			break;
		}
	}
}
/**
 * @param input command
 */
void CLI_Charge(char *input) {
	int value;
	switch(CLI_GetToken(1)[0]) {
			printf("The battery is at: %d\n\r", SoC_GetPercent());
		case 'r' : 
			SoC_SetAccum(0);//resets accumulator
		case 's' : 
			//converts string to integer and passes value to accumulator function
			SoC_SetAccum(sscanf(CLI_GetToken(2), "%d", &value)); 
		default: 
			printf("Invalid Charge Command");
	}
}

/** CLI_LED
 * Interacts with the LEDs
 * @param input command
 */
	
void toggleLED(led input) {
	if(strcmp("1", tokens[2]) == 0 || strcmp("on", tokens[2]) == 0) {
		LED_On(input);
	}
	else if(strcmp("0", tokens[2]) == 0 || strcmp("off", tokens[2]) == 0) {
		LED_Off(input);
	} else {
		printf("Invalid LED command");
	}
}
void CLI_LED() {
	LED_Init();
	// DelayInit();
	uint8_t error = (GPIOB->ODR) & GPIO_Pin_12;
	if(tokens[1] == NULL) {
		if(error) {
			printf("Error light is On\r\n");
		} else {
			printf("Error light is Off\r\n");
		}
	}
	switch(tokens[1][0]) {
		case 't':
				for(int i = 0; i < 100000000; i++) {
					LED_Toggle(FAULT);
					DelayMs(100);
					LED_Toggle(RUN);
					DelayMs(100);
					LED_Toggle(UVOLT);
					DelayMs(100);
					LED_Toggle(OVOLT);
					DelayMs(100);
					LED_Toggle(OTEMP);
					DelayMs(100);
					LED_Toggle(OCURR);
					DelayMs(100);
					LED_Toggle(WDOG);
					DelayMs(100);
					LED_Toggle(CAN);
					DelayMs(100);
					LED_Toggle(EXTRA);
					DelayMs(100);
				}
			break;
		default:
			if(strcmp("fault", tokens[1]) == 0) {
				toggleLED(FAULT);
			}
			else if(strcmp("run", tokens[1]) == 0) {
				toggleLED(RUN);
			}
			else if(strcmp("ocurr", tokens[1]) == 0) {
				toggleLED(OCURR);
			}
			else if(strcmp("otemp", tokens[1]) == 0) {
				toggleLED(OTEMP);
			}
			else if(strcmp("ovolt", tokens[1]) == 0) {
				toggleLED(OVOLT);
			}
			else if(strcmp("wdog", tokens[1]) == 0) {
				toggleLED(WDOG);
			}
			else if(strcmp("can", tokens[1]) == 0) {
				toggleLED(CAN);
			}
			else if(strcmp("extra", tokens[1]) == 0) {
				toggleLED(EXTRA);
			}
			break;
	}
}
=======
void CLI_Charge(char *input) {}

/** CLI_ErrorLight
 */
void CLI_CAN(char *input) {
	uint8_t data[8];
	switch(CLI_GetToken(1)[0]){
		case 'r':
			CAN1_Read(data);
			printf("CAN message: %s\n\r", data);  // 1 if data was read, 0 if data wasn't read
			break;
		
		case 'w':
			CAN1_Write(CLI_GetToken(2)[0],(uint8_t*)CLI_GetToken(3));
			break;
		
		default:
			printf("Invalid CAN command\n\r");
			break;
	}
}

/** CLI_Display
 * Interacts with the display
 * @param input command
 */
void CLI_Display(char *input) {}

/** CLI_Watchdog
 * Interacts with the watchdog timer
 * @param input command
 */
void CLI_Watchdog(char *input) {
	switch(CLI_GetToken(1)[0]){
		case NULL: 
			printf("Safety Status: ");
				if (WDTimer_DidSystemReset() == 0){
					printf("SAFE");
				} else if (WDTimer_DidSystemReset() == 1){
					printf("DANGER");
				}
			printf("\n\r");
			break;
		
		default:
			printf("Invalid watchdog command\n\r");
	}
}

/** CLI_EEPROM
 * Interacts with EEPROM
 * @param input command
 */
void CLI_EEPROM(char *input) {}

/** CLI_Peripherals
 * Interacts with the peripherals
 * @param input command
 */
void CLI_Peripherals(char *input) {}

/** CLI_Critical
 * Checks and displays the desired
 * voltage parameter(s)
 * @param input command
 */
void CLI_Critical() {}
	
/** CLI_Commands
 * Routes the command given to the proper
 * measurement method to check the desired values
 */
void CLI_Commands(char *input){	
	CLI_InputParse(input);
	printf("Hello! Welcome to the BPS System! I am your worst nightmare...\r\n");
	printf("Please enter a command (Type 'h', 'm', or '?' to see a list of commands) \r\n");
	switch(input[0]) {
		// Help menu
		case 'h':
		case 'm':
		case '?':
			CLI_Help();
			break;
		
		// Voltage commands
		case 'v':
			CLI_Voltage(input);
			break;
		
		// Current commands
		case 'i':
			CLI_Current(input);
			break;
		
		// Temperature commands
		case 't':
			CLI_Temperature(input);
			break;
		
		// Contactor/Switch commands
		case 's':
			CLI_Contactor(input);
			break;
		
		// State of Charge commands
		case 'q':
		case '%':
			CLI_Charge(input);
			break;
		
		// Error light commands
		case 'l':
			CLI_LED();
			break;
		
		// CAN commands
		case 'c':
			CLI_CAN(input);
			break;
		
		// Display commands
		case 'd':
			CLI_Display(input);
			break;
		
		// Watchdog commands
		case 'w':
			CLI_Watchdog(input);
			break;
		
		// EEPROM commands
		case 'e':
			CLI_EEPROM(input);
			break;
		
		// Peripheral commands
		case 'p':
			CLI_Peripherals(input);
			break;
		
		// Emergency Abort
		case '!':
			CLI_Critical();
			break;		// ABORT
		
		default:
			printf("Invalid command. Type 'h' or 'm' or '?' for the help menu");
			break;
	}
	//char* measurement;
  //char* function;
  //char* parameter;
	//parameter = NULL;
	//uint8_t word = 0;
/*
	// Parsing the input string and tokenizing into the variables
	while(split != NULL) {
    if (word == 0){
      measurement = split;
    }
		else if (word == 1){
      function = split;
    }
		else if (word == 2){
      parameter = split;
    }
    split = strtok_r(NULL, " ", &saveptr);
    word++;
	}
	// Voltage commands
	if(strcmp(measurement, "voltage") == 0){
		if(strcmp(function, "check") == 0){
			if(parameter == NULL){			// All modules
				for(int i = 0; i<12; i++){
					printf("\n\r%d", i);
					printf("th module: ");
					printf("%d",Voltage_GetModuleMillivoltage(i));
				}
				printf("\n\r");
			}else{			// Specific module
				printf("\n\r%c", *parameter);
				printf("th module: ");
				printf("%d",Voltage_GetModuleMillivoltage(*parameter));
				printf("\n\r");
			}
		}
		else if (strcmp(function,"safe") == 0){		// Check SafetyStatus
			printf("\n\r");
			SafetyStatus voltage = Voltage_IsSafe();
			switch(voltage) {
				case SAFE: 
					printf("SAFE");
					break;
				case DANGER: 
					printf("DANGER");
					break;
				case OVERVOLTAGE:
					printf("OVERVOLTAGE");
					break;
				case UNDERVOLTAGE: 
					printf("UNDERVOLTAGE");
					break;
				default:
					break;
			}
			printf("\n\r");
		}
		else if (strcmp(function,"total") == 0) {			// Total pack voltage
			printf("\n\rTotal voltage: ");
			printf("%d",Voltage_GetTotalPackVoltage());
			printf("\n\r");
		}					
  }
	// Current commands
	else if (strcmp(measurement,"current") == 0){
		if (strcmp(function,"safe") == 0) {		// Check SafetyStatus
			printf("\n\r");
			SafetyStatus current = Current_IsSafe();
			switch(current){
				case SAFE:
					printf("SAFE");
					break;
				case DANGER: 
					printf("DANGER");
					break;
				default:
					break;
			}
			printf("\n\r");			
		}
		else if (strcmp(function,"charging") == 0){
			printf("\n\r");
			uint8_t charge = Current_IsCharging();
			switch(charge){
				case 0: 
					printf("DISCHARGING");
					break;
				case 1:
					printf("CHARGING");
					break;
				default:
					break;
			}
			printf("\n\r");			
		}
		else if (strcmp(function,"high") == 0){			// High precision current reading
			printf("\n\r");		
			printf("%d",Current_GetHighPrecReading());
			printf("\n\r");			
		}
		else if (strcmp(function,"low") == 0){		// Low precision current reading
			printf("\n\r");	
			printf("%d",Current_GetLowPrecReading());
			printf("\n\r");			
		}
  }
	// Temperature commands
	else if (strcmp(measurement,"temp") == 0){
		if (strcmp(function,"average") == 0){		// Average pack temperature
			printf("\n\r");		
			printf("%d",Temperature_GetTotalPackAvgTemperature());
			printf("\n\r");			
		}
		else if (strcmp(function,"charge") == 0){		// Set charge state
			if(parameter != NULL){
				Temperature_SetChargeState(*parameter);
				printf("%d",*parameter);	
				printf("\n\r");
			}else{
				printf("Please provide a charge state (1 for charging, 0 for discharging)");
			}
		}
		else if (strcmp(function,"check") == 0){		// Get module temperatures
			if (parameter == NULL){			
				for (int i = 0; i<12; i++){
					printf("%d",Temperature_GetModuleTemperature(i));
					printf("\n\r");
					}
				}else { 
					printf("%d",Temperature_GetModuleTemperature(*parameter));
					printf("\n\r");		
				}
		}
	}else{
		printf("Invalid measurement. Enter a valid measurement and function");
	}
	*/
}
