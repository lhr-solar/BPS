/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/* CLI.c
 * Command Line Interface wrapper to 
 * define and route commands
 */

 #include "CLI.h"
 #include "Voltage.h"
 #include "Amps.h"
 #include "Temperature.h"
 #include "Contactor.h"
 #include "BSP_WDTimer.h"
 #include "BSP_Lights.h"
 #include "config.h"
 #include "Charge.h"
 #include "CANbus.h"
 #include "BSP_CAN.h"
 #include "BSP_UART.h"
 #include "Images.h"
 #include "BSP_ADC.h"
 #include "EEPROM.h"
 #include "Tasks.h"
 #include <string.h>

#define MAX_TOKEN_SIZE 4

OS_SEM UartRx_Sem4;

//static cell_asic* Minions;
char hexString[8];
const float MILLI_UNIT_CONVERSION = 1000;
const float PERCENT_CONVERSION = 100;

///** CLI_Init
// * Initializes the CLI with the values it needs
// * @param boards is a cell_asic struct pointer to the minion boards
// */
//void CLI_Init(cell_asic* boards) {
////	Minions = boards;
////	BSP_UART_Init(NULL, NULL, UART_USB);
//}

///** CLI_InputParse
// * Parses the input string and stores in cmd[]
// * @param input is a pointer to the input string
// * @param parsedTokens is an array to pass the parsed words back
// */
//void CLI_InputParse(char* input, char* parsedTokens) {
//    static char inputCpy[COMMAND_SIZE];
//    strcpy(inputCpy, input);
//
//	char *tokenized;
//	char *split = strtok_r(inputCpy, " ", &tokenized);
//	for(int i = 0; i < MAX_TOKEN_SIZE && split != NULL; i++) {
//		for(int j = 0; j < strlen(split); j++) {
//			split[j] = tolower(split[j]);
//		}
//		if(i == 3) { // CAN requires argument #3 to be in hex
//			strcpy(hexString, split);
//		}
//		if(!isalpha((unsigned char)split[0])) {
//			sscanf(split, "%d", &parsedTokens[i]);
//		} else {
//			parsedTokens[i] = CLI_StringHash(split);
//		}
//		split = strtok_r(NULL, " ", &tokenized);
//	}
//}

// TODO: Create our own pow() function to exponentiate
uint32_t CLI_Pow(uint32_t base, uint8_t power) {
	uint32_t ans = 1;
	if(!base && !power) {
		return 0;
	}
	for(int i = 0; i < power; i++) {
		ans *= base;
	}
	return ans;
}

/** CLI_StringHash
 * Calculates the hashed value of the input
 * @param string is the string to be hashed
 * @return the integer hash value
 */ 
uint32_t CLI_StringHash(char* string) {
	uint32_t hash = 0;
	uint32_t m = 1000000009;
	uint8_t p = 31;
	for(uint8_t i = 0; string[i]; i++) {
		hash += string[i] * CLI_Pow(p, i);
	}
	return hash % m;
}

void CLI_UartRxCallback(void) {
    RTOS_BPS_SemPost(&UartRx_Sem4, OS_OPT_POST_1);
}

/** CLI_Init
 * Prints the welcome messages
 */
void CLI_Init(void) {
    RTOS_BPS_SemCreate(&UartRx_Sem4, "CLI UART Rx Semaphore", 0);
    BSP_UART_Init(CLI_UartRxCallback, NULL, UART_USB);	
//    printf("From Leader: CLI Initialized\n");
}
 
// /** CLI_Help
//  * Displays the help menu
//  */
// void CLI_Help(void) {
// 	printf("-------------------------Help Menu-------------------------\n\r");
// 	printf("The available commands are: \n\r");
// 	printf("Voltage\t\t\tCurrent\t\t\tTemperature\n\r");
// 	printf("Contactor/Switch\tCharge\t\t\tLights/LED\n\r");
// 	printf("CAN\t\t\tEEPROM\t\t\tDisplay\n\r");
// 	printf("LTC/Register\t\tWatchdog\t\tADC\n\r");
// 	printf("Critical/Abort\t\topenwire\t\tAll\n\r");
// 	printf("Keep in mind: all values are 1-indexed\n\r");
// 	printf("-----------------------------------------------------------\n\r");
// }

