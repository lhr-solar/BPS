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


int16_t ModuleTemperatures[NUM_MINIONS][NUM_TEMP_SENSORS_ON_MINION];	// Holds the fixed point .001 resolution temperatures for each sensor on each board

uint8_t ModuleTempStatus[NUM_MINIONS];				// Used to check and return if a particular module is in danger.

uint8_t ChargingState;							// 0 if discharging 1 if charging

cell_asic TemperatureModule[NUM_MINIONS];                  // cell for Temperature module. Temporary until refactoring happens with voltage.c

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 */
ErrorStatus Temperature_Init(void){
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(TemperatureModule);

	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, TemperatureModule);

	// Read Configuration Register
	wakeup_sleep(NUM_MINIONS);
	int8_t error = LTC6811_rdcfg(NUM_MINIONS, TemperatureModule);

	return error == 0 ? SUCCESS : ERROR;
}

/** Temperature_ChannelConfig
 * Configures which temperature channel you're sampling from
 * Assumes there are only 2 muxes; 0 index based - 0 is sensor 1
 * @param channel number
 * @return SUCCESS or ERROR
 * CONCERNS:
 * is cell_asic temp necessary or should we just use the global cell_asic to send data?
 * For Later: we clear the otherMux every time a channel is switched even on the same mux; Maybe change depending on speed/optimization
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

	/* Clear other mux */
	temp.com.tx_data[0] = (AUX_I2C_START << 4) + ((otherMux & 0xF0)>>4); 				// 0110 START CODE
	temp.com.tx_data[1] = ((otherMux & 0x0F) << 4) + AUX_I2C_ACK;								// Acknowledge signal at 9th cycle


	// Sends what channel to open. 4th bit is the enable bit
	// Turn all channels off
	temp.com.tx_data[2] = (AUX_I2C_START << 4) + (0);
	temp.com.tx_data[3] = ((0) << 4) + AUX_I2C_NACK_STOP;

	LTC6811_wrcomm(1, &temp);
	LTC6811_stcomm();


	// Send Address for a particular mux
	temp.com.tx_data[0] = (AUX_I2C_START << 4) + ((muxAddress & 0xF0)>>4); 				// 0110 START CODE
	temp.com.tx_data[1] = ((muxAddress & 0x0F) << 4) + AUX_I2C_ACK;								// Acknowledge signal at 9th cycle


	// Sends what channel to open. 8 is the enable bit
	// 8 + temp_channel
	temp.com.tx_data[2] = (AUX_I2C_START << 4) + (0);
	temp.com.tx_data[3] = ((8 + tempChannel) << 4) + AUX_I2C_NACK_STOP;


	LTC6811_wrcomm(1, &temp);
	LTC6811_stcomm();

	return SUCCESS;
}

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new temperature measurements
 * @return SUCCESS or ERROR
 * CONCERNS:
 * What are we checking for error exactly?
 */

SafetyStatus Temperature_UpdateMeasurements(){
	for (int sensorCh = 0; sensorCh < NUM_TEMP_SENSORS_ON_MINION; sensorCh++) {
		Temperature_ChannelConfig(sensorCh);
		Temperature_GetRawADC(MD_422HZ_1KHZ);
		for(int board = 0; board < NUM_MINIONS; board++) {
			ModuleTemperatures[board][sensorCh] = TemperatureModule[board].aux.a_codes[0]; // update adc value from GPIO1 stored in a_codes[0]
		}
	}
	return SAFE;
}

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SUCCESS or ERROR
 */
SafetyStatus Temperature_IsSafe(uint8_t isCharging){
	bool dangerFlag = false;
	int16_t maxCharge = isCharging == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;

	for (int i = 0; i < NUM_MINIONS; i++) {
		for (int j = 0; j < NUM_TEMP_SENSORS_ON_MINION; j++) {
			if (ModuleTemperatures[i][j] > maxCharge) {
				ModuleTempStatus[i] = 1;
				dangerFlag = true;
				// break;  // should we break immediately or check all the sensors first?
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
void Temperature_SetChargeState(uint8_t isCharging){
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
 * are NUM_TEMP_SENSORS_ON_MINION sensors per module, just average all of the sensors
 * for that module so each module only has one temperature reading
 * @param index of module
 * @return temperature of module at specified index
 */
int16_t Temperature_GetModuleTemperature(uint16_t moduleIdx){
	// assumes if it wants the first board, then moduleIdx = 0;
	if (moduleIdx >= NUM_MINIONS) return -1;

	int32_t total = 0;
	for (int i = 0; i < NUM_TEMP_SENSORS_ON_MINION; i++) {
		total += ModuleTemperatures[moduleIdx][i];
	}
	total /= NUM_TEMP_SENSORS_ON_MINION;
	return total;
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int16_t Temperature_GetTotalPackAvgTemperature(void){
	int32_t total = 0;
	for (int i = 0; i < NUM_MINIONS; i++) {
		total += Temperature_GetModuleTemperature(i);
	}
	total /= NUM_MINIONS;
	return total;
}

/** Temperature_GetRawADC
 * Starts ADC conversion on GPIO1 on LTC6811's auxiliary registers and returns ADC value
 * @param sets the sampling rate
 * @return raw adc value from GPIO1 from one of the 16 temperature sensors
 */
ErrorStatus Temperature_GetRawADC(uint8_t ADCMode) {
	wakeup_sleep(NUM_MINIONS);
	LTC6811_adax(ADCMode, AUX_CH_GPIO1);							// Start ADC conversion on GPIO1
	LTC6811_pollAdc();

	wakeup_sleep(NUM_MINIONS);
	int8_t error = LTC6811_rdaux(AUX_CH_GPIO1, NUM_MINIONS, TemperatureModule);   // Update TemperatureModule with fresh values
	return error != -1 ? SUCCESS : ERROR;
}
