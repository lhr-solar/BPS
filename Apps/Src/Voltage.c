/* Copyright (c) 2020 UT Longhorn Racing Solar */

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

extern OS_MUTEX MinionsASIC_Mutex;

static cell_asic *Minions;

static OS_MUTEX Voltage_Mutex;
static uint16_t VoltageVal[NUM_BATTERY_MODULES]; //Voltage values gathered
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
ErrorStatus Voltage_Init(cell_asic *boards){
	// Record pointer
	Minions = boards;
	//initialize mutex
	OS_ERR err;
    OSInit(&err); 
	OSMutexCreate(&Voltage_Mutex,
				  "Voltage Buffer Mutex",
				  &err
				);

	int8_t error = 0;
	
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(Minions);
	
	//take control of mutex
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, Minions);
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);

	// Read Configuration Register
	wakeup_sleep(NUM_MINIONS);
	//take control of mutex
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	error = LTC6811_rdcfg(NUM_MINIONS, Minions);
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	
	if(error == 0){
		return SUCCESS;
	}else{
		return ERROR;
	}
}

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_UpdateMeasurements(void){
	int8_t error = 0;
	CPU_TS ts;
	// Start Cell ADC Measurements
	wakeup_idle(NUM_MINIONS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();	// In case you want to time the length of the conversion time
	
	// Read Cell Voltage Registers
	wakeup_idle(NUM_MINIONS); // Not sure if wakeup is necessary if you start conversion then read consecutively
	//take control of mutex
	OS_ERR err;
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	error = LTC6811_rdcv(0, NUM_MINIONS, Minions); // Set to read back all cell voltage registers
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	//take control of mutex
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
	assertOSError(err);
	//copies values from cells.c_codes to private array
	OSMutexPend(&Voltage_Mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		VoltageVal[i] = Minions[i / MAX_VOLT_SENSORS_PER_MINION_BOARD].cells.c_codes[i % MAX_VOLT_SENSORS_PER_MINION_BOARD];
	}
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	
	OSMutexPost(&Voltage_Mutex, OS_OPT_POST_NONE, &err);
	if(error == 0){
		return SUCCESS;
	}else{
		return ERROR;
	}
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
		if(voltage > MAX_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR) {
		    return OVERVOLTAGE;
		}
		if (Amps_IsCharging()){
			if(voltage < MIN_VOLTAGE_CHARGING_LIMIT * MILLI_SCALING_FACTOR) return UNDERVOLTAGE;
		}
		else if(voltage < MIN_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR) {
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
	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
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
			if(Voltage_GetModuleMillivoltage(i) > MAX_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR) {
				system->module_checks[i] = OVERVOLTAGE;
			}
			else if(Voltage_GetModuleMillivoltage(i) < MIN_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR){
				system->module_checks[i] = UNDERVOLTAGE;
			}
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
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
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
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
	assertOSError(err);
	
	LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);

	for(int32_t i = 0; i < NUM_MINIONS; i++) {
		if(Minions[i].system_open_wire != 0){
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
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	uint32_t result = LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	return result;
}

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery (0-indexed)
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleMillivoltage(uint8_t moduleIdx){
	CPU_TS ts;
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
	OSMutexPend(&Voltage_Mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
	assertOSError(err);
	uint16_t ret = VoltageVal[moduleIdx] / 10;
	OSMutexPost(&Voltage_Mutex, OS_OPT_POST_NONE, &err);
	assertOSError(err);
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



/* Copyright (c) 2020 UT Longhorn Racing Solar */

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

extern OS_MUTEX MinionsASIC_Mutex;

static cell_asic *Minions;

static OS_MUTEX Voltage_Mutex;
static uint16_t VoltageVal[NUM_BATTERY_MODULES]; //Voltage values gathered
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
ErrorStatus Voltage_Init(cell_asic *boards){
    // Record pointer
    Minions = boards;
    //initialize mutex
    OS_ERR err;
    OSInit(&err); 
    OSMutexCreate(&Voltage_Mutex,
                  "Voltage Buffer Mutex",
                  &err
                );

    int8_t error = 0;
    
    wakeup_sleep(NUM_MINIONS);
    LTC6811_Init(Minions);
    
    //take control of mutex
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
      assertOSError(err);
    // Write Configuration Register
    LTC6811_wrcfg(NUM_MINIONS, Minions);
    //release mutex
      OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
      assertOSError(err);

    // Read Configuration Register
    wakeup_sleep(NUM_MINIONS);
    //take control of mutex
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
      assertOSError(err);
    error = LTC6811_rdcfg(NUM_MINIONS, Minions);
    //release mutex
      OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
      assertOSError(err);
    
    if(error == 0){
        return SUCCESS;
    }else{
        return ERROR;
    }
}

/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_UpdateMeasurements(void){
    int8_t error = 0;
    CPU_TS ts;
    // Start Cell ADC Measurements
    wakeup_idle(NUM_MINIONS);
    LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
    LTC6811_pollAdc();    // In case you want to time the length of the conversion time
    
    // Read Cell Voltage Registers
    wakeup_idle(NUM_MINIONS); // Not sure if wakeup is necessary if you start conversion then read consecutively
    //take control of mutex
    OS_ERR err;
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
      assertOSError(err);
    error = LTC6811_rdcv(0, NUM_MINIONS, Minions); // Set to read back all cell voltage registers
    //release mutex
      OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
      assertOSError(err);
    //take control of mutex
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    assertOSError(err);
    //copies values from cells.c_codes to private array
    OSMutexPend(&Voltage_Mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    for(int i = 0; i < NUM_BATTERY_MODULES; i++){
        VoltageVal[i] = Minions[i / MAX_VOLT_SENSORS_PER_MINION_BOARD].cells.c_codes[i % MAX_VOLT_SENSORS_PER_MINION_BOARD];
    }
    //release mutex
      OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
      assertOSError(err);
    
    OSMutexPost(&Voltage_Mutex, OS_OPT_POST_NONE, &err);
    if(error == 0){
        return SUCCESS;
    }else{
        return ERROR;
    }
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
        if(voltage > MAX_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR) {
            return OVERVOLTAGE;
        }
        if (Amps_IsCharging()){
            if(voltage < MIN_VOLTAGE_CHARGING_LIMIT * MILLI_SCALING_FACTOR) return UNDERVOLTAGE;
        }
        else if(voltage < MIN_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR) {
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
    OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
      assertOSError(err);
    //put all the bits from each minion's system_open_wire variable into one variable
    for(int k = 0; k < NUM_MINIONS; k++){
        wires = (Minions[k].system_open_wire & 0x1FF);    //there are at most 8 modules per IC, bit 0 is GND
        for(int s = 0; s < NUM_PINS_PER_LTC; s++){
            if(k == 3 && s == NUM_PINS_PER_LTC-1){
                break;    //the last IC has only 7 modules 
            }
            openWires[openWireIdx] = (wires >> s) & 1;
            openWireIdx++;
        }
    }
    
    for (int i = 0; i < TOTAL_VOLT_WIRES; i++) {    
        if(i < NUM_BATTERY_MODULES){
            // Check if battery is in range of voltage limit
            if(Voltage_GetModuleMillivoltage(i) > MAX_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR) {
                system->module_checks[i] = OVERVOLTAGE;
            }
            else if(Voltage_GetModuleMillivoltage(i) < MIN_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR){
                system->module_checks[i] = UNDERVOLTAGE;
            }
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
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
      assertOSError(err);
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
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
    assertOSError(err);
    
    LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);

    for(int32_t i = 0; i < NUM_MINIONS; i++) {
        if(Minions[i].system_open_wire != 0){
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
      OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
      assertOSError(err);
    uint32_t result = LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
    //release mutex
      OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
      assertOSError(err);
    return result;
}

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery (0-indexed)
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleMillivoltage(uint8_t moduleIdx){
    CPU_TS ts;
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
    OSMutexPend(&Voltage_Mutex, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    assertOSError(err);
    uint16_t ret = VoltageVal[moduleIdx] / 10;
    OSMutexPost(&Voltage_Mutex, OS_OPT_POST_NONE, &err);
    assertOSError(err);
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



