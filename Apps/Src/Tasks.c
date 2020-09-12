#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"

#include "../../BSP/Inc/BSP_Lights.h"
#include "../../BSP/Inc/BSP_UART.h"
#include "Charge.h"

/*******************************************************************************
*    Shared Resources
*******************************************************************************/
/**
 * @brief   TCBs
 */
OS_TCB FaultState_TCB;
OS_TCB CriticalState_TCB;
OS_TCB PetWDog_TCB;
OS_TCB VoltTempMonitor_TCB;
OS_TCB AmperesMonitor_TCB;
OS_TCB LogInfo_TCB;
OS_TCB CANBusConsumer_TCB;
OS_TCB BatteryBalance_TCB;
OS_TCB CLI_TCB;
OS_TCB BLE_TCB;
OS_TCB Idle_TCB;

/**
 * @brief   Stacks
 */
CPU_STK FaultState_Stk[TASK_FAULT_STATE_STACK_SIZE];
CPU_STK CriticalState_Stk[TASK_CANBUS_CONSUMER_STACK_SIZE];
CPU_STK PetWDog_Stk[TASK_PETWDOG_STACK_SIZE];
CPU_STK VoltTempMonitor_Stk[TASK_VOLT_TEMP_MONITOR_STACK_SIZE];
CPU_STK AmperesMonitor_Stk[TASK_AMPERES_MONITOR_STACK_SIZE];
CPU_STK LogInfo_Stk[TASK_LOG_INFO_STACK_SIZE];
CPU_STK CANBusConsumer_Stk[TASK_CANBUS_CONSUMER_STACK_SIZE];
CPU_STK BatteryBalance_Stk[TASK_BATTERY_BALANCE_STACK_SIZE];
CPU_STK CLI_Stk[TASK_CLI_STACK_SIZE];
CPU_STK BLE_Stk[TASK_BLE_STACK_SIZE];
CPU_STK Idle_Stk[TASK_IDLE_STACK_SIZE];

/**
 * @brief   Queue for pushing and popping CAN Messages
 */
OS_Q CANBus_MsgQ;

/**
 * Semaphores
 */
OS_SEM SafetyCheck_Sem4;
OS_SEM Fault_Sem4;

/**
 * Mutexes
 */
OS_MUTEX WDog_Mutex;

/**
 * Global Variables
 */
cell_asic Minions[NUM_MINIONS];
uint32_t WDog_BitMap = 0;

/**
 * Used to assert if there has been an error in one of the OS functions
 * Kills the car if there is an OS error
 **/
void assertOSError(OS_ERR err){
    if(err != OS_ERR_NONE) {
        OSSemPost(&Fault_Sem4, OS_OPT_POST_1, &err);
    }
} 

