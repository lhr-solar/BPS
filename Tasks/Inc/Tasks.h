/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#ifndef TASKS_H
#define TASKS_H

#include "FreeRTOS.h"

#define TASK_INIT_PRIO                      0
#define TASK_PETWDOG_PRIO                   1
#define TASK_CHECK_CONTACTOR_PRIO           2
#define TASK_CANBUS_CONSUMER_PRIO           3
#define TASK_VOLT_TEMP_MONITOR_PRIO         4
#define TASK_AMPERES_MONITOR_PRIO           5
#define TASK_BATTERY_BALANCE_PRIO           6
#define TASK_LOG_INFO_PRIO                  7 
#define TASK_CLI_PRIO                       8  
#define TASK_IDLE_PRIO                      9

#define DEFAULT_STACK_SIZE                  256
#define WATERMARK_STACK_LIMIT               DEFAULT_STACK_SIZE/2

#define TASK_PETWDOG_STACK_SIZE             DEFAULT_STACK_SIZE
#define TASK_VOLT_TEMP_MONITOR_STACK_SIZE   512
#define TASK_AMPERES_MONITOR_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_LOG_INFO_STACK_SIZE            DEFAULT_STACK_SIZE
#define TASK_CANBUS_CONSUMER_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_CHECK_CONTACTOR_STACK_SIZE     DEFAULT_STACK_SIZE
#define TASK_BATTERY_BALANCE_STACK_SIZE     512
#define TASK_CLI_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_BLE_STACK_SIZE                 DEFAULT_STACK_SIZE
#define TASK_IDLE_STACK_SIZE                DEFAULT_STACK_SIZE
#define TASK_INIT_STACK_SIZE                DEFAULT_STACK_SIZE

/**
 * enum to keep track of which tasks correspond to each bit in WDog_BitMap
 */
typedef enum {
    WD_NONE = 0x0,
    WD_VOLT_TEMP = 0x1,
    WD_AMPERES = 0x2,
    WD_BALANCING = 0x4,
} WDOGBits_e;

void Task_Init(void *p_arg);

void Task_PetWDog(void *p_arg);

void Task_VoltTempMonitor(void *p_arg);

void Task_AmperesMonitor(void *p_arg);

void Task_LogInfo(void *p_arg);

void Task_CANBusConsumer(void *p_arg);

void Task_CheckContactor(void *p_arg);

void Task_BatteryBalance(void *p_arg);

void Task_CLI(void *p_arg);

void Task_BLE(void *p_arg);

void Task_Idle(void *p_arg);

void Task_Init(void *p_arg);

void EnterFaultState();


/**
 * Used to assert if there has been an error in one of the OS functions
 * Kills the car if there is an OS error
 **/
void assertOSError(BPS_OS_ERR err);

/**
 * Used to Enter Fault State in case OS is not working
 **/

void EnterFaultState(void);

/*
 * Thread Control Blocks that contain information about each thread
 */
extern StaticTask_t FaultState_TCB;
extern StaticTask_t PetWDog_TCB;
extern StaticTask_t VoltTempMonitor_TCB;
extern StaticTask_t AmperesMonitor_TCB;
extern StaticTask_t LogInfo_TCB;
extern StaticTask_t CANBusConsumer_TCB;
extern StaticTask_t CheckContactor_TCB;
extern StaticTask_t BatteryBalance_TCB;
extern StaticTask_t CLI_TCB;
extern StaticTask_t BLE_TCB;
extern StaticTask_t Idle_TCB;
extern StaticTask_t Init_TCB;

/**
 * Stacks for each thread
 */
extern StackType_t PetWDog_Stk[TASK_PETWDOG_STACK_SIZE];
extern StackType_t VoltTempMonitor_Stk[TASK_VOLT_TEMP_MONITOR_STACK_SIZE];
extern StackType_t AmperesMonitor_Stk[TASK_AMPERES_MONITOR_STACK_SIZE];
extern StackType_t LogInfo_Stk[TASK_LOG_INFO_STACK_SIZE];
extern StackType_t CANBusConsumer_Stk[TASK_CANBUS_CONSUMER_STACK_SIZE];
extern StackType_t CheckContactor_Stk[TASK_CHECK_CONTACTOR_STACK_SIZE];
extern StackType_t BatteryBalance_Stk[TASK_BATTERY_BALANCE_STACK_SIZE];
extern StackType_t CLI_Stk[TASK_CLI_STACK_SIZE];
extern StackType_t BLE_Stk[TASK_BLE_STACK_SIZE];
extern StackType_t Idle_Stk[TASK_IDLE_STACK_SIZE];
extern StackType_t Init_Stk[TASK_INIT_STACK_SIZE];

/**
 *  Semaphores, Mutexes, & Bitmaps that are used by multiple threads
 */
extern SemaphoreHandle_t SafetyCheck_Sem4;
extern SemaphoreHandle_t WDog_Mutex;
extern SemaphoreHandle_t MinionsASIC_Mutex;
extern WDOGBits_e WDog_BitMap;
extern uint8_t Fault_Flag;

typedef enum {
    Fault_UVOLT   = 0x001,
    Fault_OVOLT   = 0x002,
    Fault_OTEMP   = 0x004,
    Fault_OCURR   = 0x008,
    Fault_OW      = 0x010,
    Fault_HANDLER = 0x020,
    Fault_OS      = 0x040,
    Fault_WDOG    = 0x080,
    Fault_CRC     = 0x100,
    Fault_ESTOP   = 0x200,
    FAULT_MAX
}Fault_Set;

extern Fault_Set Fault_BitMap;

#endif
