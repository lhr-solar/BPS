/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** Temperature.c
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 */
#include "Temperature.h"
#include "os.h"
#include "Tasks.h"
#include "VoltageToTemp.h"
#include "BSP_PLL.h"
#ifdef SIMULATION
#include "Simulator.h"
#endif

// if you change this, you also need to change the calls to median()
#define TEMPERATURE_MEDIAN_FILTER_DEPTH 3

void delay_u(uint16_t micro);

// median filter for temperature values
static int32_t medianFilterIdx = TEMPERATURE_MEDIAN_FILTER_DEPTH - 1;
static int32_t rawTemperatures[NUM_MINIONS][MAX_TEMP_SENSORS_PER_MINION_BOARD][TEMPERATURE_MEDIAN_FILTER_DEPTH];

// Holds the temperatures in Celsius (Fixed Point with .001 resolution) for each sensor on each board
static int32_t temperatures[NUM_MINIONS][MAX_TEMP_SENSORS_PER_MINION_BOARD];

// Holds the maximum measured temperature in the most recent batch of temperature measurements
static int32_t maxTemperature;

// 0 if discharging 1 if charging
static uint8_t ChargingState;

// simulator bypasses ltc driver
#ifndef SIMULATION
// Interface to communicate with LTC6811 (Register values)
// Temperature.c uses auxiliary registers to view ADC data and COM register for I2C with LTC1380 MUX
static cell_asic *Minions;
#endif

#ifdef SIMULATION
// keep track of which temperature channel we are using
uint8_t currentChannel = 0;
#endif

//TODO: tune PID with actual pack and fans, and then change values below to appropiate value
#define PROPORTION -1234
#define INTEGRAL 4321
#define DERIVATIVE -9876
#define MAX_FAN_SPEED 4000
#define DIVISOR 25000
// Variables to help with PID calculation
static int32_t ErrorSum = 0;
static int32_t Error;
static int32_t Rate;
static int32_t PreviousError = 0;


/**
 * @brief find the median of three values
 * 
 * @param a first value
 * @param b second value
 * @param c third value
 * @return the median
 */
static inline int32_t median(int32_t a, int32_t b, int32_t c) {
	if ((a <= b && b < c) || (a >= b && b > c)) return b;
	if ((a < b && a > c) || (b < a && a < c)) return a;
	return c;
}

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 */
void Temperature_Init(cell_asic *boards){
// simulator bypasses ltc driver
#ifndef SIMULATION

	// Record pointer
	Minions = boards;
	OS_ERR err;
	
	// Initialize peripherals
	wakeup_sleep(NUM_MINIONS);
	LTC6811_Init(Minions);

	//take control of mutex
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	// Write Configuration Register
	LTC6811_wrcfg(NUM_MINIONS, Minions);

	// Read Configuration Register
	wakeup_sleep(NUM_MINIONS);
	LTC6811_rdcfg_safe(NUM_MINIONS, Minions);
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
#endif

	// set up the median filter with alternating temperatures of 1000 degrees and 0 degrees
	for (int32_t filterIdx = 0; filterIdx < TEMPERATURE_MEDIAN_FILTER_DEPTH - 1; ++filterIdx) {
		for (int32_t minion = 0; minion < NUM_MINIONS; ++minion) {
			for (int32_t sensor = 0; sensor < MAX_TEMP_SENSORS_PER_MINION_BOARD; ++sensor) {
				rawTemperatures[minion][sensor][filterIdx] = (filterIdx & 0x1) ? 1000000 : 0;
			}
		}
	}
	// median filter should start pointing to the last set of temperature readings
	medianFilterIdx = TEMPERATURE_MEDIAN_FILTER_DEPTH - 1;

}

/** Temperature_ChannelConfig
 * Configures which temperature channel you're sampling from in every board
 * Assumes there are only 2 muxes; 0 index based - 0 is sensor 1
 * @precondition tempChannel should be < MAX_TEMP_SENSORS_PER_MINION_BOARD
 * @param channel number (0-indexed based)
 * @return SUCCESS or ERROR
 * @note we clear the otherMux every time a channel is switched even on the same mux; Maybe change depending on speed/optimization
 */