/** CLI_Voltage
 * Checks and displays the desired
 * voltage parameter(s)
 * @param cmd is the array of hashed tokens
 */
void CLI_Voltage(char* cmd) {
    uint8_t cmd_arg = (uint8_t)(cmd[1] - CLI_CHAR_BASE);

    // list voltage of each modules
	if (cmd_arg == 0) {
		for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
			printf("Module number %d: %.3fV\n\r", i+1, Voltage_GetModuleMillivoltage(i)/MILLI_UNIT_CONVERSION);
		}
		return;
    // return voltage of specified module
	} else if (cmd_arg >= 1  && cmd_arg <= NUM_BATTERY_MODULES) {
	    printf("Module number %d: %.3fV\n\r", cmd_arg, Voltage_GetModuleMillivoltage(cmd_arg-1)/MILLI_UNIT_CONVERSION);
        return;
    // return total voltage
    } else if (cmd_arg == NUM_BATTERY_MODULES+1) {
		printf("Total voltage: %.3fV\n\r", Voltage_GetTotalPackVoltage()/MILLI_UNIT_CONVERSION);
        return;
    // return battery safety state
    } else if (cmd_arg == NUM_BATTERY_MODULES+2) {
	    SafetyStatus voltage = Voltage_CheckStatus();

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
				printf("UNKNOWN\n\r");
				break;
		}		
        return;
    } else {
        printf("\n\r"); 
    }
}

/** CLI_Current
 * Checks and displays the desired
 * current parameter(s)
 * @param cmd is the array of hashed tokens
 */
void CLI_Current(char* cmd) {
    uint8_t cmd_arg = (uint8_t)(cmd[1] - CLI_CHAR_BASE);

	if(cmd_arg == 0) {
		printf("Current: %.3fA\n\r", Amps_GetReading()/MILLI_UNIT_CONVERSION);	// Prints 4 digits, number, and A
		return;
	} else if (cmd_arg == 1) {
		if (Amps_IsCharging()) {
			printf("Charging State: CHARGING\n\r");
		} else {
			printf("Charging State: DISCHARGING\n\r");
		}
        return;
	} else if (cmd_arg == 2) {
		if (Amps_CheckStatus(false) == SAFE) {
			printf("Safety Status: SAFE\n\r");
		}
		else {
			printf("Safety Status: DANGER\n\r");
		}
        return;
    } else {
        printf("\n\r"); 
    }
}

/** CLI_Temperature
 * Checks and displays the desired
 * temperature parameter(s)
 * @param cmd is the array of hashed tokens
 */
void CLI_Temperature(char* cmd) {
    uint8_t cmd_arg = (uint8_t)(cmd[1] - CLI_CHAR_BASE);

    // list temp of each modules
	if (cmd_arg == 0) {
		for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
			printf("Module number %d: %.3f C\n\r", i+1, Temperature_GetModuleTemperature(i)/MILLI_UNIT_CONVERSION);
		}
		return;
    // return temp of specified module
	} else if (cmd_arg >= 1  && cmd_arg <= NUM_BATTERY_MODULES) {
        printf("Module number %d: %.3f C\n\r", cmd_arg, Temperature_GetModuleTemperature(cmd[2]-1)/MILLI_UNIT_CONVERSION);
        return;
    // return average temp of battery pack
    } else if (cmd_arg == NUM_BATTERY_MODULES+1) {
		printf("Total average temperature: %.3f C\n\r", Temperature_GetTotalPackAvgTemperature()/MILLI_UNIT_CONVERSION);
        return;
    // list temp of each sensor
    } else if (cmd_arg == NUM_BATTERY_MODULES+2) {
        for(int i = 0; i < NUM_MINIONS; i++) {		
            for(int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
                // last minion only has 14 sensors; -1 for 0-index
                if(i == NUM_MINIONS-1 && j > NUM_TEMP_SENSORS_LAST_MINION-1) {	
                    break;
                }
                printf("Sensor number %d: %.3f C\n\r", (i*MAX_TEMP_SENSORS_PER_MINION_BOARD)+j+1, Temperature_GetSingleTempSensor(i, j)/MILLI_UNIT_CONVERSION);
            }
        }
    } else {
        printf("\n\r"); 
    }

