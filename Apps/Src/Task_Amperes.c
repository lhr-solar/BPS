/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include <string.h>
#include "Amps.h"
#include "AS8510.h"
#include "os.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_SPI.h"
#include "CANbus.h"

void Task_AmperesMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

	bool amperesHasBeenChecked = false;

	CANData_t CanData;
    CANPayload_t CanPayload;
    CANMSG_t CanMsg;

	Amps_Init();

    while(1) {
        // BLOCKING =====================
        // Update Amperes Measurements
		Amps_UpdateMeasurements();
        // Check if amperes is NOT safe:
		SafetyStatus amperesStatus = Amps_CheckStatus(Amps_IsCharging());
		if(amperesStatus != SAFE) {
		    Fault_BitMap = Fault_OCURR;
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

		//Send measurement to CAN queue
		int current = Amps_GetReading();
		CanData.w = (uint32_t) current;
		CanPayload.data = CanData;
		CanMsg.id = CURRENT_DATA;
		CanMsg.payload = CanPayload;
        CAN_Queue_Post(CanMsg);         // send data to CAN

        //signal watchdog
        OSMutexPend(&WDog_Mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
        assertOSError(err);

        WDog_BitMap |= WD_AMPERES;

        OSMutexPost(&WDog_Mutex, OS_OPT_POST_NONE, &err);
        assertOSError(err);
        
        //delay of 100ms
        OSTimeDly(10, OS_OPT_TIME_DLY, &err);
        assertOSError(err);
    }
}