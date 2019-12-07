/** Temperature.c
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 */
#include "Temperature.h"

// Holds the temperatures in Celsius for each sensor on each board
int16_t ModuleTemperatures[NUM_MINIONS][MAX_TEMP_SENSORS_PER_MINION_BOARD];

// 0 if discharging 1 if charging
static uint8_t ChargingState;

// Interface to communicate with LTC6811 (Register values)
// Temperature.c uses auxiliary registers to view ADC data and COM register for I2C with LTC1380 MUX
static cell_asic *Minions;

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 */
ErrorStatus Temperature_Init(cell_asic *boards){
	// Record pointer
	Minions = boards;

	// Initialize peripherals
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(Minions);

	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, Minions);

	// Read Configuration Register
	wakeup_sleep(NUM_MINIONS);
	int8_t error = LTC6811_rdcfg(NUM_MINIONS, Minions);

	return error == 0 ? SUCCESS : ERROR;
}

/** Temperature_ChannelConfig
 * Configures which temperature channel you're sampling from
 * Assumes there are only 2 muxes; 0 index based - 0 is sensor 1
 * @precondition tempChannel should be < MAX_TEMP_SENSORS_PER_MINION_BOARD
 * @param channel number (0-indexed based)
 * @return SUCCESS or ERROR
 * @note we clear the otherMux every time a channel is switched even on the same mux; Maybe change depending on speed/optimization
 */
ErrorStatus Temperature_ChannelConfig(uint8_t tempChannel) {
	uint8_t muxAddress;
	uint8_t otherMux;
	
	tempChannel %= MAX_TEMP_SENSORS_PER_MINION_BOARD/2;
	
	if (tempChannel > 7) {
		muxAddress = MUX2;
		otherMux = MUX1;
	}
	else {
		muxAddress = MUX1;
		otherMux = MUX2;
	}

	// 0110 is start code, must be the 4 msb's then the 4 msb's of the data (addr of mux)
	// remaind of otherMux, then ACK/NACK/STOP from master/slave
	
	for (int board = 0; board < NUM_MINIONS; board++) {
		/* Clear other mux */
		Minions[board].com.tx_data[0] = (AUX_I2C_START << 4) + ((otherMux & 0xF0)>>4); 				
		Minions[board].com.tx_data[1] = ((otherMux & 0x0F) << 4) + AUX_I2C_NACK /*AUX_I2C_SLAVE_ACK*/;								// Acknowledge signal at 9th cycle


		// Sends what channel to open. 4th bit is the enable bit
		// Turn all channels off
		Minions[board].com.tx_data[2] = (AUX_I2C_BLANK << 4) + 0xF; //AUX_I2C_BLANK;
		Minions[board].com.tx_data[3] = /*AUX_I2C_BLANK*/ (0 << 4) + AUX_I2C_NACK_STOP;

		// Rest is no transmit with all data bits set to high, makes sure there's nothing else we're sending
		Minions[board].com.tx_data[4] = (AUX_I2C_NO_TRANSMIT << 4) + 0xF;
		Minions[board].com.tx_data[5] = (0xF << 4) + AUX_I2C_NACK_STOP;

		wakeup_sleep(NUM_MINIONS);
		LTC6811_wrcomm(NUM_MINIONS, Minions);
		LTC6811_stcomm();
			
		// Send Address for a particular mux
		Minions[board].com.tx_data[0] = (AUX_I2C_START << 4) + (muxAddress >> 4); 				
		Minions[board].com.tx_data[1] = (muxAddress << 4) + AUX_I2C_NACK; //AUX_I2C_SLAVE_ACK;								// slave Acknowledge signal at 9th cycle


		
		// Sends what channel to open. 8 is the enable bit
		// 8 + temp_channel
		Minions[board].com.tx_data[2] = (AUX_I2C_BLANK << 4) + 0xF; 							// set dont cares high
		Minions[board].com.tx_data[3] = ((8 + tempChannel) << 4) + AUX_I2C_NACK_STOP; //AUX_I2C_SLAVE_ACK_STOP;				/// Slave acks, master generates stop sig
			
		// Rest is no transmit with all data bits set to high, makes sure there's nothing else we're sending
		Minions[board].com.tx_data[4] = (AUX_I2C_NO_TRANSMIT << 4) + 0xF;
		Minions[board].com.tx_data[5] = (0xF << 4) + AUX_I2C_NACK_STOP;

		wakeup_sleep(NUM_MINIONS);
		LTC6811_wrcomm(NUM_MINIONS, Minions);
		LTC6811_stcomm();
	}

	return SUCCESS;
}

