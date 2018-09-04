/** main.cpp
 * Program for UTSVT BeVolt's Battery Protection System
 * @authors Sijin Woo
 * @lastRevised 9/4/2018
 */

#include <stdint.h>
#include "mbed.h"
#include "Voltage.h"
#include "Current.h"
#include "Temperature.h"
#include "Contactors.h"
#include "EEPROM.h"
#include "CANlib.h"
#include "WDTimer.h"
#include ""

// Global Variables
Voltage VoltSlave();
Current AmpSlave();
Temperature CelsiusSlave();


int main(){
	initialize();
	while(1){
		if(VoltSlave.isSafe() && AmpSlave.isSafe() && CelsiusSlave.isSafe()){
			VoltSlave.updateMeasurements();
			AmpSlave.updateMeasurements();
			CelsiusSlave.updateMeasurements();
		}
	}
}

void initialize(void){
	EEPROM_Init();
	WDTimer_Init();
	VoltSlave.setLimits();
	AmpSlave.setLimits();
	CelsiusSlave.setLimits();
}
