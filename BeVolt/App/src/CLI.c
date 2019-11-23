/* CLI.c
 * Command Line Interface wrapper to 
 * define and route commands
 */

#include "CLI.h"

/** CLI_Commands
 * Routes the command given to the proper
 * measurement method to check the desired values
 * @param input command
 */
void CLI_Commands(char *input){	
  char* split = strtok(input, " ");
	char* measurement;
  char* function;
  char* parameter;
	parameter = NULL;
	uint8_t word = 0;
	
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
    split = strtok(NULL," ");
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
}
