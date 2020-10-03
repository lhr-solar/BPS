#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"

OS_TCB Init_TCB;
CPU_STK Init_Stk[DEFAULT_STACK_SIZE];

static void Voltage_UpdateMeasurements(void) {
    printf("V0\r\n");

    OS_ERR err;
    OSTimeDly(1,
            OS_OPT_TIME_DLY,
            &err);

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
    
    OS_ERR err;
    OSTimeDly(1,
            OS_OPT_TIME_DLY,
            &err);

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

    OS_ERR err;
    OSTimeDly(1,
            OS_OPT_TIME_DLY,
            &err);

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
    
    OS_ERR err;
    OSTimeDly(1,
            OS_OPT_TIME_DLY,
            &err);

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

void FaultState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    printf("Fault State\r\n");

    // BLOCKING =====================
    // Wait until a FAULT is signaled by another task.
    OSSemPend(&Fault_Sem4,
                0,
                OS_OPT_PEND_BLOCKING,
                &ts,
                &err);

    // Turn Contactor Off

    // Turn Strobe Light On

    // Turn LEDs On

    // Record State of BPS into EEPROM

    // Push Trip message to CAN Q

    // Push Contactor State message to CAN Q
    printf("F\r\n");

    while(1) {
        // CLI
        // WDOG Reset
    }
}

void CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    printf("Critical State\r\n");

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for(int32_t check = 0; check < NUM_FAULT_POINTS; check++) {
        OSSemPend(&SafetyCheck_Sem4,
                    0,
                    OS_OPT_PEND_BLOCKING,
                    &ts,
                    &err);
        printf("CS:%d\r\n", check);
    }

    // Turn Contactor On
    
    // Push All Clear message to CAN Q

    // Push Contactor State message to CAN Q
}

void VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

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
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
        
        } else if((voltage_status == SAFE) && (!isfirst_voltage_check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_voltage_check = true;
        }

        // BLOCKING =====================
        // Update Open Wire Measurements
        // Check if open wire is NOT safe:
        SafetyStatus wire_status = Voltage_OpenWire();

        if(wire_status != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
        } else if((wire_status == SAFE) && (!isfirst_openwire_Check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_openwire_Check = true;
        }

        // BLOCKING =====================
        // Update Temperature Measurements
        Temperature_UpdateAllMeasurements();
        
        // Check if temperature is NOT safe:
        SafetyStatus temperature_status = Temperature_CheckStatus(Current_IsCharging());
        if(temperature_status != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
        } else if((temperature_status == SAFE) && (!isfirst_temperature_check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_temperature_check = true;
        }

        // Control Fans depending on temperature
    }
}

void AmperesMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    printf("Amperes Monitor\r\n");

	bool isfirst_amperes_check = false;

    while(1) {
        // BLOCKING =====================
        // Update Amperes Measurements
		Current_UpdateMeasurements();

        // Check if amperes is NOT safe:
		SafetyStatus amperes_status = Current_CheckStatus(true);
		if(amperes_status != SAFE) {
            OSSemPost(&Fault_Sem4,
                        OS_OPT_POST_1,
                        &err);
        } else if((amperes_status == SAFE) && (!isfirst_amperes_check)) {
            // Signal to turn on contactor but only signal once
            OSSemPost(&SafetyCheck_Sem4,
                        OS_OPT_POST_1,
                        &err);
            // assert
            isfirst_amperes_check = true;
        }
    }
}

int main() {
    OS_ERR err;

    OSInit(&err);

    // assert

    OSTaskCreate(&Init_TCB,				// TCB
				"Initialize System",	// Task Name (String)
				Task_Init,				// Task function pointer
				(void *)0,				// Task function args
				TASK_INIT_PRIO,			// Priority
				Init_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				DEFAULT_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

	// ASSERT err

    OSTaskCreate(&FaultState_TCB,				// TCB
				"Fault State",	// Task Name (String)
				FaultState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_FAULT_STATE_PRIO,			// Priority
				AmperesMonitor_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_FAULT_STATE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

	// ASSERT err

    OSTaskCreate(&CriticalState_TCB,				// TCB
				"Critical State",	// Task Name (String)
				CriticalState,				// Task function pointer
				(void *)0,				// Task function args
				TASK_CRITICAL_STATE_PRIO,			// Priority
				CriticalState_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_CRITICAL_STATE_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

	// ASSERT err

    OSTaskCreate(&VoltTempMonitor_TCB,				// TCB
				"Voltage/Temperature Monitor",	// Task Name (String)
				VoltTempMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_VOLT_TEMP_MONITOR_PRIO,			// Priority
				VoltTempMonitor_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_VOLT_TEMP_MONITOR_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

	// ASSERT err

    OSTaskCreate(&AmperesMonitor_TCB,				// TCB
				"Amperes Monitor",	// Task Name (String)
				AmperesMonitor,				// Task function pointer
				(void *)0,				// Task function args
				TASK_AMPERES_MONITOR_PRIO,			// Priority
				AmperesMonitor_Stk,				// Stack
				WATERMARK_STACK_LIMIT,	// Watermark limit for debugging
				TASK_AMPERES_MONITOR_STACK_SIZE,		// Stack size
				0,						// Queue size (not needed)
				10,						// Time quanta (time slice) 10 ticks
				(void *)0,				// Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,	// Options
				&err);					// return err code

	// ASSERT err

    OSStart(&err);

}