// // Temperature of specific module
// case CLI_MODULE:
// 	if (cmd[2] == 0 || cmd[2]-1 > NUM_BATTERY_MODULES || cmd[2]-1 < 0){
// 		printf("Invalid module number\n\r");
// 	}
// 	else {
// 		if(cmd[3] == 0) {//temperature of module
// 			printf("Module number %d: %.3f C\n\r", cmd[2], Temperature_GetModuleTemperature(cmd[2]-1)/MILLI_UNIT_CONVERSION);
// 		} else if(cmd[3]-1 == 0 || cmd[3]-1 == 1) {//temperature of specific sensor in module
// 			uint16_t boardNum = (cmd[2]-1)/MAX_VOLT_SENSORS_PER_MINION_BOARD;
// 			uint16_t sensorNum;
// 			if (cmd[3]-1) {
// 				sensorNum = ((cmd[2]-1)%MAX_VOLT_SENSORS_PER_MINION_BOARD)+MAX_VOLT_SENSORS_PER_MINION_BOARD;
// 			} else {
// 				sensorNum = (cmd[2]-1)%MAX_VOLT_SENSORS_PER_MINION_BOARD;
// 			}
// 			printf("Sensor %d on module %d: %.3f C\n\r", cmd[3], cmd[2], 
// 					Temperature_GetSingleTempSensor(boardNum, sensorNum)/MILLI_UNIT_CONVERSION);
// 		} else {
// 			printf("Invalid sensor number\n\r");
// 		}
// 	}
// 	break;
}

// /** CLI_LTC6811
//  * Prints register information
//  * All registers are of the same type
//  * Prints each register's respective three registers
//  * (tx_data, rx_date, and rx_pec_match)
//  */
// void CLI_LTC6811(void) {
// 	OS_ERR err;
//     CPU_TS ts;
// 	RTOS_Mutex_Pend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
// 	assertOSError(err);
// 	for(uint8_t current_ic = 0; current_ic < NUM_MINIONS; current_ic++) {
// 		printf("Minion board %d: ", current_ic);
// 		printf("Config: \n\rTX: ");
// 		for(uint8_t i = 0; i < 6; i++) {
// 			printf("%d", Minions[current_ic].config.tx_data[i]);
// 		}
// 		printf("\n\rRX: ");
// 		for(uint8_t i = 0; i < 8; i++) {
// 			printf("%d", Minions[current_ic].config.rx_data[i]);
// 		}
// 		printf("\n\rPEC: %d\n\r", Minions[current_ic].config.rx_pec_match);
// 		printf("Config B: \n\rTX: ");
// 		for(uint8_t i = 0; i < 6; i++) {
// 			printf("%d", Minions[current_ic].configb.tx_data[i]);
// 		}
// 		printf("\n\rRX: ");
// 		for(uint8_t i = 0; i < 8; i++) {
// 			printf("%d", Minions[current_ic].configb.rx_data[i]);
// 		}
// 		printf("\n\rPEC: %d\n\r", Minions[current_ic].configb.rx_pec_match);
// 		printf("COM: \n\rTX: ");
// 		for(uint8_t i = 0; i < 6; i++) {
// 			printf("%d", Minions[current_ic].com.tx_data[i]);
// 		}
// 		printf("\n\rRX: ");
// 		for(uint8_t i = 0; i < 8; i++) {
// 			printf("%d", Minions[current_ic].com.rx_data[i]);
// 		}
// 		printf("\n\rPEC: %d\n\r", Minions[current_ic].com.rx_pec_match);
// 		printf("S Control: \n\rTX: ");
// 		for(uint8_t i = 0; i < 6; i++) {
// 			printf("%d", Minions[current_ic].sctrl.tx_data[i]);
// 		}
// 		printf("\n\rRX: ");
// 		for(uint8_t i = 0; i < 8; i++) {
// 			printf("%d", Minions[current_ic].sctrl.rx_data[i]);
// 		}
// 		printf("\n\rPEC: %d\n\r", Minions[current_ic].sctrl.rx_pec_match);
// 		printf("S Control B: \n\rTX: ");
// 		for(uint8_t i = 0; i < 6; i++) {
// 			printf("%d", Minions[current_ic].sctrlb.tx_data[i]);
// 		}
// 		printf("\n\rRX: ");
// 		for(uint8_t i = 0; i < 8; i++) {
// 			printf("%d", Minions[current_ic].sctrlb.rx_data[i]);
// 		}
// 		printf("\n\rPEC: %d\n\r", Minions[current_ic].sctrlb.rx_pec_match);
// 	}
// 	//release mutex
//   	RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
// }

