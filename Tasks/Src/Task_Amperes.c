/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include <string.h>
#include "Amps.h"
#include "LTC2315.h"
#include "RTOS_BPS.h"
#include "os.h"
#include "Tasks.h"
#include "CAN_Queue.h"
#include "BSP_SPI.h"
#include "CANbus.h"
#include "Temperature.h"
#include "Charge.h"

// for averaging current data
static int32_t amps_data_count = 0;
static int32_t amps_totals = 0;


// amperes functions -- split off for readability
static void CheckCurrent(void);

void Task_AmperesMonitor(void *p_arg) {
    (void)p_arg;
    OS_ERR err;

    // buffers for CAN messages
    // static to avoid stack allocation
    static CANMSG_t msg_current =           {.id = CURRENT_DATA};
    static CANMSG_t msg_state_of_charge =   {.id = STATE_OF_CHARGE_DATA};

    // timing variables
    uint32_t curr_data_send_prev_tick = 0, soc_data_send_prev_tick = 0;

    Amps_Init();
    Amps_Calibrate();

    while (1) {
        // BLOCKING =====================
        // Update Amperes Measurements
        Amps_UpdateMeasurements();
        CheckCurrent();
        amps_data_count++;
        amps_totals += Amps_GetReading(false);  // use filtered value here

        // update state of charge
        Charge_Calculate(Amps_GetReading(true));    // use raw value here

        // NONBLOCKING ==================
        // Send CAN messages

        if (((uint32_t)OSTimeGet(&err)) - curr_data_send_prev_tick > MS_TO_OS_TICKS(ODR_CURRENT_DATA_PERIOD_MS)) {
            msg_current.payload.data.w = (uint32_t)(amps_totals / amps_data_count);
            CAN_TransmitQueue_Post(msg_current);
            curr_data_send_prev_tick = (uint32_t)OSTimeGet(&err);
            amps_data_count = 0;
            amps_totals = 0;
        }

        if (((uint32_t)OSTimeGet(&err)) - soc_data_send_prev_tick > MS_TO_OS_TICKS(ODR_STATE_OF_CHARGE_DATA_PERIOD_MS)) {
            msg_state_of_charge.payload.data.w = Charge_GetPercent();
            CAN_TransmitQueue_Post(msg_state_of_charge);
            soc_data_send_prev_tick = (uint32_t)OSTimeGet(&err);
        }

        //signal watchdog
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        WDog_BitMap |= WD_AMPERES;

        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE);
        //delay of 10ms
        RTOS_BPS_DelayMs(10);
    }
}


/**
 * @brief check current reading status. If current reading is not safe, enter fault state
 */
static void CheckCurrent(void) {
    static bool amperesHasBeenChecked = false;
    SafetyStatus status = Amps_CheckStatus(Temperature_GetMinTemperature(), 
                                           Temperature_GetMaxTemperature());

    if (status != SAFE) {
        Fault_BitMap |= Fault_OCURR;
        EnterFaultState();
    } 
    if (!amperesHasBeenChecked) {
        // Signal to turn on contactor but only signal once
        RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
        amperesHasBeenChecked = true;
    }
}
