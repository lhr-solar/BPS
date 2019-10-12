/** Temperature.c
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 */

// NOTE: All units are in Celsius
// TODO: Create more testing cases
// TODO: Make sure data type is consistent, need to revise if going to Fixed Point w/ .01 resolution
// TODO: Maybe make temperature boards more clean later on once system is more finalized.
// TODO: How to reset danger flags?

#include "Temperature.h"
#include "config.h"

int16_t ModuleTemperatures[NUM_TEMPERATURE_BOARDS][20];	// Each board may not have 20 sensors. Look at config.h for how many sensors
																												// there are on each board. 20 is just max that the board can handle
int16_t ModuleTempStatus[NUM_TEMPERATURE_BOARDS];

typedef enum chargeState {
	discharging,
	charging
} chargeState;

uint8_t chargingState;

// Need a temperature conversion function

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
	
	SPI3_Init();
	
	LTC2983_Init();
}

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return SUCCESS or ERROR
 */
Status Temperature_UpdateMeasurements(){
	// Should there be a isCharging here, constantly checking if temperatures are safe whenever updating measurements?
	// Error for what
	for (board b = TEMP_CS1; b <= TEMP_CS6; b++) {
		LTC2983_StartMeasuringADC(b);
		LTC2983_ReadConversions((int32_t*) ModuleTemperatures[b], b, NUM_SENSORS_ON_TEMP_BOARD_1);			// Need to change this later once devicer driver changes
		// Need to check for SPI TIMEOUT flag for error
	}
	return SUCCESS;
}

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SUCCESS or ERROR
 */
Status Temperature_IsSafe(uint8_t isCharging){									// Maybe have this be an internal thing, change parameters to
	bool dangerFlag = false;
	int16_t maxCharge;
	if (isCharging == 1) {
		maxCharge = MAX_CHARGE_TEMPERATURE_LIMIT;
	} else {
		maxCharge = MAX_DISCHARGE_TEMPERATURE_LIMIT;
	}
	for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
		for (int j = 0; j < NUM_SENSORS_ON_TEMP_BOARD_1; j++) {
			if (ModuleTemperatures[i][j] > MAX_CHARGE_TEMPERATURE_LIMIT) {
				ModuleTempStatus[i] = 1;
				dangerFlag = 1;
				break;
			}
		}
	}
	return dangerFlag ? ERROR : SUCCESS;
}

/** Temperature_SetChargeState
 * Lithium Ion Cells have two separate max temperature limits. There is a limit
 * when the battery is charging and another limit when the battery is discharging.
 * We need to account for these two limits by setting which limit should be used.
 * @param 1 if pack is charging, 0 if discharging
 */
void Temperature_SetChargeState(uint8_t isCharging){  				// Maybe have this be the outside connection, why is this here
	chargingState = isCharging;
}

/** Temperature_GetModulesInDanger
 * Finds all modules that are in danger and stores them into a list
 * @return pointer to index of modules that are in danger
 */
int16_t *Temperature_GetModulesInDanger(void){
	return ModuleTempStatus;
}

/** Temperature_GetModuleTemperature
 * Gets the temperature of a certain module in the battery pack. Since there
 * are NUM_TEMP_SENSORS_PER_MOD sensors per module, just average all of the sensors
 * for that module so each module only has one temperature reading
 * @param index of module
 * @return temperature of module at specified index
 */
int16_t Temperature_GetModuleTemperature(uint16_t moduleIdx){
	// assumes if it wants the first board, then moduleIdx = 0;
	if (moduleIdx >= NUM_TEMPERATURE_BOARDS) return -1;
	
	int32_t total = 0;
	for (int i = 0; i < NUM_SENSORS_ON_TEMP_BOARD_1; i++) {
		total += ModuleTemperatures[moduleIdx][i];
	}
	total /= NUM_SENSORS_ON_TEMP_BOARD_1;
	return total;
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int16_t Temperature_GetTotalPackAvgTemperature(void){
	int32_t total = 0;
	for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
		total += Temperature_GetModuleTemperature(i);
	}
	total /= NUM_TEMPERATURE_BOARDS;
	return total;
}