// /** CLI_Contactor
//  * Interacts with contactor status by
//  * printing the status of the contactor
//  * @param cmd is the array of hashed tokens
//  */
// void CLI_Contactor(char* cmd) {
// 	State contactor = Contactor_GetState();
// 	if(cmd[1] == 0) {
// 		if(contactor == ON) {
// 			printf("Contactor is Enabled\n\r");
// 		} else {
// 			printf("Contactor is Disabled\n\r");
// 		}
// 	} else {
// 		printf("Invalid contactor command\n\r");
// 	}
// }

/** CLI_Charge
 * Checks and displays the desired
 * state of charge parameter(s)
 * @param cmd is the array of hashed tokens
 */
void CLI_Charge(char* cmd) {
	printf("The battery percentage is %.2f%%\n\r", Charge_GetPercent()/PERCENT_CONVERSION);

//	switch(cmd[1]) {
//		case CLI_RESET:
//			Charge_SetAccum(0);	//resets accumulator
//			printf("Accumulator has been reset\n\r");
//			break;
//		case CLI_SET:
//			Charge_SetAccum(cmd[2]);
//			printf("Accumulator has been set to %d%%\n\r", cmd[2]);
//			break;
//		default: 
//			printf("Invalid Charge Command\n\r");
//			break;
//	}
}

// /** setLED
//  * Helper function for CLI_Led
//  * that sets a given LED to a state
//  * @param led is the LED to toggle
//  * @param state is the 'on' of 'off' state
//  * 				represented by a 1/0 or hashes
//  */
// void setLED(Light input, int state) {
// 	if(state == 1 || state == CLI_ON) {
// 		BSP_Light_On(input);
// 	}
// 	else if(state == 0 || state == CLI_OFF) {
// 		BSP_Light_Off(input);
// 	} else {
// 		printf("Invalid LED command\n\r");
// 	}
// }
// 
// /** CLI_Led
//  * Interacts with the LEDs by 
//  * checking error light status
//  * running a full LED test
//  * and turning a specific LED on/off
//  * @param cmd is the array of hashed tokens
//  */
// void CLI_Led(char* cmd) {
// 	switch(cmd[1]) {
// 		case CLI_TEST:
// 			for(int i = 0; i < 10; i++) {
// 				BSP_Light_Toggle(RUN);
// 				DelayMs(100);
// 				BSP_Light_Toggle(UVOLT);
// 				DelayMs(100);
// 				BSP_Light_Toggle(OVOLT);
// 				DelayMs(100);
// 				BSP_Light_Toggle(OTEMP);
// 				DelayMs(100);
// 				BSP_Light_Toggle(OCURR);
// 				DelayMs(100);
// 				BSP_Light_Toggle(WIRE);
// 				DelayMs(100);
// 				BSP_Light_Toggle(WDOG);
// 				DelayMs(100);
// 				BSP_Light_Toggle(CAN);
// 				DelayMs(100);
// 				BSP_Light_Toggle(EXTRA);
// 				DelayMs(100);
// 			}
// 			break;
// 		case CLI_RUN:
// 			setLED(RUN, cmd[2]);
// 			break;
// 		case CLI_OCURR:
// 			setLED(OCURR, cmd[2]);
// 			break;
// 		case CLI_OTEMP:
// 			setLED(OTEMP, cmd[2]);
// 			break;
// 		case CLI_UVOLT:
// 			setLED(UVOLT, cmd[2]);
// 			break;
// 		case CLI_OVOLT:
// 			setLED(OVOLT, cmd[2]);
// 			break;
// 		case CLI_WDOG:
// 			setLED(WDOG, cmd[2]);
// 			break;
// 		case CLI_CAN:
// 			setLED(CAN, cmd[2]);
// 			break;
// 		case CLI_EXTRA:
// 			setLED(EXTRA, cmd[2]);
// 			break;
// 		default:
// 			printf("Invalid LED command\n\r");
// 			break;
// 	}
// }