//leaving these functions in the code for now, so it is easier to rebase this into other people's branches
/*
void Task_Init(void *p_arg) {

    OS_ERR err;

    OSSemCreate(&SafetyCheck_Sem4,
                "Safety Check Semaphore",
                0,
                &err);

    // ASSERT err

    OSSemCreate(&Fault_Sem4,
                "Fault/Tripped Semaphore",
                0,
                &err);

    // ASSERT err

    OSMutexCreate(&MinionsASIC_Mutex,
                "Minions ASIC Mutex",
                &err);

    // ASSERT err

    // TODO: Initialize threads! Look at main.c for reference
}

char command[COMMAND_SIZE];
void Task_FaultState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

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

<<<<<<< HEAD
=======
    //while(1) {
        // CLI
        // WDOG Reset
    //}

    BSP_Contactor_Off();
	BSP_Light_Off(RUN);
    BSP_Light_On(FAULT);

	uint8_t error = 0;

	if(Current_CheckStatus(false) != SAFE){
		error |= FAULT_HIGH_CURRENT;
		BSP_Light_On(OCURR);
	}

	if(Voltage_CheckStatus() != SAFE){
		// Toggle Voltage fault LED
		switch(Voltage_CheckStatus()){
			case OVERVOLTAGE:
				error |= FAULT_HIGH_VOLT;
				BSP_Light_On(OVOLT);
				Charge_Calibrate(OVERVOLTAGE);
				break;

			case UNDERVOLTAGE:
				error |= FAULT_LOW_VOLT;
				BSP_Light_On(UVOLT);
				Charge_Calibrate(UNDERVOLTAGE);
				break;

			default:
				error |= FAULT_VOLT_MISC;
				BSP_Light_On(OVOLT);
				BSP_Light_On(UVOLT);
				break;
		}
	}

	if(Temperature_CheckStatus(Current_IsCharging()) != SAFE){
		error |= FAULT_HIGH_TEMP;
		BSP_Light_On(OTEMP);
	}

	// Log all the errors that we have
	for(int i = 1; i < 0x00FF; i <<= 1) {
		if(error & i) EEPROM_LogError(i);
	}

	// Log all the relevant data for each error
	for(int i = 1; i < 0x00FF; i <<= 1) {
		if((error & i) == 0) continue;

		SafetyStatus *voltage_modules;
		uint8_t *temp_modules;
		uint16_t curr;
		switch(i) {
		// Temperature fault handling
		case FAULT_HIGH_TEMP:
			temp_modules = Temperature_GetModulesInDanger();
			for(int j = 0; j < NUM_BATTERY_MODULES; ++j)
				if(temp_modules[j]) EEPROM_LogData(FAULT_HIGH_TEMP, j);
			break;

		// Voltage fault handling
		case FAULT_HIGH_VOLT:
		case FAULT_LOW_VOLT:
		case FAULT_VOLT_MISC:
			voltage_modules = Voltage_GetModulesInDanger();
			for(int j = 0; j < NUM_BATTERY_MODULES; ++j)
				if(voltage_modules[j]) EEPROM_LogData(i, j);
			break;

		// Current fault handling
		case FAULT_HIGH_CURRENT:
			curr = Current_GetLowPrecReading();
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & curr);
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & (curr >> 8));
			curr = Current_GetHighPrecReading();
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & curr);
			EEPROM_LogData(FAULT_HIGH_CURRENT, 0x00FF & (curr >> 8));
			break;
		}
	}

	while(1) {
		Current_UpdateMeasurements();
		if(BSP_UART_ReadLine(command)) {
			CLI_Handler(command);
		}
		BSP_WDTimer_Reset();	// Even though faulted, WDTimer needs to be updated or else system will reset
					// causing WDOG error. WDTimer can't be stopped after it starts.
	}
}
>>>>>>> Direct copy paste from main

void Task_CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for(int32_t check = 0; check < NUM_FAULT_POINTS; check++) {
        OSSemPend(&SafetyCheck_Sem4,
                    0,
                    OS_OPT_PEND_BLOCKING,
                    &ts,
                    &err);
    }

    // Turn Contactor On
    
    // Push All Clear message to CAN Q

    // Push Contactor State message to CAN Q
}

void Task_PetWDog(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    // If WDog_BitMap is all set:
    //      Reset watchdog timer

}

void Task_VoltTempMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // BLOCKING =====================
        // Update Voltage Measurements
        // Check if voltage is NOT safe:

        // BLOCKING =====================
        // Update Open Wire Measurements
        // Check if open wire is NOT safe:

        // BLOCKING =====================
        // Update Temperature Measurements
        // Check if temperature is NOT safe:

        // Control Fans depending on temperature
    }
}

void Task_AmperesMonitor(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // BLOCKING =====================
        // Update Amperes Measurements
        // Check if amperes is NOT safe:
    }
}

void Task_DashboardNotify(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    // Create Motor Enable CAN Msg
    // Push message to CAN Q
}

void Task_LogInfo(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
}

void Task_CANBusConsumer(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // BLOCKING =====================
        // Wait for CAN Q to have message

        // BLOCKING =====================
        // Transmit to CAN Bus
    }
}

void Task_BatteryBalance(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // Todo: Battery balancing
    }
}

void Task_CLI(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // BLOCKING =====================
        // Wait for command

        // Handle command
    }
}

void Task_BLE(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    while(1) {
        // Todo: Add BLE API calls
    }
}

void Task_Idle(void *p_arg) {
    (void)p_arg;

    while(1) {
        // Todo: Do something cool or be lame.
    }
}
*/
