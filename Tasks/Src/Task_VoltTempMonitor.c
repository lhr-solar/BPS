/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include <string.h>
#include "Tasks.h"
#include "Voltage.h"
#include "Temperature.h"
#include "Fans.h"
#include "CANbus.h"
#include "Amps.h"
#include "CAN_Queue.h"
#include "Print_Queue.h"
#include "RTOS_BPS.h"
#include "os.h"

typedef struct voltage_summary_s {
    uint64_t pack_voltage_mv:       24;
    uint64_t voltage_range_mv:      24;
    uint64_t elapsed_ms:            16;
} volt_summary_t;
_Static_assert(sizeof(volt_summary_t) == 8, 
              "volt_summary_t must fit within a CAN message!");

typedef struct temperature_summary_s {
    int64_t avg_temperature_mc:     24;
    int64_t temperature_range_mc:   24;
    uint64_t elapsed_ms:            16;
} temp_summary_t;
_Static_assert(sizeof(temp_summary_t) == 8, 
              "temp_summary_t must fit within a CAN message!");

// declared in Tasks.c
extern cell_asic Minions[NUM_MINIONS];

// for averaging voltage and temperature data
static uint32_t voltage_data_count = 0;
static uint32_t voltage_totals[NUM_BATTERY_MODULES] = {0};
static uint32_t temperature_data_count = 0;
static int32_t temperature_totals[NUM_BATTERY_MODULES] = {0};

static uint32_t task_cycle_counter = 0;


// volttempmonitor functions -- split off for readability
static bool CheckVoltage(void);
static void CheckOpenWire(void);
static bool CheckTemperature(void);
static void SendVoltageArray(void);
static void SendTemperatureArray(void);



void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg; 
    OS_ERR err;

    Fans_Init();
    Voltage_Init(Minions);
    Temperature_Init(Minions);

    // two charge enables for an edge detector
    // charge enable is sent every ODR_CHARGING_ENABLED_PERIOD_MS but if it changes we 
    // want to send it immediately 
    bool charge_enable_prev = true;
    bool charge_enable = true;

    // buffers for CAN messages
    // static to avoid stack allocation
    static CANMSG_t msg_charge_enable =         {.id = CHARGING_ENABLED};
    static CANMSG_t msg_voltage_summary =       {.id = VOLTAGE_SUMMARY};
    static CANMSG_t msg_temperature_summary =   {.id = TEMPERATURE_SUMMARY};

    // timing variables
    uint32_t volt_prev_tick = 0, temp_prev_tick = 0, chg_en_prev_tick = 0, 
             volt_data_send_prev_tick = 0, temp_data_send_prev_tick = 0;
    uint32_t volt_elapsed = 0, temp_elapsed = 0;

    while (1) {
        task_cycle_counter++;       // used for output and sampling decimation / averaging

        // BLOCKING =====================
        // Update Voltage Measurements
        Voltage_UpdateMeasurements();
        charge_enable = CheckVoltage();
        volt_elapsed = ((uint32_t)OSTimeGet(&err)) - volt_prev_tick;
        volt_prev_tick = (uint32_t)OSTimeGet(&err);
        voltage_data_count++;
        for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
            voltage_totals[i] += Voltage_GetModuleMillivoltage(i);
        }
        
        // BLOCKING =====================
        // Check if open wire is NOT safe:
        CheckOpenWire();
        
        // BLOCKING =====================
        // Update Temperature Measurements (every ODR_TEMPERATURE_DECIMATION iterations)
        if (task_cycle_counter % ODR_TEMPERATURE_DECIMATION == 0) {
            Temperature_UpdateAllMeasurements();
            charge_enable &= CheckTemperature();
            temp_elapsed = ((uint32_t)OSTimeGet(&err)) - temp_prev_tick;
            temp_prev_tick = (uint32_t)OSTimeGet(&err);
            temperature_data_count++;
            for (int i = 0; i < NUM_TEMPERATURE_SENSORS; i++) {
                temperature_totals[i] += Temperature_GetSingleTempSensor(i);
            }
        }
        
        // NONBLOCKING ==================
        // Update Fan Speeds based on average temperature
        Fans_SetAll(TOPSPEED);

        // NONBLOCKING ==================
        // Send more frequent CAN messages -- every loop iteration

        *(volt_summary_t *)&msg_voltage_summary.payload.data.bytes = (volt_summary_t){
            .pack_voltage_mv = Voltage_GetTotalPackVoltage(),
            .voltage_range_mv = 0,
            .elapsed_ms = volt_elapsed
        };

        *(temp_summary_t *)&msg_temperature_summary.payload.data.bytes = (temp_summary_t){
            .avg_temperature_mc = Temperature_GetTotalPackAvgTemperature(),
            .temperature_range_mc = 0,
            .elapsed_ms = temp_elapsed
        };

        // NONBLOCKING ==================
        // Send less frequent CAN messages -- change frequency in config.h

        if ((charge_enable != charge_enable_prev)
         || (((uint32_t)OSTimeGet(&err)) - chg_en_prev_tick > MS_TO_OS_TICKS(ODR_CHARGING_ENABLED_PERIOD_MS))) {
            msg_charge_enable.payload.data.b = charge_enable;
            CAN_TransmitQueue_Post(msg_charge_enable);
            chg_en_prev_tick = (uint32_t)OSTimeGet(&err);
        }
        charge_enable_prev = charge_enable;

        if (((uint32_t)OSTimeGet(&err)) - volt_data_send_prev_tick > MS_TO_OS_TICKS(ODR_VOLTAGE_DATA_ARRAY_PERIOD_MS)) {
            SendVoltageArray();
            volt_data_send_prev_tick = (uint32_t)OSTimeGet(&err);
            voltage_data_count = 0;
            memset(voltage_totals, 0, sizeof(voltage_totals));
        }

        if (((uint32_t)OSTimeGet(&err)) - temp_data_send_prev_tick > MS_TO_OS_TICKS(ODR_TEMPERATURE_DATA_ARRAY_PERIOD_MS)) {
            SendTemperatureArray();
            temp_data_send_prev_tick = (uint32_t)OSTimeGet(&err);
            temperature_data_count = 0;
            memset(temperature_totals, 0, sizeof(temperature_totals));
        }

        // signal watchdog
        RTOS_BPS_MutexPend(&WDog_Mutex, OS_OPT_PEND_BLOCKING);

        WDog_BitMap |= WD_VOLT_TEMP; //Set watchdog bits for task

        RTOS_BPS_MutexPost(&WDog_Mutex, OS_OPT_POST_NONE); 
        
        // delay of 20ms
        RTOS_BPS_DelayMs(20);
    }
}