// /** CLI_CAN
//  * Interacts with CAN by
//  * reading and writing to bus
//  * @param cmd is the array of hashed tokens
//  */
// void CLI_Can(char* cmd) {
//     uint32_t id;
// 	uint8_t rxData[8];
// 	uint8_t txData[8];
// 	uint8_t dataLength = (strlen(hexString)/2) + (strlen(hexString)%2);	// Ceiling of length
// 	for(uint8_t i = 0; i < 7; i+= 2) {
// 		char tempData[2] = {hexString[i], hexString[i+1]};
// 		txData[i/2] = strtol(tempData, NULL, 16);
// 	}
// 	switch(cmd[1]) {
// 		case CLI_READ: 
// 			while(BSP_CAN_Read(&id, rxData)){
//                 for(int32_t i = 0; i < 8; i++) {
//     				printf("0x%x  ", rxData[i]);
//                 }
//                 printf("\r\n");
// 			}
// 		break;
// 		case CLI_WRITE: 
// 			switch(cmd[2]) {
// 				case CLI_TRIP:
// 					BSP_CAN_Write(TRIP, txData, dataLength);
// 					break;
// 				case CLI_CLEAR:
// 					BSP_CAN_Write(ALL_CLEAR, txData, dataLength);
// 					break;
// 				case CLI_OFF:
// 					BSP_CAN_Write(CONTACTOR_STATE, txData, dataLength);
// 					break;
// 				case CLI_CURRENT:
// 					BSP_CAN_Write(CURRENT_DATA, txData, dataLength);
// 					break;
// 				case CLI_VOLTAGE:
// 					BSP_CAN_Write(VOLT_DATA, txData, dataLength);
// 					break;
// 				case CLI_TEMPERATURE:
// 					BSP_CAN_Write(TEMP_DATA, txData, dataLength);
// 					break;
// 				case CLI_CHARGE:
// 					BSP_CAN_Write(SOC_DATA, txData, dataLength);
// 					break;
// 				case CLI_WATCHDOG:
// 					BSP_CAN_Write(WDOG_TRIGGERED, txData, dataLength);
// 					break;
// 				case CLI_ERROR:
// 					BSP_CAN_Write(CAN_ERROR, txData, dataLength);
// 					break;
// 				default:
// 					printf("Invalid ID\n\r");
// 					break;
// 			}
// 			break;
// 		default:
// 			printf("Invalid CAN command\n\r");
// 			break;
// 	}
// }
     
// TODO: Confirm status of Display;
//		 if operational, display proper information (specifics found on Drive)
/** CLI_Display
 * Interacts with the display
 */
//void CLI_Display(void) {
//	printf("Display has not been implemented yet\n\r");
//}

