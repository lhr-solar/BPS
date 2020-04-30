/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 */

#include "common.h"
#include "config.h"
#include "LTC6811.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "EEPROM.h"
#include "SoC.h"
#include "CLI.h"
#include "BSP_UART.h"
#include "BSP_Contactor.h"
#include "BSP_Lights.h"
#include "BSP_WDTimer.h"


cell_asic Minions[NUM_MINIONS];
bool override = false;		// This will be changed by user via CLI
char command[128];

void initialize(void);
void preliminaryCheck(void);
void faultCondition(void);

int realmain(){
	// __disable_irq();		// Disable all interrupts until initialization is done
	initialize();			// Initialize codes/pins
	preliminaryCheck();		// Wait until all boards are powered on
	// __enable_irq();			// Enable interrupts

	BSP_WDTimer_Start();

	while(1) {
		// First update the measurements.
		Voltage_UpdateMeasurements();
		Current_UpdateMeasurements();
    	Temperature_UpdateAllMeasurements();

		// Update battery percentage
		SoC_Calculate(Current_GetLowPrecReading());

		// Checks for user input to send to CLI
		if(BSP_UART_ReadLine(command)) {
			CLI_Handler(command);
		}
		
		SafetyStatus current = Current_CheckStatus(override);
		SafetyStatus temp = Temperature_CheckStatus(Current_IsCharging());
		SafetyStatus voltage = Voltage_CheckStatus();

		// Check if everything is safe (all return SAFE = 0)
		if((current == SAFE) && (temp == SAFE) && (voltage == SAFE)) {
			BSP_Contactor_On();
		}
		else if((current == SAFE) && (temp == SAFE) && (voltage == UNDERVOLTAGE) && override) {
			BSP_Contactor_On();
		} else {
			break;
		}

		// Update necessary
		// CAN_SendMessageStatus()	// Most likely need to put this on a timer if sending too frequently

		BSP_WDTimer_Reset();
	}

	// BPS has tripped if this line is reached
	faultCondition();
	return 0;
}

/**
 * Initialize system.
 *	1. Initialize device drivers.
 *		- This includes communication protocols, GPIO pins, timers
 *	2. Set the current, voltage, and temperature limits.
 *		- Give wrappers (Voltage, Current, Temperature) the limits
 */
void initialize(void){
	BSP_Lights_Init();
	BSP_Contactor_Init();
	BSP_Contactor_Off();
	BSP_WDTimer_Init();
	EEPROM_Init();
	SoC_Init();
	Current_Init();
	Voltage_Init(Minions);
	Temperature_Init(Minions);
	CLI_Init(Minions);

	// __enable_irq();
	CLI_Startup();

	// Checks to see if the batteries need to be charged
	Voltage_UpdateMeasurements();
	SafetyStatus voltage = Voltage_CheckStatus();
	if(voltage == UNDERVOLTAGE) {
		printf("Do you need to charge the batteries? (y/n)\n\r>> ");
		uint32_t wait = 0;
		while(wait < STARTUP_WAIT_TIME) {
			if(BSP_UART_ReadLine(command)) {
				override = command[0] == 'y' ? true : false;
				break;
			}
			wait++;
		}
	}
}

/** preliminaryCheck
 * Before starting any data monitoring, check if all the boards are powered. If we start the data
 * collection before everything is powered on, then the system will immediately fault and not turn on
 * even though everything is safe.
 */
void preliminaryCheck(void){
	// Check if Watch dog timer was triggered previously
	if (BSP_WDTimer_DidSystemReset() == DANGER) {
		BSP_Light_On(FAULT);
		BSP_Light_On(WDOG);
		while(1);		// Spin
	}
}

/** faultCondition
 * This block of code will be executed whenever there is a fault.
 * If bps trips, make it spin and impossible to connect the battery to car again
 * until complete reboot is done.
 */
void faultCondition(void){
	BSP_Contactor_Off();
	BSP_Light_Off(RUN);
    BSP_Light_On(FAULT);

	uint8_t error = 0;

	if(!Current_CheckStatus(false)){
		error |= FAULT_HIGH_CURRENT;
		BSP_Light_On(OCURR);
	}

	if(!Voltage_CheckStatus()){
		// Toggle Voltage fault LED
		switch(Voltage_CheckStatus()){
			case OVERVOLTAGE:
				error |= FAULT_HIGH_VOLT;
				BSP_Light_On(OVOLT);
				SoC_Calibrate(OVERVOLTAGE);
				break;

			case UNDERVOLTAGE:
				error |= FAULT_LOW_VOLT;
				BSP_Light_On(UVOLT);
				SoC_Calibrate(UNDERVOLTAGE);
				break;

			default:
				error |= FAULT_VOLT_MISC;
				BSP_Light_On(OVOLT);
				BSP_Light_On(UVOLT);
				break;
		}
	}

	if(!Temperature_CheckStatus(Current_IsCharging())){
		error |= FAULT_HIGH_TEMP;
		BSP_Light_On(OCURR);
	}

	// Log all the errors that we have
	for(int i = 1; i < 0x00FF; i <<= 1) {
		if(error & i) EEPROM_LogError(i);
	}

	// Log all the relevant data for each error
	for(int i = 1; i < 0x00FF; i <<= 1) {
		if((error & i) == 0) continue;

		SafetyStatus *voltage_modules;
		uint8_t *temp_modules;
		uint16_t curr;
		switch(i) {
		// Temperature fault handling
		case FAULT_HIGH_TEMP:
			temp_modules = Temperature_GetModulesInDanger();
			for(int j = 0; j < NUM_BATTERY_MODULES; ++j)
				if(temp_modules[j]) EEPROM_LogData(FAULT_HIGH_TEMP, j);
			break;

		// Voltage fault handling
		case FAULT_HIGH_VOLT:
		case FAULT_LOW_VOLT:
		case FAULT_VOLT_MISC:
			voltage_modules = Voltage_GetModulesInDanger();
			for(int j = 0; j < NUM_BATTERY_MODULES; ++j)
				if(voltage_modules[j]) EEPROM_LogData(i, j);
			break;

		// Current fault handling
		case FAULT_HIGH_CURRENT:
			curr = Current_GetLowPrecReading();
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & curr);
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & (curr >> 8));
			curr = Current_GetHighPrecReading();
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & curr);
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & (curr >> 8));
			break;
		}
	}

	while(1) {
		BSP_WDTimer_Reset();	// Even though faulted, WDTimer needs to be updated or else system will reset
					// causing WDOG error. WDTimer can't be stopped after it starts.
	}
}


// DELETE ALL CODE BELOW THIS

int main(void) {
	Current_Init();
	Current_UpdateMeasurements();
	while (1) {
		printf("=============================\n\r");
		printf("Low Precision: %gA\n\r", Current_GetLowPrecReading()/1000.0);
		printf("High Precision: %gA\n\r", Current_GetHighPrecReading()/1000.0);
		for (int i = 0; i < 500000000; i++);
	}
}
	