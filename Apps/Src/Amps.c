/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** Amps.c
 * Wrapper that holds all electrical current related information of BeVolt's
 * battery pack.
 */

#include "Amps.h"
#include "LTC2315.h"
#include "RTOS_BPS.h"
#include "Tasks.h"
#include "BSP_SPI.h"
#include "CANbus.h"
#include "Charge.h"
#ifdef SIMULATION
#include "Simulator.h"
#endif

static OS_SEM AmperesIO_Sem;
static bsp_os_t spi_os;

void Amperes_Pend(){
    RTOS_BPS_SemPend(&AmperesIO_Sem, OS_OPT_PEND_BLOCKING);
}

void Amperes_Post(){
    RTOS_BPS_SemPost(&AmperesIO_Sem, OS_OPT_POST_1);
}

static int32_t latestMeasureMilliAmps;

/** Amps_Init
 * Initializes hardware to begin current monitoring.
 */
void Amps_Init(void) {
    RTOS_BPS_SemCreate(&AmperesIO_Sem, "AmperesIO Semaphore", 0);
    spi_os.pend = Amperes_Pend;
    spi_os.post = Amperes_Post;
    LTC2315_Init(spi_os);
}

/** Amps_UpdateMeasurements
 * Stores and updates the new measurements received
 */
void Amps_UpdateMeasurements(void) {
    #ifdef SIMULATION
        int32_t current = Simulator_getCurrent();
        latestMeasureMilliAmps = Simulator_getCurrent();
        char* msg;
        asprintf(&msg, "Milliamp measurement is %d\n", current);
        Simulator_Log(LOG_INFO, msg);
        free(msg);
    #else 
        latestMeasureMilliAmps = LTC2315_GetCurrent();
    #endif
}

/** Amps_CheckStatus
 * Checks if pack does not have a short circuit
 * @param minTemperature the minimum temperature measured in the most recent batch of temperature readings
 * @param maxTemperature the maximum temperature measured in the most recent batch of temperature readings
 * @return SAFE or DANGER
 */
SafetyStatus Amps_CheckStatus(int32_t minTemperature, int32_t maxTemperature) {
    int32_t lo_limit = (maxTemperature > MAX_CHARGE_TEMPERATURE_LIMIT)  ? -AMPS_NOISE_LIMIT : 
                      ((minTemperature < COLD_CHARGE_TEMPERATURE)       ? MAX_COLD_CHARGING_CURRENT : 
                                                                          MAX_CHARGING_CURRENT);
    int32_t hi_limit = (minTemperature < COLD_DISCHARGE_TEMPERATURE)    ? MAX_COLD_CURRENT_LIMIT : 
                                                                          MAX_CURRENT_LIMIT;

    int32_t current = latestMeasureMilliAmps;
    return (current > lo_limit && current < hi_limit) ? SAFE : DANGER;
}

/** Amps_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return true if charge, false if discharge
 */
bool Amps_IsCharging(void) {
    return latestMeasureMilliAmps + AMPS_NOISE_LIMIT < 0;
}

/** Amps_GetReading
 * Measures the electrical current from the Ampere minion board
 * @return milliamperes value
 */
int32_t Amps_GetReading(void) {
    return latestMeasureMilliAmps;
}

/**
 * @brief calibrate the amperes module. Must be called when the contactors are open
 * 
 */
void Amps_Calibrate(void) {
    // we have observed that when the BPS is initially powered, there is a delay before the LTC2315
    // can be calibrated properly. This is not observed on pressing the reset button
    
    // initial calibration
    #ifndef SIMULATION
        LTC2315_Calibrate();
    #endif
    Amps_UpdateMeasurements();

    // keep calibrating until we read 0 Amps
    RTOS_BPS_DelayTick(1); //TODO: Change this to DelayMs if resolution of ticks is <10ms
    Amps_UpdateMeasurements();
    #ifndef SIMULATION
    while (Amps_GetReading() != 0) {
        LTC2315_Calibrate();
        RTOS_BPS_DelayTick(1);
        Amps_UpdateMeasurements();
    }
    #endif
}
