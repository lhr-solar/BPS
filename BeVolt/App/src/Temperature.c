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
	// Initialize CS pins (PC6-8, PB13-15)
	GPIO_InitTypeDef GPIOB_InitStruct;
	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// Up vs Down
	GPIOB_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIOB_InitStruct);
	
	GPIO_InitTypeDef GPIOC_InitStruct;
	GPIOC_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIOC_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOC_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// Up vs Down
	GPIOC_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOC_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIOC_InitStruct);
	
	// Start all temperature CS pins HIGH
	GPIOB->ODR = GPIOB->ODR | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIOC->ODR = GPIOC->ODR | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	
	SPI_Init8();
	
	LTC2983_Init();
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


/** Temperature_SetLimits
 * Sets the max temperature limit the cells can reach before danger
 * @param max temperature limit
 */
void Temperature_SetLimits(uint16_t ceiling){
	MaxTemperatureLimit = ceiling;
}


	
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