ErrorStatus Temperature_ChannelConfig(uint8_t tempChannel) {
#ifdef SIMULATION
	currentChannel = tempChannel;
#else
	uint8_t muxAddress;
	uint8_t otherMux;
	
	if (tempChannel >= MAX_TEMP_SENSORS_PER_MINION_BOARD/2) {
		muxAddress = MUX2;
		otherMux = MUX1;
	}
	else {
		muxAddress = MUX1;
		otherMux = MUX2;
	}
	
	// need to make 0 to 7 since each mux only sees those channels
	tempChannel %= MAX_TEMP_SENSORS_PER_MINION_BOARD/2;

	// MESSAGE FORMAT FOR MUX:
	// 8 bytes needed in total
	// First 2 bytes: START_CODE + 4 MSB of Mux Address
	// Second 2 bytes: 4 LSB of Mux Address + END_CODE
	// Third 2 bytes: START_CODE + 4 MSB of data
	// Fourth 2 bytes: 4 LSB of data + END_CODE

	//take control of mutex
	OS_ERR err;
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	
	for (int board = 0; board < NUM_MINIONS; board++) {
		/* Clear other mux */
		
		// Send Address
		Minions[board].com.tx_data[0] = (AUX_I2C_START << 4) + ((otherMux & 0xF0)>>4); 				
		Minions[board].com.tx_data[1] = ((otherMux & 0x0F) << 4) + AUX_I2C_NACK;
		
		// Sends what channel to open. 4th bit is the enable bit, the 4MSB bits will be by default high
		// Turn all channels off
		Minions[board].com.tx_data[2] = (AUX_I2C_BLANK << 4) + 0xF;
		Minions[board].com.tx_data[3] = (0 << 4) + AUX_I2C_NACK_STOP;

		// Rest is no transmit with all data bits set to high, makes sure there's nothing else we're sending
		Minions[board].com.tx_data[4] = (AUX_I2C_NO_TRANSMIT << 4) + 0xF;
		Minions[board].com.tx_data[5] = (0xF << 4) + AUX_I2C_NACK_STOP;
	}

	// Send data
    wakeup_sleep(NUM_MINIONS);
    LTC6811_wrcomm(NUM_MINIONS, Minions);
	delay_u(200);
	LTC6811_stcomm();

	for (int board = 0; board < NUM_MINIONS; board++) {
		/* Open channel on mux */
		
		// Send Address for a particular mux
		Minions[board].com.tx_data[0] = (AUX_I2C_START << 4) + (muxAddress >> 4); 				
		Minions[board].com.tx_data[1] = (muxAddress << 4) + AUX_I2C_NACK;

		// Sends what channel to open. 8 is the enable bit
		// 8 + temp_channel
		Minions[board].com.tx_data[2] = (AUX_I2C_BLANK << 4) + 0xF; 				// set dont cares high
		Minions[board].com.tx_data[3] = ((8 + tempChannel) << 4) + AUX_I2C_NACK_STOP;
			
		// Rest is no transmit with all data bits set to high, makes sure there's nothing else we're sending
		Minions[board].com.tx_data[4] = (AUX_I2C_NO_TRANSMIT << 4) + 0xF;
		Minions[board].com.tx_data[5] = (0xF << 4) + AUX_I2C_NACK_STOP;
    }

	// Send data
    LTC6811_wrcomm(NUM_MINIONS, Minions);
	delay_u(200);
	LTC6811_stcomm();
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
#endif

	return SUCCESS;
}

/** convertVoltageToTemperature
 * Converts mv to temperature based on the temperature sensor equation
 * Equation : T(in C) = (((13.582 - sqrt((-13.582)*(-13.582) + 4 * 0.00433 * (2230.8 - milliVolt)))/ (2.0 * -0.00433)) + 30)
 * @param mV from ADC
 * @return temperature in Celsius (Fixed Point with .001 resolution) 
 */
int32_t milliVoltToCelsius(uint32_t milliVolt){
	if (milliVolt < sizeof(voltToTemp)/sizeof(voltToTemp[0])) {
		return voltToTemp[milliVolt];
	}
	else {
		return TEMP_ERR_OUT_BOUNDS;
	}
}

