/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include <string.h>
#include "Amps.h"
#include "LTC2315.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_SPI.h"
#include "CANbus.h"
#include "Temperature.h"
#include "Charge.h"

void Task_AmperesMonitor(void *p_arg) {
    (void)p_arg;

	bool amperesHasBeenChecked = false;

	CANData_t CanData;
    CANPayload_t CanPayload;
    CANMSG_t CanMsg;

	Amps_Init();
    Amps_Calibrate();

    while(1) {
        // BLOCKING =====================
        // Update Amperes Measurements
		Amps_UpdateMeasurements();
        // Check if amperes is NOT safe:
		SafetyStatus amperesStatus = Amps_CheckStatus(Temperature_GetMaxTemperature());
		if(amperesStatus != SAFE) {
		    Fault_BitMap = Fault_OCURR;
            EnterFaultState();
        } else if((amperesStatus == SAFE) && (!amperesHasBeenChecked)) {
            // Signal to turn on contactor but only signal once
	xSemaphoreGive(SafetyCheck_Sem4);
            amperesHasBeenChecked = true;
        }

        // update state of charge
        Charge_Calculate(Amps_GetReading());

		//Send measurement to CAN queue
		int current = Amps_GetReading();
		CanData.w = (uint32_t) current;
		CanPayload.data = CanData;
		CanMsg.id = CURRENT_DATA;
		CanMsg.payload = CanPayload;
        CAN_Queue_Post(CanMsg);         // send data to CAN

        // Send state of charge to CAN queue
        CanData.w = Charge_GetPercent();
        CanPayload.data = CanData;
        CanMsg.id = SOC_DATA;
        CanMsg.payload = CanPayload;
        CAN_Queue_Post(CanMsg);

        //signal watchdog
	xSemaphoreTake(WDog_Mutex, (TickType_t)portMAX_DELAY); 

        WDog_BitMap |= WD_AMPERES;

	xSemaphoreGive(WDog_Mutex);
        //delay of 10ms
        RTOS_BPS_DelayMs(10);
    }
}
