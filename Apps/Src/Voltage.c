/** Voltage.c
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 */

#include "Voltage.h"
#include "LTC6811.h"
#include "config.h"
#include <stdlib.h>

static cell_asic *Minions;
static uint16_t VoltageVal[NUM_BATTERY_MODULES]; //Voltage values gathered
/** LTC ADC measures with resolution of 4 decimal places, 
 * But we standardized to have 3 decimal places to work with
 * millivolts
 */

/** Voltage_Init
 * Initializes all device drivers including LTC6811 and GPIO to begin Voltage Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 * @return SUCCESS or ERROR
 */
ErrorStatus Voltage_Init(cell_asic *boards){
	// Record pointer
	Minions = boards;

	int8_t error = 0;
	
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(Minions);
	
	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, Minions);

	// Read Configuration Register
	wakeup_sleep(NUM_MINIONS);
	error = LTC6811_rdcfg(NUM_MINIONS, Minions);
	
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
	
	// Start Cell ADC Measurements
	wakeup_idle(NUM_MINIONS);
	LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
	LTC6811_pollAdc();	// In case you want to time the length of the conversion time
	
	// Read Cell Voltage Registers
	wakeup_idle(NUM_MINIONS); // Not sure if wakeup is necessary if you start conversion then read consecutively
	error = LTC6811_rdcv(0, NUM_MINIONS, Minions); // Set to read back all cell voltage registers
	
	//copies values from cells.c_codes to private array
	for(int i = 0; i < NUM_BATTERY_MODULES; i++){
		VoltageVal[i] = Minions[i / MAX_VOLT_SENSORS_PER_MINION_BOARD].cells.c_codes[i % MAX_VOLT_SENSORS_PER_MINION_BOARD];
	}
	
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
		if(voltage > MAX_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR){
			return OVERVOLTAGE;
		}
		
		else if(voltage < MIN_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR){
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
SafetyStatus *Voltage_GetModulesInDanger(void){
	static SafetyStatus checks[NUM_BATTERY_MODULES];
	uint32_t open_wires = Voltage_GetOpenWire();
	
	for (int i = 0; i < NUM_BATTERY_MODULES; i++) {	
		// Check if battery is under max voltage limit
		if (Voltage_GetModuleMillivoltage(i) > MAX_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR){
			checks[i] = OVERVOLTAGE;
		}
		// Check if battery is above minimum voltage limit
		else if (Voltage_GetModuleMillivoltage(i) < MIN_VOLTAGE_LIMIT * MILLI_SCALING_FACTOR){
			checks[i] = UNDERVOLTAGE;
		}
		//Check if open wires at module
		else if((open_wires >> i) & 1) {
			checks[i] = OPENWIRE;
		} 
		//No errors 
		else {
			checks[i] = SAFE;
		}
	}
	return checks;
}

/** Voltage_OpenWireSummary
 * Runs the open wire method with print=true
 */
void Voltage_OpenWireSummary(void){
	wakeup_idle(NUM_MINIONS);
	LTC6811_run_openwire_multi(NUM_MINIONS, Minions, true);
}

/** Voltage_OpenWire
 * Uses the LTC6811_run_openwire_multi() function to check for open wires
 * @return SafetyStatus
 */
SafetyStatus Voltage_OpenWire(void){
	wakeup_idle(NUM_MINIONS);
	long openwires = LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
	if(openwires != 0){
		return DANGER;
	} else {
		return SAFE;
	}
}

/** Voltage_GetOpenWire
 * Finds the pin locations of the open wires
 * @return hexadecimal string (1 means open wire, 0 means closed)
 */
uint32_t Voltage_GetOpenWire(void){
	wakeup_idle(NUM_MINIONS);
	return LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
}

/** Voltage_GetModuleVoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery (0-indexed)
 * @return voltage of module at specified index
 */
uint16_t Voltage_GetModuleMillivoltage(uint8_t moduleIdx){
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

	return VoltageVal[moduleIdx] / 10;
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




