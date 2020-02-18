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

static cell_asic* Minions;
int32_t hashTokens[MAX_TOKEN_SIZE];

/** CLI_Init
 * Initializes the CLI with the values it needs
 * @param boards is a cell_asic struct pointer to the minion boards
 */
void CLI_Init(cell_asic* boards) {
	Minions = boards;
	UART3_Init();
}

/** CLI_InputParse
 * Parses the input string and stores in hashTokens[]
 * @param input is a pointer to the input string
 */
void CLI_InputParse(char *input) {
	char *tokenized;
	char *split = strtok_r(input, " ", &tokenized);
	for(int i = 0; i < MAX_TOKEN_SIZE && split != NULL; i++) {
		for(int j = 0; j < strlen(split); j++) {
			split[j] = tolower(split[j]);
		}
		if(!isalpha(split[0])) {
			sscanf(split, "%d", &hashTokens[i]);
		} else {
			hashTokens[i] = CLI_StringHash(split);
		}
		split = strtok_r(NULL, " ", &tokenized);
	}
}

uint32_t CLI_StringHash(char* string) {
	uint32_t hash = 0;
	uint32_t m = 1000000009;
	uint8_t p = 31;
	for(uint8_t i = 0; string[i]; i++) {
		hash += string[i] * pow(p, i);
	}
	return hash % m;
}

/** CLI_Startup
 * Prints the welcome messages and
 * asks for the override command
 * @return true or false
 */
void CLI_Startup(void) {
	printf("\n\r\n\r%s\n\r\n\r", utsvt_logo);
	printf("Hello! Welcome to the BPS System! I am your worst nightmare...\n\r");
	printf("Please enter a command (Type 'h', 'm', or '?' to see a list of commands)\n\r");
	printf(">> ");
}

/** CLI_Help
 * Displays the help menu
 */
void CLI_Help(void) {
	printf("-------------------------Help Menu-------------------------\n\r");
	printf("The available commands are: \n\r");
	printf("Voltage\t\t\tCurrent\t\t\tTemperature\n\r");
	printf("Contactor/Switch\tCharge\t\t\tLights/LED\n\r");
	printf("CAN\t\t\tEEPROM\t\t\tDisplay\n\r");
	printf("LTC/Register\t\tWatchdog\t\tADC\n\r");
	printf("Critical/Abort\t\tAll\n\r");
	printf("Keep in mind: all values are 1-indexed\n\r");
	printf("-----------------------------------------------------------\n\r");
}

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 */
void CLI_Voltage(void) {
	Voltage_UpdateMeasurements();
	if(hashTokens[1] == NULL) {
		for(int i = 0; i < NUM_BATTERY_MODULES; i++){
			printf("Module number %d: %.3fV\n\r", i+1, Voltage_GetModuleMillivoltage(i)/1000.0);
		}
		return;
	}
	SafetyStatus voltage = Voltage_CheckStatus();
	switch(hashTokens[1]){		
		// Specific module
		case MODULE:
			if (hashTokens[2] == NULL || hashTokens[2] > NUM_BATTERY_MODULES || hashTokens[2] < 0){
				printf("Invalid module number");
			}
			else {
				printf("Module number %d: %.3fV\n\r", hashTokens[2], Voltage_GetModuleMillivoltage(hashTokens[2]-1)/1000.0);
			}
			break;
		// Total
		case TOTAL:
			printf("Total voltage: %.3fV\n\r", Voltage_GetTotalPackVoltage()/1000.0);
			break;
		// Safety Status
		case SAFE_HASH:
		case SAFETY:	
			printf("Safety Status: ");
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
			printf("Invalid voltage command\n\r");
			break;
	}
}

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 */
void CLI_Current(void) {
	if(hashTokens[1] == NULL) {
		printf("High: %.3fA\n\r", Current_GetHighPrecReading()/1000.0);	// Prints 4 digits, number, and A
		printf("Low: %.3fA\n\r", Current_GetLowPrecReading()/1000.0);
		return;
	}
	switch (hashTokens[1]) {
		case HIGH: // High precision reading
			printf("High: %.3fA\n\r", Current_GetHighPrecReading()/1000.0);
			break;
		case LOW: // Low precision reading
			printf("Low: %.3fA\n\r", Current_GetLowPrecReading()/1000.0);
			break;
		case SAFE_HASH: 
		case SAFETY:
			if (Current_CheckStatus() == SAFE) {
				printf("Safety Status: SAFE\n\r");
			}
			else {
				printf("Safety Status: DANGER\n\r");
			}
			break;
		case CHARGE: // Whether battery is charging
		case CHARGING:
			if (Current_IsCharging() == 0) {
				printf("Charging State: CHARGING\n\r");
			}
			else {
				printf("Charging State: DISCHARGING\n\r");
			}
			break;
		default:
			printf("Invalid Current Command\n\r");
			break;
		}
}