/** CLI_Watchdog
 * Shows whether watchdog was tripped
 * @param cmd is the array of hashed tokens
 */
//void CLI_Watchdog(char* cmd) {
//	if(cmd[1] == 0) {
//		printf("Safety Status: ");
//		if (!BSP_WDTimer_DidSystemReset()){
//			printf("SAFE\n\r");
//		} else if (BSP_WDTimer_DidSystemReset()){
//			printf("DANGER\n\r");
//		}
//	}
//	uint8_t errorAddrArray[2];
//	EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC, 2, errorAddrArray);	// Need to get address where the error is stored
//	uint16_t errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];		// Turn address into one number (not an array)
//	switch(cmd[1]) {
//		case ERROR:
//			printf("Most recent Watchdog error: %d", EEPROM_ReadByte(errorAddr-1));	// Use the address to get the error	
//			break;
//		default:
//			printf("Invalid Watchdog command");
//			break;
//	}
//}

///** CLI_EEPROM
// * Interacts with EEPROM
// * by reading and writing to the EEPROM
// * @param cmd is the array of hashed tokens
// */
//void CLI_EEPROM(char* cmd) {
//	if(cmd[1] == 0) {
//		EEPROM_SerialPrintData();
//		return;
//	}
//	uint8_t errorAddrArray[2];	// Need to get address where the error is stored
//	uint16_t errorAddr = 0;
//	switch(cmd[1]) {
//		case CLI_RESET:
//			EEPROM_Reset();
//			printf("EEPROM has been reset");
//			break;
//		case CLI_ERROR:
//			switch(cmd[2]) {
//				case CLI_FAULT:
//					EEPROM_ReadMultipleBytes(EEPROM_FAULT_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];	// Turn address into one number (not an array)
//					printf("Fault error: %d", EEPROM_ReadByte(errorAddr-1));	// Use the address to get the error
//					break;
//				case CLI_TEMPERATURE:
//					EEPROM_ReadMultipleBytes(EEPROM_TEMP_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
//					printf("Temperature error: %d", EEPROM_ReadByte(errorAddr-1));
//					break;
//				case CLI_VOLTAGE:
//					EEPROM_ReadMultipleBytes(EEPROM_VOLT_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
//					printf("Voltage error: %d", EEPROM_ReadByte(errorAddr-1));
//					break;
//				case CLI_CURRENT:
//					EEPROM_ReadMultipleBytes(EEPROM_CURRENT_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
//					printf("Current error: %d", EEPROM_ReadByte(errorAddr-1));
//					break;
//				case CLI_WATCHDOG:
//					EEPROM_ReadMultipleBytes(EEPROM_WATCHDOG_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
//					printf("Watchdog error: %d", EEPROM_ReadByte(errorAddr-1));
//					break;
//				case CLI_CAN:
//					EEPROM_ReadMultipleBytes(EEPROM_CAN_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
//					printf("CAN error: %d", EEPROM_ReadByte(errorAddr-1));
//					break;
//				case CLI_CHARGE:
//					EEPROM_ReadMultipleBytes(EEPROM_SOC_PTR_LOC, 2, errorAddrArray);
//					errorAddr = (errorAddrArray[0] << 2) + errorAddrArray[1];
//					printf("Charge error: %d", EEPROM_ReadByte(errorAddr-1));
//					break;
//				default:
//					break;
//			}
//			break;
//		default:
//			printf("Invalid EEPROM command\n\r");
//			break;
//	}
//}

/** CLI_Adc
 * Prints the high precision and
 * low precision readings
 */
void CLI_Adc(void) {
	printf("High precision ADC: %d mV\n\r", BSP_ADC_High_GetMilliVoltage());
	printf("Low precision ADC: %d mV\n\r", BSP_ADC_Low_GetMilliVoltage());
}


