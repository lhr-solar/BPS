/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** Temperature.c
 * Temperature class that holds all temperature related information of BeVolt's
 * battery pack.
 */
#include "Temperature.h"
#include "Tasks.h"
#include "VoltageToTemp.h"
#include "BSP_PLL.h"
#include "Fans.h"
#ifdef SIMULATION
#include "Simulator.h"
#endif

// median filter
#define MEDIAN_FILTER_TYPE int32_t
#define MEDIAN_FILTER_DEPTH 3
#define MEDIAN_FILTER_CHANNELS MAX_TEMP_SENSORS
#define MEDIAN_FILTER_NAME TemperatureFilter
#include "MedianFilter.h"
static TemperatureFilter_t TemperatureFilter;

// simulator bypasses ltc driver
#ifndef SIMULATION
// Interface to communicate with LTC6811 (Register values)
// Temperature.c uses auxiliary registers to view ADC data and COM register for I2C with LTC1380 MUX
static cell_asic *Minions;
#endif

// Holds the temperatures in Celsius (Fixed Point with .001 resolution) for each sensor on each board
static int32_t Temperatures[NUM_TEMPERATURE_SENSORS];

// Raw temperature values from the ADC -- this is a sparse array and will be packed down later.
static int32_t rawTemperatures[MAX_TEMP_SENSORS];

// Holds the maximum measured temperature in the most recent batch of temperature measurements
static int32_t maxTemperature;

// Holds the minimum measured temperature in the most recent batch of temperature measurements
static int32_t minTemperature;

// Holds the average pack temperature in the most recent batch of temperature measurements
static int32_t avgTemperature;

// 0 if discharging 1 if charging
static uint8_t ChargingState;

#ifdef SIMULATION
// keep track of which temperature channel we are using
static uint8_t currentChannel = 0;
#endif

/** Temperature_Init
 * Initializes device drivers including SPI inside LTC6811_init and LTC6811 for Temperature Monitoring
 * @param boards LTC6811 data structure that contains the values of each register
 */
