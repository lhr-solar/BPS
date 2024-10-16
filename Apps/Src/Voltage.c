/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/** Voltage.c
 * Voltage wrapper that holds all voltage related information of BeVolt's
 * battery pack.
 */

#include "Voltage.h"
#include "config.h"
#include <stdlib.h>
#include "Tasks.h"
#include "Amps.h"
#ifdef SIMULATION
#include "Simulator.h"
#else
#include "LTC6811.h"
#endif

// median filter
#define MEDIAN_FILTER_TYPE uint16_t
#define MEDIAN_FILTER_DEPTH 3
#define MEDIAN_FILTER_CHANNELS NUM_BATTERY_MODULES
#define MEDIAN_FILTER_NAME VoltageFilter
#include "MedianFilter.h"
static VoltageFilter_t VoltageFilter;

static cell_asic *Minions;

static OS_MUTEX Voltage_Mutex;
static uint16_t Voltages[NUM_BATTERY_MODULES]; // Voltage values gathered, in units of 0.1 mV
static uint32_t openWires[MAX_TEMP_SENSORS];

static uint32_t minVoltage_mv = UINT32_MAX;
static uint32_t maxVoltage_mv = 0;
static uint32_t packVoltage_mv = 0;

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
void Voltage_Init(cell_asic *boards){
    // Record pointer
    Minions = boards;

    //initialize mutex
    RTOS_BPS_MutexCreate(&Voltage_Mutex, "Voltage Buffer Mutex");
                    
// simulator bypasses ltc driver
#ifndef SIMULATION
    wakeup_sleep(NUM_MINIONS);
    LTC6811_Init(Minions);
    
    //take control of mutex
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    // Write Configuration Register
    LTC6811_wrcfg(NUM_MINIONS, Minions);
    //release mutex
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);

    wakeup_sleep(NUM_MINIONS);
    // take control of mutex
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    // Read Configuration Register
    LTC6811_rdcfg_safe(NUM_MINIONS, Minions);
    // release mutex
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
#endif
    // Initialize median filter. There should be no modules with less than 0 volts or more than 5 volts
    VoltageFilter_init(&VoltageFilter, 0, 50000);
}


/** Voltage_UpdateMeasurements
 * Stores and updates the new measurements received
 * @param pointer to new voltage measurements
 */
void Voltage_UpdateMeasurements(void){
    uint16_t rawVoltages[NUM_BATTERY_MODULES];
#ifndef SIMULATION
    // Start Cell ADC Measurements
    wakeup_sleep(NUM_MINIONS);
    LTC6811_adcv(ADC_CONVERSION_MODE,ADC_DCP,CELL_CH_TO_CONVERT);
    LTC6811_pollAdc();	// In case you want to time the length of the conversion time
    
    // Read Cell Voltage Registers
    //take control of mutex
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    LTC6811_rdcv_safe(0, NUM_MINIONS, Minions); // Set to read back all cell voltage registers

    // package raw voltage values into single array
    for (uint8_t minion = 0, module = 0; minion < NUM_MINIONS; minion++){
        for (uint8_t tap = 0; tap < VoltageSensorsCfg[minion]; tap++) {
            rawVoltages[module++] = Minions[minion].cells.c_codes[tap];
        }
    }

    // release minions asic mutex
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
#else
    // package raw voltage values into single array
    for(uint8_t i = 0; i < NUM_BATTERY_MODULES; i++){
        rawVoltages[i] = Simulator_getVoltage(i);
    }
#endif
    // run median filter
    VoltageFilter_put(&VoltageFilter, rawVoltages);

    // update public voltage values
    RTOS_BPS_MutexPend(&Voltage_Mutex, OS_OPT_PEND_BLOCKING);

    VoltageFilter_get(&VoltageFilter, Voltages);

    // calculate min, max, and pack voltage
    uint32_t maxv = 0, minv = UINT32_MAX, totalv = 0;
    for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
        uint32_t voltage = Voltages[i];
        totalv += voltage;
        if (voltage > maxv) maxv = voltage;
        if (voltage < minv) minv = voltage;
    }

    RTOS_BPS_MutexPost(&Voltage_Mutex, OS_OPT_POST_NONE);

    // update global variables
    minVoltage_mv = minv / 10;  // convert to millivolts
    maxVoltage_mv = maxv / 10;
    packVoltage_mv = totalv / 10;
}

/** Voltage_CheckStatus
 * Checks if all battery modules are safe
 * @return SAFE, opt = CHARGE_DISABLE / OPT_SAFE
 * @return DANGER, opt = UNDERVOLTAGE / OVERVOLTAGE
 */
SafetyStatus Voltage_CheckStatus(SafetyStatusOpt* opt){
    SafetyStatusOpt tmp;
    if (opt == NULL) opt = &tmp;
    
    bool charge_enable = true;
    for(int i = 0; i < NUM_BATTERY_MODULES; i++){
        uint16_t voltage = Voltage_GetModuleMillivoltage(i);
        // VOLTAGE_LIMITS in integer millivolts. The LTC6811 sends the voltage data
        // as unsigned 16-bit fixed point integers with a resolution of 0.00001
        if (voltage > MAX_VOLTAGE_LIMIT) {
            *opt = OVERVOLTAGE;
            return DANGER;
        } else if (voltage > CHARGE_DISABLE_VOLTAGE) {
            charge_enable = false;
        } else if ((voltage < MIN_VOLTAGE_LIMIT)
                || (Amps_IsCharging() && (voltage < MIN_VOLTAGE_CHARGING_LIMIT))) {
            *opt = UNDERVOLTAGE;
            return DANGER;
        }
    }
    *opt = charge_enable ? OPT_SAFE : CHARGE_DISABLE;
    return SAFE;
}

