/** Temperature.c
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 */

// NOTE: All units are in Celsius

#include "Temperature.h"


uint16_t *ModuleTemperatures;				// list of voltages of all modules

/** Temperature_Init
 * Initializes device drivers including SPI and LTC2983 for Temperature Monitoring
 */
void Temperature_Init(void){
	//LTC2983_Init();
	
}

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return SUCCESS or ERROR
 */
Status Temperature_UpdateMeasurements(){
	//ModuleTemperatures = LTC2983_Measure();
	
	
	return ERROR;
}

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SUCCESS or ERROR
 */
Status Temperature_IsSafe(uint8_t isCharging){
	
	/* TODO: Change to accomodate for charge and discharge limits
	for(int i = 0; i < sizeof(ModuleTemperatures)/sizeof(uint16_t); ++i){
		if(Temperature_GetModuleTemperature(i) > MaxTemperatureLimit){
			return 0;
		}
	}
	*/
	return ERROR;
}

/** Temperature_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
uint16_t *Temperature_GetModulesInDanger(void){
	
	return NULL;
}

/** Temperature_GetModuleTemperature
 * Gets the temperature of a certain module in the battery pack
 * @param index of module
 * @return temperature of module at specified index
 */
uint16_t Temperature_GetModuleTemperature(uint16_t moduleIdx){
	return ModuleTemperatures[moduleIdx];
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
uint16_t Temperature_GetTotalPackAvgTemperature(void){
	int sum = 0;
	for(int i = 0; i < sizeof(ModuleTemperatures)/sizeof(uint16_t); ++i){
		sum += Temperature_GetModuleTemperature(i);
	}
	return (sum / (sizeof(ModuleTemperatures)/sizeof(uint16_t)));
}
