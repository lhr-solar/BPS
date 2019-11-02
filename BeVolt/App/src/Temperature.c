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
#include "LTC6811.h"

extern cell_asic Modules[NUM_VOLTAGE_BOARDS];


#define MUX1 0x91
#define MUX2 0x93 
#define START_CODE 0x06
#define ACK_CODE 0
#define NACK_STOP_CODE 0x9

int16_t ModuleTemperatures[NUM_TEMPERATURE_BOARDS][20];	// Each board may not have 20 sensors. Look at config.h for how many sensors
																												// there are on each board. 20 is just max that the board can handle

uint8_t ModuleTempStatus[NUM_TEMPERATURE_BOARDS];				// Used to check and return if a particular module is in danger. 

uint8_t ChargingState;																	// 0 if discharging 1 if charging

cell_asic TempCell[1];

/** Temperature_Init
 * Initializes device drivers including SPI and LTC2983 for Temperature Monitoring
 */
void Temperature_Init(void){
//	// Initialize CS pins (PC6-8, PB13-15)
//	GPIO_InitTypeDef GPIOB_InitStruct;
//	GPIOB_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIOB_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIOB_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// Up vs Down
//	GPIOB_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIOB_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init(GPIOB, &GPIOB_InitStruct);
//	
//	GPIO_InitTypeDef GPIOC_InitStruct;
//	GPIOC_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
//	GPIOC_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIOC_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// Up vs Down
//	GPIOC_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIOC_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_Init(GPIOC, &GPIOC_InitStruct);
//	
//	// Start all temperature CS pins HIGH
//	GPIOB->ODR = GPIOB->ODR | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIOC->ODR = GPIOC->ODR | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
//	
//	SPI3_Init();
//	
//	LTC2983_Init();


	//LTC6811_set_cfgr_gpio(

}

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return SUCCESS or ERROR
 * CONCERNS: 
 * Should there be a isCharging here, constantly checking if temperatures are safe whenever updating measurements?
 * What are we checking for error exactly?
 */

ErrorStatus Temperature_UpdateMeasurements(){
	for (board b = TEMP_CS1; b < NUM_TEMPERATURE_BOARDS; b++) {
		LTC2983_StartMeasuringADC(b);
		LTC2983_ReadConversions((int32_t*) ModuleTemperatures[b], b, NUM_SENSORS_ON_TEMP_BOARD_1);			// Need to change this later once devicer driver changes
		// Need to check for SPI TIMEOUT flag for error,
	}
	return SUCCESS;
}

/** Temperature_ChannelConfig
 * Configures which temperature channel you're sampling from
 * Assumes there are only 2 muxes
 * @Param channel number
 * @return SUCCESS or ERROR
 * CONCERNS: 
 * Should we have tempChannel be from 1 - 16 or 0-15?
 */


ErrorStatus Temperature_ChannelConfig(uint8_t tempChannel) {
	cell_asic temp;
	uint8_t muxAddress;
	uint8_t otherMux;
	
	temp.isospi_reverse = false;
	
	if (tempChannel > 7) {
		muxAddress = MUX2;
		otherMux = MUX1;
	}
	else { 
		muxAddress = MUX1;	
		otherMux = MUX2;
	}
	
	// 0110 is start code, must be the 4 msb's then the 4 msb's of the data (addr of mux)
	// remaind of otherMux, then 0000 for just an acknowledge signal at 9th cycle
	
	// First clear the other mux so that it does not interfere
	// FORMAT: SEND ADDRESS OF MUX FIRST
	temp.com.tx_data[0] = (START_CODE << 4) + ((otherMux & 0xF0)>>4); 				// 0110 START CODE
	temp.com.tx_data[1] = ((otherMux & 0x0F) << 4) + ACK_CODE;								// Acknowledge signal at 9th cycle
	
	
	// Sends what channel to open. 4th bit is the enable bit
	// Turn all channels off 
	temp.com.tx_data[2] = (START_CODE << 4) + (0);
	temp.com.tx_data[3] = ((0) << 4) + NACK_STOP_CODE;
	
	LTC6811_wrcomm(1, &temp);
	LTC6811_stcomm();
	

	// Send Address for a particular mux
	temp.com.tx_data[0] = (START_CODE << 4) + ((muxAddress & 0xF0)>>4); 				// 0110 START CODE
	temp.com.tx_data[1] = ((muxAddress & 0x0F) << 4) + ACK_CODE;								// Acknowledge signal at 9th cycle
	
	
	// Sends what channel to open. 8 is the enable bit
	// 8 + temp_channel
	temp.com.tx_data[2] = (0x6 << 4) + (0);
	temp.com.tx_data[3] = ((8 + tempChannel) << 4) + NACK_STOP_CODE;
	
	
	LTC6811_wrcomm(1, &temp);
	LTC6811_stcomm();
	
	return SUCCESS;
}

//ErrorStatus Temperature_ReadADC(uint8_t tempChannel) {
//	// Read adc from GPIO1
//	LTC681x_rdaux(0, 1, TempCell);
//	return SUCCESS;
//}


/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_IsSafe(uint8_t isCharging){
	bool dangerFlag = false;
	int16_t maxCharge = isCharging == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;
	
	for (int i = 0; i < NUM_TEMPERATURE_BOARDS; i++) {
		for (int j = 0; j < NUM_SENSORS_ON_TEMP_BOARD_1; j++) {
			if (ModuleTemperatures[i][j] > maxCharge) {
				ModuleTempStatus[i] = 1;
				dangerFlag = true;
				break;
			}
		}
	}
	return dangerFlag ? DANGER : SAFE;
}

/** Temperature_SetChargeState
 * Lithium Ion Cells have two separate max temperature limits. There is a limit
 * when the battery is charging and another limit when the battery is discharging.
 * We need to account for these two limits by setting which limit should be used.
 * ChargingState is used for GetModulesInDanger when we can't use the current module to check if charging or discharging
 * @param 1 if pack is charging, 0 if discharging
 */
void Temperature_SetChargeState(uint8_t isCharging){  				// Maybe have this be the outside connection, why is this here
	ChargingState = isCharging;
}

/** Temperature_GetModulesInDanger
 * Finds all modules that are in danger and stores them into a list
 * This function is called when you can't use the current module to see if it is charging.
 * @return pointer to index of modules that are in danger
 */
uint8_t *Temperature_GetModulesInDanger(void){
	Temperature_IsSafe(ChargingState);
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



uint16_t Temperature_GetRawADC(uint8_t ADCMode, uint8_t GPIOChannel) {
	
	wakeup_sleep(NUM_VOLTAGE_BOARDS);

	LTC6811_adax(ADCMode, GPIOChannel);		// Start ADC conversion
	LTC6811_pollAdc();
	for(int i = 0; i < 80000; i++);
	
  int8_t error = LTC6811_rdaux(0x00C, 1, Modules); 	
	
	return Modules[0].aux.a_codes[0];
}
	
	