/** Voltage_GetModulesInDanger
 * Finds all modules that in danger and stores them into a list.
 * Each module corresponds to an index of the array of SafetyStatus
 * @return pointer to index of modules that are in danger
 */
void Voltage_GetModulesInDanger(VoltageSafety_t* system){
// no simulator support for open wires
#ifndef SIMULATION
    uint32_t wires;
    uint32_t openWireIdx = 0;
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    //put all the bits from each minion's system_open_wire variable into one variable
    for(int k = 0; k < NUM_MINIONS; k++){
        // TO DO: change this to be based on VOLT_TAP_DIST array

        /*
        This section of code assumes a specific distribution of modules that we don't have anymore
        (don't use)
        */
        wires = (Minions[k].system_open_wire & 0x1FF);	//there are at most 8 modules per IC, bit 0 is GND
        for(int s = 0; s <= MAX_VOLT_SENSORS_PER_MINION_BOARD; s++){
            if((k == NUM_MINIONS - 1) && (s == MAX_VOLT_SENSORS_PER_MINION_BOARD)){
                break;	//the last IC has only 7 modules 
            }
            openWires[openWireIdx] = (wires >> s) & 1;
            openWireIdx++;
        }
    }
#endif
    
    for (int i = 0; i < MAX_VOLT_WIRES; i++) {	
        if(i < NUM_BATTERY_MODULES){
            // Check if battery is in range of voltage limit
            if(Voltage_GetModuleMillivoltage(i) > MAX_VOLTAGE_LIMIT) {
                system->module_checks[i] = DANGER;
            }
            else if(Voltage_GetModuleMillivoltage(i) < MIN_VOLTAGE_LIMIT){
                system->module_checks[i] = DANGER;
            }
            else system->module_checks[i] = SAFE;
        }
        if(openWires[i] == 1) {
            system->wire_checks[i] = DANGER;
        } else {
            system->wire_checks[i] = SAFE;
        }
    }
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
}

// no simulator support for open wire checks (yet)
#ifndef SIMULATION
/** Voltage_OpenWireSummary
 * Runs the open wire method with print=true
 */
void Voltage_OpenWireSummary(void){
    wakeup_idle(NUM_MINIONS);
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    LTC6811_run_openwire_multi(NUM_MINIONS, Minions, true);
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
}

/** Voltage_OpenWire
 * Uses the LTC6811_run_openwire_multi() function to check for open wires
 * @return SafetyStatus
 */
SafetyStatus Voltage_OpenWire(void){
    SafetyStatus status = SAFE;
    wakeup_idle(NUM_MINIONS);
    
    RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    
    //int32_t openModules = LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
    LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);

    /*
    TO DO: fix this to work with VOLT_TAP_DIST array
    This assumes a specific distribution of modules to minions, which we do not have anymore
    (don't use)
    In the future, check the bitmap returned by LTC6811_run_openwire_multi, and see if the expected modules are closed (closed = 0) based on VOLT_TAP_DIST
    */
    for(int32_t i = 0; i < NUM_MINIONS; i++) {

        if(Minions[i].system_open_wire != 0){
            if ((i == NUM_MINIONS -1) && ((Minions[i].system_open_wire & 0xEF) != 0)) { 
                break; //Open Wire test runs using MAX_VOLT_SENSORS_PER_MINION_BOARD so value of last module should be cleared
            }
            status = DANGER;
            break;
        }
    }
    
    RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);

    return status;
}

/** Voltage_GetOpenWire
 * Finds the pin locations of the open wires
 * @return hexadecimal string (1 means open wire, 0 means closed)
 */
uint32_t Voltage_GetOpenWire(void){
    wakeup_idle(NUM_MINIONS);
    if (!Fault_Flag) {
        RTOS_BPS_MutexPend(&MinionsASIC_Mutex, OS_OPT_PEND_BLOCKING);
    }
    
    uint32_t result = LTC6811_run_openwire_multi(NUM_MINIONS, Minions, false);
    
    if (!Fault_Flag) {
        RTOS_BPS_MutexPost(&MinionsASIC_Mutex, OS_OPT_POST_NONE);
    }
    return result;
}
#endif

/** Voltage_GetModuleMillivoltage
 * Gets the voltage of a certain battery module in the battery pack
 * @precondition moduleIdx < NUM_BATTERY_SENSORS
 * @param index of battery (0-indexed)
 * @return voltage of module at specified index, 0xFFFF if error
 */
uint16_t Voltage_GetModuleMillivoltage(uint8_t moduleIdx){
    // These if statements prevents a hardfault.
    if(moduleIdx >= NUM_BATTERY_MODULES) {
        return 0xFFFF;
    }

    if (!Fault_Flag) {
        RTOS_BPS_MutexPend(&Voltage_Mutex, OS_OPT_PEND_BLOCKING);
    }
    
    uint16_t ret = Voltages[moduleIdx] / 10;
    
    if (!Fault_Flag) {
        RTOS_BPS_MutexPost(&Voltage_Mutex, OS_OPT_POST_NONE);
    }
    return ret;
}

/** Voltage_GetTotalPackVoltage
 * Gets the total voltage of the battery pack
 * @return voltage of whole battery pack
 */
uint32_t Voltage_GetTotalPackVoltage(void){
    return packVoltage_mv;
}


/** Voltage_GetMaxVoltage
 * Gets the maximum cell voltage within the battery pack
 * @return maximum voltage of battery pack
 */
uint32_t Voltage_GetMaxVoltage(void){
    return maxVoltage_mv;
}

/** Voltage_GetMinVoltage
 * Gets the minimum cell voltage within the battery pack
 * @return minimum voltage of battery pack
 */
uint32_t Voltage_GetMinVoltage(void){
    return minVoltage_mv;
}