/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 */
void CLI_Temperature(void) {
	if(hashTokens[1] == NULL) {
		for(int i = 0; i < NUM_BATTERY_MODULES; i++) {
			printf("Module number %d: %.3f C\n\r", i+1, Temperature_GetModuleTemperature(i)/1000.0);
		}
		return;
	}
	// Account for 1-indexing
	hashTokens[2]--;
	hashTokens[3]--;
	switch(hashTokens[1]){			
		// All temperature sensors
		case ALL:
			for(int i = 0; i < NUM_MINIONS; i++) {		// last minion only has 14 sensors
				for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
					if(i == 3 && j > 13) {
						break;
					}
					printf("Sensor number %d: %.3f C\n\r",(i*MAX_TEMP_SENSORS_PER_MINION_BOARD)+j+1, Temperature_GetSingleTempSensor(i, j)/1000.0);
				}
			}
			break;
		// Temperature of specific module
		case MODULE:
			if (hashTokens[2] == NULL || hashTokens[2] > NUM_BATTERY_MODULES || hashTokens[2] < 0){
				printf("Invalid module number");
			}
			else {
				if(hashTokens[3] == NULL) {
					printf("Module number %d: %.3f C\n\r", hashTokens[2]+1, Temperature_GetModuleTemperature(hashTokens[2])/1000.0);
				} else if(hashTokens[3] != NULL && (hashTokens[3] == 0 || hashTokens[3] == 1)) {
					uint16_t boardNum = hashTokens[2]/MAX_VOLT_SENSORS_PER_MINION_BOARD;
					printf("Sensor %d on module %d: %.3f C\n\r", hashTokens[3]+1, hashTokens[2]+1, 
							Temperature_GetSingleTempSensor(boardNum, hashTokens[3])/1000.0);
				} else {
					printf("Invalid sensor number\n\r");
				}
			}
			break;
		// Average temperature of the whole pack
		case TOTAL:
			printf("Total average temperature: %.3f C\n\r", Temperature_GetTotalPackAvgTemperature()/1000.0);
			break;
		default:
			printf("Invalid temperature command\n\r");
			break;
	}
}

/** CLI_LTC6811
 * Interacts with LTC6811 registers
 */
