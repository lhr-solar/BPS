/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
/** Amps.c
 * Wrapper that holds all electrical current related information of BeVolt's
 * battery pack.
 */

#include "Amps.h"
#include "LTC2315.h"
#include "Tasks.h"
#include "BSP_SPI.h"
#include "CANbus.h"
#include "Charge.h"
#ifdef SIMULATION
#include "Simulator.h"
#endif
#include "RTOS_BPS.h"

static OS_MUTEX AmperesData_Mutex;

static OS_SEM AmperesIO_Sem;
static bsp_os_t spi_os;

#ifdef RTOS
void Amperes_Pend(){
    RTOS_BPS_SemPend(&AmperesIO_Sem, OS_OPT_PEND_BLOCKING);
}

void Amperes_Post(){
    RTOS_BPS_SemPost(&AmperesIO_Sem, OS_OPT_POST_1);
}
#endif

#ifdef BAREMETAL
void Amperes_Pend(void) {
    return;
}

void Amperes_Post(void) {
    return;
}
#endif

static int32_t latestMeasureMilliAmps;

/** Amps_Init
 * Initializes hardware to begin current monitoring.
 */
void Amps_Init(void) {
	RTOS_BPS_SemCreate(&AmperesIO_Sem, "AmperesIO Semaphore", 0);
	spi_os.pend = Amperes_Pend;
	spi_os.post = Amperes_Post;
	LTC2315_Init(spi_os);
	RTOS_BPS_MutexCreate(&AmperesData_Mutex, "Amperes Mutex");
}

/** Amps_UpdateMeasurements
 * Stores and updates the new measurements received
 */
void Amps_UpdateMeasurements(void) {
	OS_ERR err;
	CPU_TS ticks;
	RTOS_BPS_MutexPend(&AmperesData_Mutex, OS_OPT_PEND_BLOCKING);
	#ifdef SIMULATION
		latestMeasureMilliAmps = Simulator_getCurrent();
		char* msg;
		asprintf(&msg, "Milliamp measurement is %d\n", latestMeasureMilliAmps);
    	Simulator_Log(LOG_INFO, msg);
		free(msg);
	#else 
		latestMeasureMilliAmps = LTC2315_GetCurrent();
	#endif
	RTOS_BPS_MutexPost(&AmperesData_Mutex, OS_OPT_POST_NONE);


}

/** Amps_CheckStatus
 * Checks if pack does not have a short circuit
 * @param maxTemperature the maximum temperature measured in the most recent batch of temperature readings
 * @return SAFE or DANGER
 */
SafetyStatus Amps_CheckStatus(int32_t maxTemperature) {
	SafetyStatus status;

	// determine if we should allow charging or not
	bool dischargingOnly = maxTemperature > MAX_CHARGE_TEMPERATURE_LIMIT;
	int32_t chargingCurrentLimit = dischargingOnly ? 0 : MAX_CHARGING_CURRENT;

	RTOS_BPS_MutexPend(&AmperesData_Mutex, OS_OPT_PEND_BLOCKING);
	if((latestMeasureMilliAmps >= chargingCurrentLimit)&&(latestMeasureMilliAmps < MAX_CURRENT_LIMIT)){
		status = SAFE;
	} else{
		status = DANGER;
	}
	RTOS_BPS_MutexPost(&AmperesData_Mutex, OS_OPT_POST_NONE);
	return status;
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
	RTOS_BPS_DelayTick(1);
	Amps_UpdateMeasurements();
	#ifndef SIMULATION
	while (Amps_GetReading() != 0) {
		LTC2315_Calibrate();
		RTOS_BPS_DelayTick(1);
		Amps_UpdateMeasurements();
	}
	#endif
}
