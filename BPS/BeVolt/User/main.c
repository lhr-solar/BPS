/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 * @authors Sijin Woo, Chase Block
 * @lastRevised 10/2/2018
 */

#include <stdint.h>
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
//#include "Contactor.h"
#include "EEPROM.h"
//#include "CANlib.h"
#include "WDTimer.h"
#include "stm32f4xx.h"

void initialize();

int Mainmain(){
	initialize();
	WDTimer_Start();
	while(1){
		if(Voltage_IsSafe() && Current_IsSafe() && Temperature_IsSafe()){
			Voltage_UpdateMeasurements();
			Current_UpdateMeasurements();
			Temperature_UpdateMeasurements();
		}
		
		WDTimer_Reset();
	}
}

void initialize(void){
	EEPROM_Init();
	WDTimer_Init();
	//TODO EEPROM
	//VoltSlave.setLimits();
	//AmpSlave.setLimits();
	//CelsiusSlave.setLimits();
}




// LTC6811 Test
#include "SPI.h"
#include "LTC6811.h"
#include "UART.h"
#include <string.h>
int LTCmain(){
	LTC6811_Init();
	UART3_Init(9600);
	while(1){
		
	}
}

// UART Test
int UARTTestmain(){
	char str[] = "Hello there.";
	UART3_Init(9600);
	while(1){
		UART3_Write(str, strlen(str));
		for(uint32_t i = 0; i < 100000; i++);
	}
}

// Gyro test
#include "FXAS21002CQR1.h"
int gyroTestmain(){
	FXAS21002CQR1_Init();
	UART3_Init(9600);
	while(1){
		uint16_t *vals = FXAS21002CQR1_Measure();
		// print or something
	}
}
