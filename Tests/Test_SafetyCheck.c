/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "FreeRTOS.h"
#include "Tasks.h"
#include "BSP_UART.h"
#ifdef SIMULATION
#include "Simulator.h"
#endif

static void Voltage_UpdateMeasurements(void) {
    printf("V0\r\n");\
    RTOS_BPS_DelayTick(1);
    printf("V1\r\n");
}

static SafetyStatus Voltage_CheckStatus(void) {
    static int cnt = 0;
    if(cnt == 15) {
        return DANGER;
    }
    cnt++;
    return SAFE;
}

static SafetyStatus Voltage_OpenWire(void) {
    printf("O0\r\n");

    RTOS_BPS_DelayTick(1);

    printf("O1\r\n");

    static int cnt = 0;
    if(cnt == 20) {
        return DANGER;
    }

    cnt++;

    return SAFE;
}

static void Temperature_UpdateAllMeasurements(void) {
    printf("T0\r\n");

    RTOS_BPS_DelayTick(1);

    printf("T1\r\n");
}

static SafetyStatus Temperature_CheckStatus(int8_t isCharging) {
    static int cnt = 0;
    (void)isCharging;

    if(cnt == 40) {
        return DANGER;
    }

    cnt++;

    return SAFE;
}

static void Current_UpdateMeasurements(void) {
    printf("A0\r\n");

    RTOS_BPS_DelayTick(1);

    printf("A1\r\n");
}

static SafetyStatus Current_CheckStatus(bool override) {
    static int cnt = 0;
    (void)override;

    if(cnt == 20) {
        return DANGER;
    }

    cnt++;

    return SAFE;
}

static int8_t Current_IsCharging(void) {
    return 1;
}

void CheckContactor(void *p_arg) {
    (void)p_arg;


    printf("Check Contactor\r\n");

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for(int32_t check = 0; check < NUM_FAULT_POINTS; check++) {
	xSemaphoreTake(SafetyCheck_Sem4, (TickType_t)portMAX_DELAY);
        printf("CS:%lld\r\n", (int64_t)check);
    }

    // Turn Contactor On
    
    // Push All Clear message to CAN Q

    // Push Contactor State message to CAN Q
}

void VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    printf("Volt Temp Monitor\r\n");

    // SafetyCheck_Sem4 must only be signaled once per parameter at system boot up.
    // These flags indicate was signaled for that parameter
    bool isfirst_voltage_check = false;
    bool isfirst_openwire_Check = false;
    bool isfirst_temperature_check = false;

    while(1) {
        // BLOCKING =====================
        // Update Voltage Measurements
        Voltage_UpdateMeasurements();

        // Check if voltage is NOT safe:
        SafetyStatus voltage_status = Voltage_CheckStatus();
        if(voltage_status != SAFE) {
            EnterFaultState();        
        } else if((voltage_status == SAFE) && (!isfirst_voltage_check)) {
            // Signal to turn on contactor but only signal once
	xSemaphoreGive(SafetyCheck_Sem4);
            isfirst_voltage_check = true;
        }

        // BLOCKING =====================
        // Update Open Wire Measurements
        // Check if open wire is NOT safe:
        SafetyStatus wire_status = Voltage_OpenWire();

        if(wire_status != SAFE) {
            EnterFaultState();
        } else if((wire_status == SAFE) && (!isfirst_openwire_Check)) {
            // Signal to turn on contactor but only signal once
	xSemaphoreGive(SafetyCheck_Sem4);
            // assert
            isfirst_openwire_Check = true;
        }

        // BLOCKING =====================
        // Update Temperature Measurements
        Temperature_UpdateAllMeasurements();
        
        // Check if temperature is NOT safe:
        SafetyStatus temperature_status = Temperature_CheckStatus(Current_IsCharging());
        if(temperature_status != SAFE) {
            EnterFaultState();
        } else if((temperature_status == SAFE) && (!isfirst_temperature_check)) {
            // Signal to turn on contactor but only signal once
	xSemaphoreGive(SafetyCheck_Sem4);
            // assert
            isfirst_temperature_check = true;
        }

        // Control Fans depending on temperature
    }
}

void AmperesMonitor(void *p_arg) {
    (void)p_arg;

    printf("Amperes Monitor\r\n");

	bool isfirst_amperes_check = false;

    while(1) {
        // BLOCKING =====================
        // Update Amperes Measurements
		Current_UpdateMeasurements();

        // Check if amperes is NOT safe:
		SafetyStatus amperes_status = Current_CheckStatus(true);
		if(amperes_status != SAFE) {
            EnterFaultState();
        } else if((amperes_status == SAFE) && (!isfirst_amperes_check)) {
            // Signal to turn on contactor but only signal once
	xSemaphoreGive(SafetyCheck_Sem4);
            // assert
            isfirst_amperes_check = true;
        }
    }
}

void foo(){
    return;
}

#ifdef SIMULATION
int main(int argc, char **argv) {
#else
int main() {
#endif

#ifdef SIMULATION
    // the first command line argument is the path to the JSON file
    Simulator_Init(argv[1]);
#endif
    
    OS_ERR err;

    CPU_Init();

    #if SIMULATION == 1
    OS_CPU_SysTickInit();
    #else
    OS_CPU_SysTickInit(100000);
    #endif

    OSInit(&err);

    // assert

    BSP_UART_Init(foo, foo, UART_USB);

    RTOS_BPS_TaskCreate(&Init_TCB,				// TCB
				"Initialize System",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_INIT_PRIO,			// Priority
				Init_Stk,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE);

    RTOS_BPS_TaskCreate(&CheckContactor_TCB,    // TCB
				"Task_CheckContactor",          // Task Name (String)
				CheckContactor,            // Task function pointer
				(void *)0,                      // Task function args
				TASK_CHECK_CONTACTOR_PRIO,      // Priority
				CheckContactor_Stk,             // Stack
				TASK_CHECK_CONTACTOR_STACK_SIZE);

    RTOS_BPS_TaskCreate(&VoltTempMonitor_TCB,				// TCB
				"Voltage/Temperature Monitor",	// Task Name (String)
				VoltTempMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_VOLT_TEMP_MONITOR_PRIO,			// Priority
				VoltTempMonitor_Stk,	// Watermark limit for debugging
				TASK_VOLT_TEMP_MONITOR_STACK_SIZE);	 

    RTOS_BPS_TaskCreate(&AmperesMonitor_TCB,				// TCB
				"Amperes Monitor",	// Task Name (String)
				AmperesMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_AMPERES_MONITOR_PRIO,			// Priority
				AmperesMonitor_Stk,	// Watermark limit for debugging
				TASK_AMPERES_MONITOR_STACK_SIZE); 

    OSStart(&err);

}
