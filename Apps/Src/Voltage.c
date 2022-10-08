/* Copyright (c) 2022 UT Longhorn Racing Solar */

/** Voltage.c
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 */

#include "Voltage.h"
#include "LTC6811.h"
#include "config.h"
#include <stdlib.h>
#include "os.h"
#include "Tasks.h"
#include "Amps.h"

// median filter
#define MEDIAN_FILTER_TYPE uint16_t
#define MEDIAN_FILTER_DEPTH 5
#define MEDIAN_FILTER_CHANNELS NUM_BATTERY_MODULES
#define MEDIAN_FILTER_NAME VoltageFilter
#include "MedianFilter.h"
static VoltageFilter_t VoltageFilter;

static cell_asic *Minions;

static OS_MUTEX Voltage_Mutex;
static uint16_t Voltages[NUM_BATTERY_MODULES]; // Voltage values gathered, in units of 0.1 mV
static uint32_t openWires[TOTAL_VOLT_WIRES];

/** LTC ADC measures with resolution of 4 decimal places, 
 * But we standardized to have 3 decimal places to work with
 * millivolts
 */

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 * @param voltageMutex pointer to mutex, meant to pass pointer to VoltageBuffer_Mutex
 * @return SUCCESS or ERROR
 */
void Voltage_Init(cell_asic *boards){
	// Record pointer
	Minions = boards;
	//initialize mutex
	OS_ERR err;
	OSMutexCreate(&Voltage_Mutex,
				  "Voltage Buffer Mutex",
				  &err
				);
					
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(Minions);
	
	//take control of mutex
  	RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, Minions);
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);

	// Read Configuration Register
	// take control of mutex
  	RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	wakeup_sleep(NUM_MINIONS);
	LTC6811_rdcfg_safe(NUM_MINIONS, Minions);
	// release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	
	// Initialize median filter. There should be no modules with less than 0 volts or more than 5 volts
	VoltageFilter_init(&VoltageFilter, 0, 50000);
}

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 */
void Voltage_UpdateMeasurements(void){
	// Start Cell ADC Measurements
	wakeup_sleep(NUM_MINIONS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();	// In case you want to time the length of the conversion time
	
	// Read Cell Voltage Registers
	//take control of mutex
	OS_ERR err;
  	RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	LTC6811_rdcv_safe(0, NUM_MINIONS, Minions); // Set to read back all cell voltage registers
	//copies values from cells.c_codes to private array

	// package raw voltage values into single array
	static uint16_t rawVoltages[NUM_BATTERY_MODULES];
	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		rawVoltages[i] = Minions[i / MAX_VOLT_SENSORS_PER_MINION_BOARD].cells.c_codes[i % MAX_VOLT_SENSORS_PER_MINION_BOARD];
	}
	// release minions asic mutex
	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	
	// run median filter
	VoltageFilter_put(&VoltageFilter, rawVoltages);

	// update public voltage values
	RTOS_BPS_MutexPend(&Voltage_Mutex, 0, OS_OPT_PEND_BLOCKING);
	VoltageFilter_get(&VoltageFilter, Voltages);
	//release mutex
	OSMutexPost(&Voltage_Mutex, OS_OPT_POST_NONE, &err);
	assertOSError(err);
}

/** Voltage_CheckStatus
 * Checks if all battery modules are safe
 * @return SAFE or danger: UNDERVOLTAGE or OVERVOLTAGE
 */
SafetyStatus Voltage_CheckStatus(void){
	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		uint16_t voltage = Voltage_GetModuleMillivoltage(i);
			
		// VOLTAGE_LIMITS are in floating point. The LTC6811 sends the voltage data
		// as unsigned 16-bit fixed point integers with a resolution of 0.00001
		if(voltage > MAX_VOLTAGE_LIMIT) {
		    return OVERVOLTAGE;
		}
		if (Amps_IsCharging()){
			if(voltage < MIN_VOLTAGE_CHARGING_LIMIT) return UNDERVOLTAGE;
		}
		else if(voltage < MIN_VOLTAGE_LIMIT) {
		    return UNDERVOLTAGE;
		}
	}
	return SAFE;
}

/** Voltage_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list.
 * Each module corresponds to an index of the array of SafetyStatus
 * @return pointer to index of modules that are in danger
 */