/** Temperature_UpdateSingleChannel
 * Stores and updates the new measurements received on one particular temperature sensor
 * @param channel < MAX_TEMP_SENSORS_PER_MINION_BOARD, 0-indexed
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_UpdateSingleChannel(uint8_t channel){
	// Configure correct channel
	if (ERROR == Temperature_ChannelConfig(channel)) {
		return ERROR;
	}
	
	// Sample ADC channel
	Temperature_SampleADC(MD_422HZ_1KHZ);

	// update the median filter

	// Convert to Celsius
	for(int board = 0; board < NUM_MINIONS; board++) {
		
		// update adc value from GPIO1 stored in a_codes[0]; 
		// a_codes[0] is fixed point with .001 resolution in volts -> multiply by .001 * 1000 to get mV in double form
#ifndef SIMULATION
		rawTemperatures[board][channel][medianFilterIdx] = milliVoltToCelsius(Minions[board].aux.a_codes[0] / 10);
#else
		if (board * MAX_TEMP_SENSORS_PER_MINION_BOARD + channel < NUM_TEMPERATURE_SENSORS) {
			rawTemperatures[board][channel][medianFilterIdx] = Simulator_getTemperature(board * MAX_TEMP_SENSORS_PER_MINION_BOARD + channel);
		}
#endif
	}

	// increment the median filter index
	medianFilterIdx = (medianFilterIdx + 1) % TEMPERATURE_MEDIAN_FILTER_DEPTH;

	// update the filtered values
	// you need to change this if you change 
	for (int32_t minion = 0; minion < NUM_MINIONS; ++minion) {
		for (int32_t sensor = 0; sensor < MAX_TEMP_SENSORS_PER_MINION_BOARD; ++sensor) {
			temperatures[minion][sensor] = median(
													 rawTemperatures[minion][sensor][0],
													 rawTemperatures[minion][sensor][1],
													 rawTemperatures[minion][sensor][2]
												 );
		}
	}

	return SUCCESS;
}

/** Temperature_UpdateAllMeasurements
 * Stores and updates the new measurements received on all temperature sensors
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_UpdateAllMeasurements(){
	// update all measurements
	for (int sensorCh = 0; sensorCh < MAX_TEMP_SENSORS_PER_MINION_BOARD; sensorCh++) {
		// A hack to solve a timing issue related to enabling one of the muxes
		if(sensorCh % 8 == 0) {
			Temperature_ChannelConfig(sensorCh);
			Temperature_ChannelConfig(sensorCh);
		}

		// Update the measurement for this channel
		Temperature_UpdateSingleChannel(sensorCh);
	}

	// update max temperature
	int32_t newMaxTemperature = temperatures[0][0];
	for (int minion = 0; minion < NUM_MINIONS; ++minion) {
		for (int sensor = 0; sensor < MAX_TEMP_SENSORS_PER_MINION_BOARD; ++sensor) {
			// ignore parts of the array that are out of bounds
			if (minion * MAX_TEMP_SENSORS_PER_MINION_BOARD + sensor >= NUM_TEMPERATURE_SENSORS) {
				break;
			}
			if (temperatures[minion][sensor] > newMaxTemperature) {
				newMaxTemperature = temperatures[minion][sensor];
			}
		}
	}
	maxTemperature = newMaxTemperature;
	return SUCCESS;
}

/** Temperature_CheckStatus
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SAFE or DANGER
 */