/**
 * @brief check voltage status. If voltage is not safe, enter fault state
 *        Voltage_UpdateMeasurements() must be called beforehand.
 * 
 * @return true if charging should be enabled, false otherwise -- if not SAFE, 
 *         this will not return
 */
static bool CheckVoltage(void) {
    SafetyStatusOpt status_opt;
    SafetyStatus status = Voltage_CheckStatus(&status_opt);
    static bool voltageHasBeenChecked = false;

    if (status != SAFE) {
        if      (status_opt == UNDERVOLTAGE) Fault_BitMap |= Fault_UVOLT;
        else if (status_opt == OVERVOLTAGE)  Fault_BitMap |= Fault_OVOLT;
        EnterFaultState();  // doesn't return
    }
    if (!voltageHasBeenChecked) { // Signal to turn on contactor but only signal once
        RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
        voltageHasBeenChecked = true;
    }

    return (status_opt != CHARGE_DISABLE);
}

/**
 * @brief check open wire status. If open wire is not safe, enter fault state
 */
static void CheckOpenWire(void) {
    static bool openWireHasBeenChecked = false;
    // TODO: get OpenWire to not take forever and actually run this
    // SafetyStatus status = Voltage_OpenWire();
    SafetyStatus status = SAFE;
    
    if (status != SAFE) {
        Fault_BitMap |= Fault_OW;
        EnterFaultState();
    }
    if (!openWireHasBeenChecked) {
        // Signal to turn on contactor but only signal once
        RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
        openWireHasBeenChecked = true;
    }
}

/**
 * @brief check temperature status. If temperature is not safe, enter fault state
 *        Temperature_UpdateAllMeasurements() must be called beforehand.
 * 
 * @return true if charging should be enabled, false otherwise -- if not SAFE,
 *         this will not return
 */
static bool CheckTemperature(void) {
    SafetyStatusOpt status_opt;
    SafetyStatus status = Temperature_CheckStatus(Amps_IsCharging(), &status_opt);
    static bool temperatureHasBeenChecked = false;

    if (status != SAFE) {
        Fault_BitMap = Fault_OTEMP;
        EnterFaultState();
    } 
    if (!temperatureHasBeenChecked) {
        // Signal to turn on contactor but only signal once
        RTOS_BPS_SemPost(&SafetyCheck_Sem4, OS_OPT_POST_1);
        temperatureHasBeenChecked = true;
    }

    return (status_opt != CHARGE_DISABLE);
}

/**
 * @brief send all voltages over CAN. This will be an average of voltages since last send
 */
static void SendVoltageArray(void) {
    static CANMSG_t msg = {.id = VOLTAGE_DATA_ARRAY};

    for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
        msg.payload.idx = i;
        msg.payload.data.w = (int)(voltage_totals[i] / voltage_data_count);
        CAN_TransmitQueue_Post(msg);
    }
}

/**
 * @brief send all temperatures over CAN. This will be an average of temperatures since last send
 */
static void SendTemperatureArray(void) {
    static CANMSG_t msg = {.id = TEMPERATURE_DATA_ARRAY};

    for (int i = 0; i < NUM_BATTERY_MODULES; i++) {
        msg.payload.idx = i;
        msg.payload.data.w = (int)(temperature_totals[i] / temperature_data_count);
        CAN_TransmitQueue_Post(msg);
    }
}