void Voltage_GetModulesInDanger(VoltageSafety_t* system){
	uint32_t wires;
	uint32_t openWireIdx = 0;
	OS_ERR err;
	RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	//put all the bits from each minion's system_open_wire variable into one variable
	for(int k = 0; k < NUM_MINIONS; k++){
		wires = (Minions[k].system_open_wire & 0x1FF);	//there are at most 8 modules per IC, bit 0 is GND
		for(int s = 0; s < NUM_PINS_PER_LTC; s++){
			if(k == 3 && s == NUM_PINS_PER_LTC-1){
				break;	//the last IC has only 7 modules 
			}
			openWires[openWireIdx] = (wires >> s) & 1;
			openWireIdx++;
		}
	}
	
	for (int i = 0; i < TOTAL_VOLT_WIRES; i++) {	
		if(i < NUM_BATTERY_MODULES){
			// Check if battery is in range of voltage limit
			if(Voltage_GetModuleMillivoltage(i) > MAX_VOLTAGE_LIMIT) {
				system->module_checks[i] = OVERVOLTAGE;
			}
			else if(Voltage_GetModuleMillivoltage(i) < MIN_VOLTAGE_LIMIT){
				system->module_checks[i] = UNDERVOLTAGE;
			}
			else system->module_checks[i] = SAFE;
		}
		if(openWires[i] == 1) {
			system->wire_checks[i] = DANGER;
		} else {
			system->wire_checks[i] = SAFE;
		}
	}
	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
}

/** Voltage_OpenWireSummary
 * Runs the open wire method with print=true
 */
void Voltage_OpenWireSummary(void){
	wakeup_idle(NUM_MINIONS);
	//take control of mutex
	OS_ERR err;
  	RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	LTC6811_run_openwire_multi(NUM_MINIONS, Minions, true);
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
}

/** Voltage_OpenWire
 * Uses the LTC6811_run_openwire_multi() function to check for open wires
 * @return SafetyStatus
 */
SafetyStatus Voltage_OpenWire(void){
	SafetyStatus status = SAFE;
	wakeup_idle(NUM_MINIONS);

	OS_ERR err;
  	RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	
	LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);

	for(int32_t i = 0; i < NUM_MINIONS; i++) {
		if(Minions[i].system_open_wire != 0){
			if ((i == NUM_MINIONS -1) && ((Minions[i].system_open_wire & 0xEF) != 0)) { //The last Voltage board is only connected to 7 modules
				break; //Open Wire test runs using MAX_VOLT_SENSORS_PER_MINION_BOARD so value of last module should be cleared
			}
			status = DANGER;
			break;
		}
	}

	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);

	return status;
}

/** Voltage_GetOpenWire
 * Finds the pin locations of the open wires
 * @return hexadecimal string (1 means open wire, 0 means closed)
 */
uint32_t Voltage_GetOpenWire(void){
	wakeup_idle(NUM_MINIONS);
	//take control of mutex
	OS_ERR err;
	if(!Fault_Flag){
  		RTOS_BPS_MutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING);
	}
	uint32_t result = LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
	//release 
	if (!Fault_Flag){
  		OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  		assertOSError(err);
	}
	return result;
}

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery (0-indexed)
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleMillivoltage(uint8_t moduleIdx){
	OS_ERR err;
	// These if statements prevents a hardfault.
    if(moduleIdx >= NUM_BATTERY_MODULES) {
        return 0xFFFF;  // return -1 which indicates error voltage
    }
    // Each board will measure the same number of modules except for the last board in the daisy chain.
    // To find which minion board the battery module (moduleIdx) is assigned to, we need to
    // divide the moduleIdx by how many battery modules are assigned to each minion board
    // (indicated by MAX_VOLT_SENSORS_PER_MINION_BOARD). If the minion idx exceeds how many minion
    // boards are currently present, then return an error voltage.
    if((moduleIdx / MAX_VOLT_SENSORS_PER_MINION_BOARD) >= NUM_MINIONS) {
        return 0xFFFF;  // return -1 which indicates error voltage
    }
	if (!Fault_Flag){
		RTOS_BPS_MutexPend(&Voltage_Mutex, 0, OS_OPT_PEND_BLOCKING);
	}
	uint16_t ret = Voltages[moduleIdx] / 10;
	if (!Fault_Flag){
		OSMutexPost(&Voltage_Mutex, OS_OPT_POST_NONE, &err);
		assertOSError(err);
	}
	return ret;
}

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint32_t Voltage_GetTotalPackVoltage(void){
	int sum = 0;
	for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
		sum += Voltage_GetModuleMillivoltage(i);
	}
	return sum;
}