void CLI_LTC6811(void) {
	for(uint8_t current_ic = 0; current_ic < NUM_MINIONS; current_ic++) {
		printf("Minion board %d: ", current_ic);
		printf("Config: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].config.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].config.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].config.rx_pec_match);
		printf("Config B: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].configb.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].configb.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].configb.rx_pec_match);
		printf("COM: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].com.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].com.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].com.rx_pec_match);
		printf("S Control: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].sctrl.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].sctrl.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].sctrl.rx_pec_match);
		printf("S Control B: \n\rTX: ");
		for(uint8_t i = 0; i < 6; i++) {
			printf("%d", Minions[current_ic].sctrlb.tx_data[i]);
		}
		printf("\n\rRX: ");
		for(uint8_t i = 0; i < 8; i++) {
			printf("%d", Minions[current_ic].sctrlb.rx_data[i]);
		}
		printf("\n\rPEC: %d\n\r", Minions[current_ic].sctrlb.rx_pec_match);
	}
}

/** CLI_Contactor
 * Interacts with contactor status by
 * printing the status of the contactor
 */
void CLI_Contactor(void) {
	FunctionalState contactor = Contactor_Flag();
	switch(hashTokens[1]) {
		case STATE:
			if(contactor == ENABLE) {
				printf("Contactor is Enabled\n\r");
			} else {
				printf("Contactor is Disabled\n\r");
			break;
		default:
			printf("Invalid contactor command\n\r");
			break;
		}
	}
}

/** CLI_Charge
 * Checks and displays the desired
 * state of charge parameter(s)
 */
void CLI_Charge(void) {
	if(hashTokens[1] == NULL) {
		printf("The battery percentage is %.2f%%\n\r", SoC_GetPercent()/100.0);
		return;
	}
	switch(hashTokens[1]) {
		case RESET:
			SoC_SetAccum(0);	//resets accumulator
			printf("Accumulator has been reset\n\r");
			break;
		case SET:
			SoC_SetAccum(hashTokens[2]);
			printf("Accumulator has been set to %d%%\n\r", hashTokens[2]);
			break;
		default: 
			printf("Invalid Charge Command\n\r");
			break;
	}
}

/** toggleLED
 * Helper function for CLI_LED
 * that toggles a given led
 * @param led is the led to toggle
 */
void toggleLED(led input) {
	if(hashTokens[2] == 1 || hashTokens[2] == ON) {
		LED_On(input);
	}
	else if(hashTokens[2] == 0 || hashTokens[2] == OFF) {
		LED_Off(input);
	} else {
		printf("Invalid LED command\n\r");
	}
}

/** CLI_LED
 * Interacts with the LEDs by 
 * checking error light status
 * running a full LED test
 * and turning a specific LED on/off
 */
void CLI_LED(void) {
	LED_Init();
	uint8_t error = (GPIOB->ODR) & GPIO_Pin_12;
	if(hashTokens[1] == NULL) {
		if(error) {
			printf("Error light is On\n\r");
		} else {
			printf("Error light is Off\n\r");
		}
	}
	switch(hashTokens[1]) {
		case TEST:
				printf("\n\r");
				for(int i = 0; i < 10; i++) {
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
			if(hashTokens[1] == FAULT_HASH) {
				toggleLED(FAULT);
			}
			else if(hashTokens[1] == RUN_HASH) {
				toggleLED(RUN);
			}
			else if(hashTokens[1] == OCURR_HASH) {
				toggleLED(OCURR);
			}
			else if(hashTokens[1] == OTEMP_HASH) {
				toggleLED(OTEMP);
			}
			else if(hashTokens[1] == OVOLT_HASH) {
				toggleLED(OVOLT);
			}
			else if(hashTokens[1] == WDOG_HASH) {
				toggleLED(WDOG);
			}
			else if(hashTokens[1] == CAN_HASH) {
				toggleLED(CAN);
			}
			else if(hashTokens[1] == EXTRA_HASH) {
				toggleLED(EXTRA);
			} else {
				printf("Invalid LED command\n\r");
			}
			break;
	}
}

// TODO: Confirm status of CAN;
//		 if functional, display CAN information (specifics found on Drive)
/** CLI_CAN
 * Interacts with CAN
 */
void CLI_CAN(void) {}

// TODO: Confirm status of Display;
//		 if operational, display proper information (specifics found on Drive)
/** CLI_Display
 * Interacts with the display
 */
void CLI_Display(void) {}

/** CLI_Watchdog
 * Shows whether watchdog was tripped
 */
void CLI_Watchdog(void) {
	if(hashTokens[1] == NULL) {
		printf("Safety Status: ");
		if (WDTimer_DidSystemReset() == SAFE){
			printf("SAFE\n\r");
		} else if (WDTimer_DidSystemReset() == DANGER){
			printf("DANGER\n\r");
		}
	}
	uint8_t errorAddrArray[2];
	EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC, 2, errorAddrArray);
	uint16_t errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
	switch(hashTokens[1]) {
		case ERROR:
			printf("Most recent Watchdog error: %d", EEPROM_ReadByte(errorAddr-1));
			break;
		default:
			printf("Invalid Watchdog command");
			break;
	}
}

/** CLI_EEPROM
 * Interacts with EEPROM
 * by reading and writing to the EEPROM
 */
void CLI_EEPROM(void) {
	if(hashTokens[1] == NULL) {
		EEPROM_SerialPrintData();
		return;
	}
	uint8_t errorAddrArray[2];
	uint16_t errorAddr = 0;
	switch(hashTokens[1]) {
		case RESET:
			EEPROM_Reset();
			printf("EEPROM has been reset");
			break;
		case ERROR:
			switch(hashTokens[2]) {
				case FAULT:
					EEPROM_ReadMultipleBytes(EEPROM_FAULT_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Fault error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case TEMPERATURE:
					EEPROM_ReadMultipleBytes(EEPROM_TEMP_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Temperature error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case VOLTAGE:
					EEPROM_ReadMultipleBytes(EEPROM_VOLT_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Voltage error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CURRENT:
					EEPROM_ReadMultipleBytes(EEPROM_CURRENT_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Current error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case WATCHDOG:
					EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("Watchdog error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CAN:
					EEPROM_ReadMultipleBytes(EEPROM_CAN_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("CAN error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				case CHARGE:
					EEPROM_ReadMultipleBytes(EEPROM_SOC_PTR_LOC, 2, errorAddrArray);
					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
					printf("SoC error: %d", EEPROM_ReadByte(errorAddr-1));
					break;
				default:
					break;
			}
			break;
		default:
			printf("Invalid EEPROM command\n\r");
			break;
	}
}

/** CLI_ADC
 * Prints the high precision and
 * low precision readings
 */
void CLI_ADC(void) {
	printf("High precision ADC: %d\n\r", ADC_ReadHigh());
	printf("Low precision ADC: %d\n\r", ADC_ReadLow());
}


/** CLI_Critical
 * Shuts off contactor manually
 */  
void CLI_Critical(void) {
	Fifo criticalFifo;
	char response[fifo_size];
	printf("Please type 'shutdown' to turn the contactor off");
	while(1) {
		UART3_CheckAndEcho(&criticalFifo);
		if(UART3_HasCommand(&criticalFifo)) {
			UART3_GetCommand(&criticalFifo, response);
			if(CLI_StringHash(response) == SHUTDOWN) {
				Contactor_Off();
				printf("Contactor is off\n\r");
				break;
			} else {
				printf("Contactor is still on");
				break;
			}
		}
	}
}

/** CLI_All
 * Displays all information about BPS modules
 * (voltage, current, temperature, charge, contactor)
 */
void CLI_All(void){
	printf("Voltage: \n\r");
	hashTokens[0] = VOLTAGE;
	hashTokens[1] = 0;
	CLI_Voltage();
	printf("Current: \n\r");
	hashTokens[0] = CURRENT;
	CLI_Current();
	printf("Temperature: \n\r");
	hashTokens[0] = TEMPERATURE;
	CLI_Temperature();
	printf("State of Charge: \n\r");
	hashTokens[0] = CHARGE;
	CLI_Charge();
	printf("Contactor: \n\r");
	hashTokens[0] = CONTACTOR;
	hashTokens[1] = STATE;
	CLI_Contactor();
}

/** CLI_Handler
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Handler(char *input) {
	CLI_InputParse(input);
	if(hashTokens[0] == PARTYTIME) {
		printf("\n\r");
		for(int i = 0; i < 100; i++) {
			printf("%s", party_parrot_frames[i%10]);
			for(int j = 0; j < 18 && i < 99; j++) {
					printf("\033[A\r");
			}
		}
		printf("\n\r");
	return;
	}
	if(hashTokens[0] == PING) {
		printf("pong\n\r");
	}
	switch(hashTokens[0]) {
		// Help menu
		case MENU:
		case HELP:
			CLI_Help();
			break;
		// Voltage commands
		case VOLTAGE:
			CLI_Voltage();
			break;
		// Current commands
		case CURRENT:
			CLI_Current();
			break;
		// Temperature commands
		case TEMPERATURE:
			CLI_Temperature();
			break;
		// LTC6811 register commands
		case REGISTER:
		case LTC:
			CLI_LTC6811();
			break;
		// Contactor/Switch commands
		case SWITCH:
		case CONTACTOR:
			CLI_Contactor();
			break;
		// State of Charge commands
		case CHARGE:
			CLI_Charge();
			break;
		// Error light commands
		case LED:
		case LIGHTS:
			CLI_LED();
			break;
		// CAN commands
		case CAN_HASH:
		case CANBUS:
			CLI_CAN();
			break;
		// Display commands
		case DISPLAY:
			CLI_Display();
			break;
		// Watchdog commands
		case WATCHDOG:
			CLI_Watchdog();
			break;
		// EEPROM commands
		case EEPROM:
			CLI_EEPROM();
			break;
		// Peripheral commands
		case ADC_HASH:
			CLI_ADC();
			break;
		// Emergency Abort
		case CRITICAL:
		case ABORT:
			CLI_Critical();
			break;		// ABORT
		// All
		case ALL:
			break;
		default:
			printf("Invalid command. Type 'h' or 'm' or '?' for the help menu\n\r");
			break;
	}
	printf(">> ");
}