/** convertVoltageToTemperature
 * Converts mv to temperature based on the temperature sensor equation
 * Equation : T(in C) = (((13.582 - sqrt((-13.582)*(-13.582) + 4 * 0.00433 * (2230.8 - milliVolt)))/ (2.0 * -0.00433)) + 30)
 * @param mV from ADC
 * @return temperature in Celsius (Fixed Point with .001 resolution) 
 */
int milliVoltToCelsius(float milliVolt){
	// Typecasting to get rid of warnings
	float sumInRt = (float)(-13.582)*(float)(-13.582) + (float)4.0 * (float)0.00433 * ((float)2230.8 - milliVolt);
	float rt = sqrt(sumInRt);				
	float numerator = (float)13.582 - rt;
	float denom = (2.0 * -0.00433);		
	float frac = numerator/denom;			
	float retVal = frac + 30;			
	
	return retVal * 1000;
}

/** Temperature_UpdateMeasurements
 * Stores and updates the new measurements received on all temperature sensors
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_UpdateMeasurements(){
	for (int sensorCh = 0; sensorCh < MAX_TEMP_SENSORS_PER_MINION_BOARD; sensorCh++) {
		if (ERROR == Temperature_ChannelConfig(sensorCh)) {
			return ERROR;
		}
		Temperature_GetRawADC(MD_422HZ_1KHZ);
		for(int board = 0; board < NUM_MINIONS; board++) {
			
			// update adc value from GPIO1 stored in a_codes[0]; 
			// a_codes[0] is fixed point with .001 resolution in volts -> multiply by .001 * 1000 to get mV in double form
			ModuleTemperatures[board][sensorCh] = milliVoltToCelsius(Minions[board].aux.a_codes[0]*0.1);
		}
	}
	return SUCCESS;
}

/** Temperature_IsSafe
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SAFE or DANGER
 */
SafetyStatus Temperature_IsSafe(uint8_t isCharging){
	int16_t temperatureLimit = isCharging == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;
	temperatureLimit *= MILLI_SCALING_FACTOR;

	for (int i = 0; i < NUM_MINIONS; i++) {
		for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
			if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j >= NUM_TEMPERATURE_SENSORS) break;
			if (ModuleTemperatures[i][j] > temperatureLimit) {
				return DANGER;
			}
		}
	}
	return SAFE;
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
	static uint8_t ModuleTempStatus[NUM_BATTERY_MODULES];
	int16_t temperatureLimit = ChargingState == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;

	for (int i = 0; i < NUM_MINIONS-1; i++) {
		for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
			if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j >= NUM_TEMPERATURE_SENSORS) break;
			if (ModuleTemperatures[i][j] > temperatureLimit) {
				ModuleTempStatus[(i * (NUM_TEMP_SENSORS_PER_MOD/2)) + (j % (NUM_TEMP_SENSORS_PER_MOD/2))] = 1;
			}
		}
	}
	return ModuleTempStatus;
}

/** Temperature_GetModuleTemperature
 * Gets the avg temperature of a certain battery module in the battery pack. Since there
 * are 2 sensors per module, the return value is the average
 * @precondition: moduleIdx must be < NUM_BATTERY_MODULE
 * @param index of board (0-indexed based)
 * @param index of battery (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int16_t Temperature_GetModuleTemperature(uint8_t moduleIdx){
	int32_t total = 0;
	uint8_t board = (moduleIdx * 2) / MAX_TEMP_SENSORS_PER_MINION_BOARD;
	uint8_t sensor = moduleIdx % (MAX_TEMP_SENSORS_PER_MINION_BOARD / 2);
	
	total += ModuleTemperatures[board][sensor];
	
	// Get temperature from other sensor on other side
	total += ModuleTemperatures[board][sensor + MAX_TEMP_SENSORS_PER_MINION_BOARD / 2];
	total /= 2;
	return total;
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int16_t Temperature_GetTotalPackAvgTemperature(void){
	int32_t total = 0;
	for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
		total += Temperature_GetModuleTemperature(i);
	}
	return total /= NUM_BATTERY_MODULES;
}

/** Temperature_GetRawADC
 * Starts ADC conversion on GPIO1 on LTC6811's auxiliary registers on all boards
 * @param sets the sampling rate
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_GetRawADC(uint8_t ADCMode) {
	wakeup_sleep(NUM_MINIONS);
	LTC6811_adax(ADCMode, AUX_CH_GPIO1);							// Start ADC conversion on GPIO1
	LTC6811_pollAdc();

	wakeup_sleep(NUM_MINIONS);
	int8_t error = LTC6811_rdaux(AUX_CH_GPIO1, NUM_MINIONS, Minions);   // Update Minions with fresh values
	return error != -1 ? SUCCESS : ERROR;
}
