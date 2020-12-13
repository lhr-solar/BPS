/* Copyright (c) 2020 UT Longhorn Racing Solar */
/** Amps.c
 * Wrapper that holds all electrical current related information of BeVolt's
 * battery pack.
 */

#include "Amps.h"
#include "AS8510.h"
#include "os.h"
#include "Tasks.h"
#include "BSP_SPI.h"
#include "CANbus.h"

static OS_MUTEX AmperesData_Mutex;

static OS_SEM AmperesIO_Sem;
static bsp_os_t spi_os;

#ifdef RTOS
void Amperes_Pend(){
	CPU_TS ts;
    OS_ERR err;
    OSSemPend(&AmperesIO_Sem,
                        0,
                        OS_OPT_PEND_BLOCKING,
                        &ts,
                        &err);
    assertOSError(err);
}

void Amperes_Post(){
	OS_ERR err;
    OSSemPost(&AmperesIO_Sem,
                        OS_OPT_POST_1,
                        &err);
    assertOSError(err);
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

static int16_t latestMeasureMilliAmps;

/** Amps_Init
 * Initializes hardware to begin current monitoring.
 */
void Amps_Init(void) {
	OS_ERR err;
	OSSemCreate(&AmperesIO_Sem,
				"AmperesIO Semaphore",
                0,
                &err);
	assertOSError(err);
	spi_os.pend = Amperes_Pend;
	spi_os.post = Amperes_Post;
	AS8510_Init(spi_os);
	OSMutexCreate(&AmperesData_Mutex, "Amperes Mutex", &err);
	assertOSError(err);
}

/** Amps_UpdateMeasurements
 * Stores and updates the new measurements received
 * @return SUCCESS or ERROR
 */
ErrorStatus Amps_UpdateMeasurements(void) {
	OS_ERR err;
	CPU_TS ticks;
	OSMutexPend(&AmperesData_Mutex, 0, OS_OPT_PEND_BLOCKING, &ticks, &err);
	assertOSError(err);
	latestMeasureMilliAmps = AS8510_GetCurrent();	// TODO: verify that there is no conversion required here
	OSMutexPost(&AmperesData_Mutex, OS_OPT_POST_NONE, &err);
	assertOSError(err);
	return SUCCESS;		//TODO: Once this has been tested, stop returning errors
}

/** Amps_CheckStatus
 * Checks if pack does not have a short circuit
 * @param chargingOnly a flag that indicates whether we should allow
 * 		  only charging or both charging and discharging.
 * @return SAFE or DANGER
 */
SafetyStatus Amps_CheckStatus(bool chargingOnly) {
	OS_ERR err;
	CPU_TS ticks;
	SafetyStatus status;
	OSMutexPend(&AmperesData_Mutex, 0, OS_OPT_PEND_BLOCKING, &ticks, &err);
	assertOSError(err);
	if((latestMeasureMilliAmps > MAX_CHARGING_CURRENT)&&(latestMeasureMilliAmps < MAX_CURRENT_LIMIT)&&(!chargingOnly)){
		status = SAFE;
	}
	else if((latestMeasureMilliAmps <= 0)&&(latestMeasureMilliAmps > MAX_CHARGING_CURRENT)&&chargingOnly){
		return SAFE;
	}
	else{
		status = DANGER;
	}
	OSMutexPost(&AmperesData_Mutex, OS_OPT_POST_NONE, &err);
	assertOSError(err);
	return status;
}

/** Amps_IsCharging
 * Determines if the the battery pack is being charged or discharged depending on
 * the sign of the current
 * @return 1 if charge, 0 if discharge
 */
int8_t Amps_IsCharging(void) {
	// TODO: Make sure that the amperes board is installed in such a way that negative => charging
	return latestMeasureMilliAmps < 0;
}

/** Amps_GetReading
 * Measures the electrical current from the Ampere minion board
 * @return milliamperes value
 */
int32_t Amps_GetReading(void) {
	return latestMeasureMilliAmps;
}

void Task_AmperesMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

	bool amperesHasBeenChecked = false;

	CANData_t CanData;
    CANPayload_t CanPayload;
    CANMSG_t CanMsg;

    while(1) {
        // BLOCKING =====================
        // Update Amperes Measurements
		Amps_UpdateMeasurements();
		int current = Amps_GetReading();
		CanData.f = (float)current/1000; //send data in Amps
		CanPayload.data = CanData;
		CanMsg.id = CURRENT_DATA;
		CanMsg.payload = CanPayload;
		OSQPost(&CANBus_MsgQ, &CanMsg, 4, OS_OPT_POST_FIFO, &err); //Send data to Can
        // Check if amperes is NOT safe:
		SafetyStatus amperesStatus = Amps_CheckStatus(AdminOverride);
		if(amperesStatus != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
			assertOSError(err);
			
        } else if((amperesStatus == SAFE) && (!amperesHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
			assertOSError(err);

            amperesHasBeenChecked = true;
        }

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

        WDog_BitMap |= WD_AMPERES;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);
    }
}