void Temperature_Init(cell_asic *boards){
// simulator bypasses ltc driver
#ifndef SIMULATION
    // Record pointer
    Minions = boards;
    wakeup_sleep(NUM_MINIONS);
    LTC6811_Init(Minions); // Initialize peripherals

    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    LTC6811_wrcfg(NUM_MINIONS, Minions); // Write Configuration Register
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
    
    wakeup_sleep(NUM_MINIONS);

    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    LTC6811_rdcfg_safe(NUM_MINIONS, Minions); // Read Configuration Register
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
    
#endif
    // set up the median filter with alternating temperatures of 1000 degrees and 0 degrees
    TemperatureFilter_init(&TemperatureFilter, 0, 1000000);
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
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    
    for (uint8_t board = 0; board < NUM_MINIONS; board++) {
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
    RTOS_BPS_DelayUs(200);
    LTC6811_stcomm();

    for (uint8_t board = 0; board < NUM_MINIONS; board++) {
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
    RTOS_BPS_DelayUs(200);
    LTC6811_stcomm();
    //release mutex
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);

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
    else if (milliVolt < 5200 && milliVolt > 4800) { //temperature sensor is disconnected for scrutineering
        return 0; //safe value
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

#ifndef SIMULATION
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
#endif

    // Convert to Celsius
    for (uint8_t board = 0; board < NUM_MINIONS; board++) {
        // update adc value from GPIO1 stored in a_codes[0]; 
        // a_codes[0] is fixed point with .001 resolution in volts -> multiply by .001 * 1000 to get mV in double form
        uint8_t sensor_idx = (board * MAX_TEMP_SENSORS_PER_MINION_BOARD) + channel;
        if (channel < TemperatureSensorsCfg[board]) {   // don't touch unused sensors
#ifndef SIMULATION
            rawTemperatures[sensor_idx] = milliVoltToCelsius(Minions[board].aux.a_codes[0] / 10);
#else
            // simulator expects the actual number of physical sensors (and not just mux channels on the PCB)
            // therefore, we have to do some index crunching -- this is very inefficient but it's just for 
            // simulator so it's ok.
            uint8_t empty_sensors_so_far = 0;
            for (uint8_t i = 0; i < board; i++) {
                empty_sensors_so_far += MAX_TEMP_SENSORS_PER_MINION_BOARD - TemperatureSensorsCfg[i];
            }
            uint8_t simulator_idx = sensor_idx - empty_sensors_so_far;
            
            // populate nonvalid sensors with 0
            rawTemperatures[sensor_idx] = (channel < TemperatureSensorsCfg[board]) ? Simulator_getTemperature(simulator_idx) : 0;
#endif
        }
    }
#ifndef SIMULATION
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
#endif
    
    // run median filter
    TemperatureFilter_put(&TemperatureFilter, rawTemperatures);

    return SUCCESS;
}

/** Temperature_UpdateAllMeasurements
 * Stores and updates the new measurements received on all temperature sensors
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_UpdateAllMeasurements(){
    // update all measurements
    for (uint8_t sensorCh = 0; sensorCh < MAX_TEMP_SENSORS_PER_MINION_BOARD; sensorCh++) {
        // A hack to solve a timing issue related to enabling one of the muxes
        if (sensorCh % 8 == 0) {
            Temperature_ChannelConfig(sensorCh);
            Temperature_ChannelConfig(sensorCh);
        }
        // Update the measurement for this channel
        Temperature_UpdateSingleChannel(sensorCh);
    }

    // update public temperatures with output of median filter
    int32_t filteredTemperatures[MAX_TEMP_SENSORS];
    TemperatureFilter_get(&TemperatureFilter, filteredTemperatures);

    // package raw voltage values into single array
    for (uint8_t minion = 0, sensor = 0; minion < NUM_MINIONS; minion++){
        for (uint8_t channel = 0; channel < TemperatureSensorsCfg[minion]; channel++) {
            uint8_t sensor_idx = (minion * MAX_TEMP_SENSORS_PER_MINION_BOARD) + channel;
            Temperatures[sensor++] = filteredTemperatures[sensor_idx];
        }
    }

    // update min/max/avg temperature
    int32_t running_max = 0, running_min = INT32_MAX, running_total = 0;
    for (uint8_t i = 0; i < NUM_TEMPERATURE_SENSORS; i++) {
        running_max = MAX(running_max, Temperatures[i]);
        running_min = MIN(running_min, Temperatures[i]);
        running_total += Temperatures[i];
    }

    maxTemperature = running_max;
    minTemperature = running_min;
    avgTemperature = running_total / NUM_TEMPERATURE_SENSORS;

    return SUCCESS;
}

/** Temperature_CheckStatus
 * Checks if all modules are safe
 * @param 1 if pack is charging, 0 if discharging
 * @return SAFE, opt = CHARGE_DISABLE / OPT_SAFE
 * @return DANGER, opt = UNDERVOLTAGE / OVERVOLTAGE
 */
SafetyStatus Temperature_CheckStatus(uint8_t isCharging, SafetyStatusOpt *opt){
    SafetyStatusOpt tmp;
    if (opt == NULL) opt = &tmp;
    int32_t temperatureLimit = isCharging ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;

    bool charge_enable = true;
    for (uint8_t i = 0; i < NUM_TEMPERATURE_SENSORS; i++) {
        if ((Temperatures[i] > temperatureLimit) || (Temperatures[i] == TEMP_ERR_OUT_BOUNDS)) {
            *opt = OPT_NONE;
            return DANGER;
        } else if (Temperatures[i] > CHARGE_DISABLE_TEMPERATURE) {
            charge_enable = false;
        }
    }

    *opt = charge_enable ? OPT_SAFE : CHARGE_DISABLE;
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
    static uint8_t ModuleTempStatus[NUM_TEMPERATURE_SENSORS];
    int32_t temperatureLimit = ChargingState == 1 ? MAX_CHARGE_TEMPERATURE_LIMIT : MAX_DISCHARGE_TEMPERATURE_LIMIT;

    for (uint8_t i = 0; i < NUM_TEMPERATURE_SENSORS; i++) {
        if (Temperatures[i] > temperatureLimit) {
            ModuleTempStatus[i] = 1;
        }
    }

    return ModuleTempStatus;
}
/** Temperature_GetSingleTempSensor
 * Gets the single sensor from a particular board
 * @param index of sensor (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int32_t Temperature_GetSingleTempSensor(uint8_t sensorIdx) {
    return Temperatures[sensorIdx];
}

/** Temperature_GetModuleTemperature
 * Gets the avg temperature of a certain battery module in the battery pack. 
 * There is currently (2023-2024) one temperature per module, so this function does nothing.
 * @precondition: moduleIdx must be < NUM_BATTERY_MODULE
 * @param index of module (0-indexed based)
 * @return temperature of the battery module at specified index
 */
int32_t Temperature_GetModuleTemperature(uint8_t moduleIdx) {
    if (moduleIdx >= NUM_BATTERY_MODULES) {
        return INT32_MAX;
    }
    return Temperature_GetSingleTempSensor(moduleIdx);
}

/** Temperature_SampleADC
 * Starts ADC conversion on GPIO1 on LTC6811's auxiliary registers on all boards
 * NOTE: May need to call wakeup_sleep before this function.
 * @param sets the sampling rate
 * @return SUCCESS or ERROR
 */
ErrorStatus Temperature_SampleADC(uint8_t ADCMode) {
#ifndef SIMULATION    
    LTC6811_adax(ADCMode, AUX_CH_GPIO1); // Start ADC conversion on GPIO1
    LTC6811_pollAdc(); //wait till adc is done returns time taken

    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    int8_t error = LTC6811_rdaux(AUX_CH_GPIO1, NUM_MINIONS, Minions); // Update Minions with fresh values
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);\
    
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

/** Temperature_GetMinTemperature
 * Gets the minimum measured temperature in the most recent batch of temperature measurements
 * @return the minimum measured temperature in the most recent batch of temperature measurements
 */
int32_t Temperature_GetMinTemperature(void) {
    return minTemperature;
}

/** Temperature_GetTotalPackAvgTemperature
 * Gets the average temperature of the whole battery pack
 * @return average temperature of battery pack
 */
int32_t Temperature_GetTotalPackAvgTemperature(void){
    return avgTemperature;
}