// /** CLI_Critical
//  * Shuts off contactor manually
//  */  
// void CLI_Critical(void) {
// 	static char response[128];
// 	printf("Please type 'shutdown' to turn the contactor off\n\r");
// 	printf(">> ");
// 	while(1) {
// 		if(BSP_UART_ReadLine(response, 3) > 0) {
// 			if(CLI_StringHash(response) == CLI_SHUTDOWN) {
// 				Contactor_Off();
// 				printf("Contactor is off\n\r");
// 				break;
// 			} else {
// 				printf("Contactor is still on\n\r");
// 				break;
// 			}
// 		}
// 	}
// }

/** CLI_OpenWire
 * Displays open wire status for the BPS system
 */
void CLI_OpenWire(void){
	Voltage_OpenWireSummary();
}

/** CLI_All
 * Displays all information about BPS modules
 * (voltage, current, temperature, charge, contactor, open wires)
 */
void CLI_All(void) {
	char cmd[2];
	printf("Voltage: \n\r");
	cmd[0] = CLI_VOLTAGE;
	cmd[1] = CLI_CHAR_BASE + 0;
	CLI_Voltage(cmd);
	printf("Current: \n\r");
	cmd[0] = CLI_CURRENT;
	CLI_Current(cmd);
	printf("Temperature: \n\r");
	cmd[0] = CLI_TEMPERATURE;
	CLI_Temperature(cmd);
	printf("State of Charge: \n\r");
	cmd[0] = CLI_CHARGE;
	CLI_Charge(cmd);
//	printf("Contactor: \n\r");
//	cmd[0] = CLI_CONTACTOR;
//	CLI_Contactor(cmd);
	printf("Openwire: \n\r");
	cmd[0] = CLI_OPENWIRE;
	CLI_OpenWire();
}

void CLI_Ping(void) {
//    static int i;
    BSP_UART_Write("pong\n\r", 6, UART_USB);
//    printf("pong %i\n\r", i++);
}

/** CLI_Handler
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input is a command string
 */
void CLI_Handler(char* cmd) {
	switch(cmd[0]) {
        case CLI_PING:
            CLI_Ping();
            break;
		case CLI_VOLTAGE:
			CLI_Voltage(cmd);
			break;
		case CLI_CURRENT:
			CLI_Current(cmd);
			break;
		case CLI_TEMPERATURE:
			CLI_Temperature(cmd);
			break;
//		case CLI_CAN:
//			CLI_Can(cmd);
//			break;
		case CLI_ADC:
			CLI_Adc();
			break;
		case CLI_OPENWIRE:
			CLI_OpenWire();
			break;
		case CLI_ALL:
			CLI_All();
			break;
//		case CLI_DISPLAY:
//			CLI_Display();
//			break;
//		case CLI_WATCHDOG:
//			CLI_Watchdog(cmd);
//			break;
//		case CLI_EEPROM:
//			CLI_EEPROM(cmd);
//			break;
//		case CLI_CRITICAL:
//		case CLI_ABORT:
//			CLI_Critical();
//			break;
//		case CLI_REGISTER:
//		case CLI_LTC:
//			CLI_LTC6811();
//			break;
//		case CLI_SWITCH:
//		case CLI_CONTACTOR:
//			CLI_Contactor(cmd);
//			break;
//		case CLI_CHARGE:
//			CLI_Charge(cmd);
//			break;
//		case CLI_LED:
//		case CLI_LIGHTS:
//			CLI_Led(cmd);
//			break;
		default:
			break;
	}
}


void Task_CLI(void *p_arg) {
    (void)p_arg;
    uint32_t len = 0;
    char cmd[128] = {0};

	CLI_Init();

    while(1) {
        RTOS_BPS_SemPend(&UartRx_Sem4, OS_OPT_PEND_BLOCKING);
        len = BSP_UART_ReadLine(cmd, UART_USB);
//        command[13+len] = '\n';
//        command[13+len+1] = '\r';
//        BSP_UART_Write(command, len+13+2, UART_USB);
//            printf("From leader: %s\n\r", command);
        if (len > 0) {
		    CLI_Handler(cmd);	//handle command
        }
    }
}


