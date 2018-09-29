/** main.c
 * Program for UTSVT BeVolt's Battery Protection System
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/4/2018
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


int main(){
	initialize();
	while(1){
		if(Voltage_IsSafe() && Current_IsSafe() && Temperature_IsSafe()){
			Voltage_UpdateMeasurements();
			Current_UpdateMeasurements();
			Temperature_UpdateMeasurements();
		}
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
