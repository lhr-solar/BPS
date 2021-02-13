/* Copyright (c) 2020 UT Longhorn Racing Solar */
#include "common.h"
#include "config.h"
#include "os.h"
#include "Tasks.h"
#include "Amps.h"
#include "Voltage.h"
#include "Temperature.h"

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
OS_TCB Init_TCB;

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
CPU_STK Init_Stk[TASK_INIT_STACK_SIZE];

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
uint32_t Fault_BitMap = 0;

void EnterFaultState(void);

/**
 * Used to assert if there has been an error in one of the OS functions
 * Kills the car if there is an OS error
 **/
void assertOSError(OS_ERR err){
    if(err != OS_ERR_NONE) {
        OSSemPost(&Fault_Sem4, OS_OPT_POST_1, &err);
    
        // We should not get to this point if the call above worked.
        // Thus, we need to manually enter a fault state, since the
        // OS obviously is not functioning correctly.
        EnterFaultState();
    }
}
