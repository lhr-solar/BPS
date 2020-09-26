#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "Current.h"
#include "Voltage.h"
#include "Temperature.h"
#include "BSP_UART.h"

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

/**
 * Global Variables
 */
cell_asic Minions[NUM_MINIONS];
uint32_t WDog_BitMap = 0;


/** Pend_ResourceForCLI
 * Pends for desired resource that CLI needs
 * @param resourceNum the resource you want to wait for, look at CLI_Resource enum in CLI.h
 */
void Pend_ResourceForCLI(int resourceNum){
    switch(resourceNum){
        case 0:
            OS_Sem4Pend(&SafetyCheck_Sem4);
            break;
        case 1:
            OS_MutexLock(&MinionsASIC_Mutex);
            break;
        case 2:
            OS_MutexLock(&VoltageBuffer_Mutex);
            break;
        case 3:
            OS_MutexLock(&Temperature_Mutex);
            break;
        case 4:
            OS_MutexLock(&OpenWireBuffer_Mutex);
            break;
        case 5:
            OS_MutexLock(&AmperesData_Mutex);
            break;
        default:
            break;
    }
}

/** Post_ResourceForCLI
 * Posts desired resource that CLI needs
 * @param resourceNum the resource you want to post, look at CLI_Resource enum in CLI.h
 */
void Post_ResourceForCLI(int resourceNum){
    switch(resourceNum){
        case 0:
            OS_Sem4Post(&SafetyCheck_Sem4);
            break;
        case 1:
            OS_MutexUnlock(&MinionsASIC_Mutex);
            break;
        case 2:
            OS_MutexUnlock(&VoltageBuffer_Mutex);
            break;
        case 3:
            OS_MutexUnlock(&Temperature_Mutex);
            break;
        case 4:
            OS_MutexUnlock(&OpenWireBuffer_Mutex);
            break;
        case 5:
            OS_MutexUnlock(&AmperesData_Mutex);
            break;
        default:
            break;
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

    while(1) {
        // CLI
        // WDOG Reset
    }
}

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

void Check_ResourceForCLI(int resourceNum){
    switch(resourceNum){
        case 0:
            OS_Sem4Pend(&SafetyCheck_Sem4);
            break;
        case 1:
            OS_MutexLock(&MinionsASIC_Mutex);
            break;
        case 2:
            OS_MutexLock(&VoltageBuffer_Mutex);
            break;
        case 3:
            OS_MutexLock(&Temperature_Mutex);
            break;
        case 4:
            OS_MutexLock(&OpenWireBuffer_Mutex);
            break;
        case 5:
            OS_Sem4Pend(&MinionIO_Sem4);
            break;
        case 6:
            OS_Sem4Pend(&AmperesIO_Sem4);
            break;
        case 7:
            OS_MutexLock(&AmperesData_Mutex);
            break;
        default:
            break;
    }
}

void Post_ResourceForCLI(int resourceNum){
    switch(resourceNum){
        case 0:
            OS_Sem4Post(&SafetyCheck_Sem4);
            break;
        case 1:
            OS_MutexUnlock(&MinionsASIC_Mutex);
            break;
        case 2:
            OS_MutexUnlock(&VoltageBuffer_Mutex);
            break;
        case 3:
            OS_MutexUnlock(&Temperature_Mutex);
            break;
        case 4:
            OS_MutexUnlock(&OpenWireBuffer_Mutex);
            break;
        case 5:
            OS_Sem4Post(&MinionIO_Sem4);
            break;
        case 6:
            OS_Sem4Post(&AmperesIO_Sem4);
            break;
        case 7:
            OS_MutexUnlock(&AmperesData_Mutex);
            break;
        default:
            break;
    }
}

void Task_CLI(void *p_arg) {
    (void)p_arg;

    OS_ERR err;

    OS_Sem4Pend(&SafetyCheck_Sem4);
    //call OS_Sem4Pend() in whatever CLI function uses the specific semaphore

    initialize();
	__enable_irq();
	//Contactor_On();
	uint32_t fifo_size = 200;
    char command[fifo_size];
	CLI_Startup();
	
    while(1) {
        // BLOCKING =====================
        // Wait for command
<<<<<<< HEAD

        // Handle command
        UART3_CheckAndEcho(&CLIFifo);
		if (UART3_HasCommand(&CLIFifo)) {
			UART3_GetCommand(&CLIFifo, command);
			CLI_Handler(command);

void Task_BLE(void *p_arg) {
    (void)p_arg;


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
