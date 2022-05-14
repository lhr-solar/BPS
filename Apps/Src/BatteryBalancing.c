/* Copyright (c) 2022 UT Longhorn Racing Solar */
/** BatteryBalancing.h
 *  Functions for battery balancing.
 */
#include "LTC6811.h"
#include "Voltage.h"
#include "BatteryBalancing.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"

static uint16_t BattBalancing_GetMinimumVoltage(void);

/**
 * @brief   Loops through all 31 modules, sets discharge bits for any module if its voltage is too high, and clears discharge bits for any modules with voltages that are too low
 * @param   Minions array of the ICs that the modules are connected to
 * @return  None
 */	
void BattBalancing_Balance(cell_asic Minions[]){ 
	OS_ERR err;
	uint16_t lowest = BattBalancing_GetMinimumVoltage(); //get lowest voltage
	bool dcc[MAX_VOLT_SENSORS_PER_MINION_BOARD];
	uint8_t module = 0;
	for (uint8_t i = 0; i < NUM_MINIONS; i++){
		for(uint8_t j = 0; j < MODULES_PER_MINION[i]; j++) {	
			uint16_t voltage = Voltage_GetModuleMillivoltage(module);
			if (voltage > lowest + BALANCING_TOLERANCE_START) {	
				dcc[j] = true;	
			}
			else if(voltage < lowest + BALANCING_TOLERANCE_STOP) { //Clear discharge bit of module if it reaches minimum
				dcc[j] = false;
			}
			module++;
		}
		OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
		assertOSError(err);

		LTC681x_set_cfgr_dis(i, Minions, dcc); //set discharge bits
		LTC6811_wrcfg(NUM_MINIONS, Minions); //write to LTC minions
	
		OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
		assertOSError(err);
	}
}

/**
 * @brief   Finds lowest voltage amongst the 31 modules. Meant to be called only in Balancing_Balance()
 * @param   None
 * @return  the  minimum voltage
 */
static uint16_t BattBalancing_GetMinimumVoltage(void) {
	uint16_t minVoltage = 0;
	minVoltage = Voltage_GetModuleMillivoltage(0);
	for (uint8_t i = 1; i < NUM_BATTERY_MODULES; i++){
		if(Voltage_GetModuleMillivoltage(i) < minVoltage) {
			minVoltage = Voltage_GetModuleMillivoltage(i);//Find minimum voltage
		}
	}
	return minVoltage;
}