SafetyStatus Temperature_CheckStatus(uint8_t isCharging){
	int32_t temperatureLimit = isCharging == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;

	for (int i = 0; i < NUM_MINIONS; i++) {
		for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
			if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j >= NUM_TEMPERATURE_SENSORS) break;
			if ((temperatures[i][j] > temperatureLimit) || (temperatures[i][j] == TEMP_ERR_OUT_BOUNDS)) {
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
	int32_t temperatureLimit = ChargingState == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;

	for (int i = 0; i < NUM_MINIONS-1; i++) {
		for (int j = 0; j < MAX_TEMP_SENSORS_PER_MINION_BOARD; j++) {
			if (i * MAX_TEMP_SENSORS_PER_MINION_BOARD + j >= NUM_TEMPERATURE_SENSORS) break;
			if (temperatures[i][j] > temperatureLimit) {
				ModuleTempStatus[(i * (NUM_TEMP_SENSORS_PER_MOD/2)) + (j % (NUM_TEMP_SENSORS_PER_MOD/2))] = 1;
			}
		}
	}
	return ModuleTempStatus;
}
/** Temperature_GetSingleTempSensor
 * Gets the single sensor from a particular board
 * @precondition: board must be < NUM_MINIONS, sensorIdx < MAX_TEMP_SENSORS_PER_MINION_BOARD
 * @param index of board (0-indexed based)
 * @param index of sensor (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int32_t Temperature_GetSingleTempSensor(uint8_t board, uint8_t sensorIdx) {
	return temperatures[board][sensorIdx];
}

/** Temperature_GetModuleTemperature
 * Gets the avg temperature of a certain battery module in the battery pack. Since there
 * are 2 sensors per module, the return value is the average
 * @precondition: moduleIdx must be < NUM_BATTERY_MODULE
 * @param index of module (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int32_t Temperature_GetModuleTemperature(uint8_t moduleIdx){
	int32_t total = 0;
	uint8_t board = (moduleIdx * 2) / MAX_TEMP_SENSORS_PER_MINION_BOARD;
	uint8_t sensor = moduleIdx % (MAX_TEMP_SENSORS_PER_MINION_BOARD / 2);

	total += temperatures[board][sensor];
	// Get temperature from other sensor on other side
	total += temperatures[board][sensor + MAX_TEMP_SENSORS_PER_MINION_BOARD / 2];
	total /= 2;
	return total;
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int32_t Temperature_GetTotalPackAvgTemperature(void){
	int32_t total = 0;
	for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
		total += Temperature_GetModuleTemperature(i);
	}
	return total /= NUM_BATTERY_MODULES;
}

/** Temperature_SampleADC
 * Starts ADC conversion on GPIO1 on LTC6811's auxiliary registers on all boards
 * NOTE: May need to call wakeup_sleep before this function.
 * @param sets the sampling rate
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_SampleADC(uint8_t ADCMode) {
#ifndef SIMULATION
	//take control of mutex
	OS_ERR err;
  	OSMutexPend(&MinionsASIC_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
  	assertOSError(err);
	
	LTC6811_adax(ADCMode, AUX_CH_GPIO1);							// Start ADC conversion on GPIO1
	LTC6811_pollAdc();
	
	int8_t error = LTC6811_rdaux(AUX_CH_GPIO1, NUM_MINIONS, Minions);   // Update Minions with fresh values
	//release mutex
  	OSMutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE, &err);
  	assertOSError(err);
	return error != -1 ? SUCCESS : ERROR;
#else
	return SUCCESS;
#endif
}

/** Temperature_GetMaxTemperature
 * Gets the maximum measured temperature in the most recent batch of temperature measurements
 * @return the maximum measured temperature in the most recent batch of temperature measurements
 */
int32_t Temperature_GetMaxTemperature(void) {
	return maxTemperature;
}

/**
 * @brief Gives fan speed based on Average temperature of pack and past error values
 * @param InputTemp - current temperature
 * @param DesiredTemp - desired temperature
 * @return FanSpeed: 0-4000 PWM
 */
int32_t Temperature_PID_Output(int32_t InputTemp, int32_t DesiredTemp) {
    Error = DesiredTemp - InputTemp;
    ErrorSum = ErrorSum + Error;

    if (PreviousError == 0) {PreviousError = Error;} //init previous val first time

    Rate = Error - PreviousError;
    PreviousError = Error;     //updates previous err value

    if (((PROPORTION*(Error) + INTEGRAL*(ErrorSum) + DERIVATIVE*(Rate))/DIVISOR) > MAX_FAN_SPEED) {
        return MAX_FAN_SPEED;
    }
    if (((PROPORTION*(Error) + INTEGRAL*(ErrorSum) + DERIVATIVE*(Rate))/DIVISOR) <= 0) {
        return 0;
    }
    return (PROPORTION*(Error) + INTEGRAL*(ErrorSum) + DERIVATIVE*(Rate))/DIVISOR;
}
