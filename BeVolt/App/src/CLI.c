#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "CLI.h"
#include "config.h"

void CLI_InputReturn(char *input){	
  char* split = strtok(input, " ");

	char* first;
  char* second;
  char* third;
	third = NULL;
	char i = 0;
	int num = 0;
	while(split != NULL)
	{
    if (i == 0){
      first = split;
    }else if (i == 1){
      second = split;
    }else if (i == 2){
      third = split;
    }
    split=strtok(NULL," ");
    i++;
	};
	
	if (strcmp(first,"voltage") == 0){
		if (strcmp(second,"check") == 0){
			if (third == NULL){
				for (num = 0;num<12;num++){
					printf("\n\r%d",num);
					printf("# board: ");
					printf("%d",Voltage_GetModuleVoltage(num));
				}
				printf("\n\r");
			}else{	
				printf("\n\r%c",*third);
				printf("# board: ");
				printf("%d",Voltage_GetModuleVoltage(*third));
				printf("\n\r");
			}
		
		}
		else if (strcmp(second,"safe") == 0) {
			printf("\n\r");
			int check = Voltage_IsSafe();
			if (check == 0){
				printf("SAFE");
			}else if (check == 1){
				printf("DANGER");
			}else if (check == 2){
				printf("OVERVOLTAGE");
			}else {
				printf("UNDERVOLTAGE");
			}
			printf("\n\r");
		}
		else if (strcmp(second,"total") == 0) {
			printf("\n\rTotal voltage: ");
			printf("%d",Voltage_GetTotalPackVoltage());
			printf("\n\r");
		}			
		
  }
	else if (strcmp(first,"current") == 0){
		if (strcmp(second,"safe") == 0) {
			printf("\n\r");
			printf("%d",Current_IsSafe());
			printf("\n\r");			
		}
		else if (strcmp(second,"charging") == 0){
			printf("\n\r");
			printf("%d",Current_IsCharging());
			printf("\n\r");			
		}
		else if (strcmp(second,"high") == 0){
			printf("\n\r");		
			printf("%d",Current_GetHighPrecReading());
			printf("\n\r");			
		}
		else if (strcmp(second,"low") == 0){
			printf("\n\r");	
			printf("%d",Current_GetLowPrecReading());
			printf("\n\r");			
		}
   
  }
	else if (strcmp(first,"temp") == 0){
		if (strcmp(second,"average") == 0) {
			printf("\n\r");		
			printf("%d",Temperature_GetTotalPackAvgTemperature());
			printf("\n\r");			
		}
		else if (strcmp(second,"charge") == 0){ 
				Temperature_SetChargeState(*third);
				printf("%d",*third);	
				printf("\n\r");			
		}
		else if (strcmp(second,"check") == 0){
			if (third == NULL){			
				for (num = 0;num<12;num++){
					printf("%d",Temperature_GetModuleTemperature(num));
					printf("\n\r");
					}
				}else { 
					printf("%d",Temperature_GetModuleTemperature(*third));
					printf("\n\r");		
				}
		}
	}
	else {
		printf("Wrong label");
	}
}