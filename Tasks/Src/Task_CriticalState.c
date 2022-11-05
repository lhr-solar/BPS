/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "os.h"
#include "config.h"
#include "BSP_Contactor.h"
#include "CANbus.h"
#include "Tasks.h"
#include "BSP_UART.h"
#include "Voltage.h"
#include "CAN_Queue.h"


void Task_CriticalState(void *p_arg) {
    (void)p_arg;

    OS_ERR err;
    CPU_TS ts;
    CANMSG_t CANMSG;
    CANPayload_t CanPayload; 
    CanPayload.idx    = 0;
    CanPayload.data.b = 1;

    // BLOCKING =====================
    // Wait until voltage, open wire, temperature, and current(Amperes) are all checked and safe
    for (int i = 0; i < NUM_FAULT_POINTS; i++){
        OSSemPend(&SafetyCheck_Sem4, 0, OS_OPT_PEND_BLOCKING, &ts, &err);
    }

    // launch watchdog task
    OSTaskCreate(&PetWDog_TCB,              // TCB
                "TASK_PETWDOG_PRIO",        // Task Name (String)
                Task_PetWDog,               // Task function pointer
                (void *)0,                  // Task function args
                TASK_PETWDOG_PRIO,          // Priority
                PetWDog_Stk,                // Stack
                WATERMARK_STACK_LIMIT,      // Watermark limit for debugging
                TASK_PETWDOG_STACK_SIZE,    // Stack size
                0,                          // Queue size (not needed)
                10,                         // Time quanta (time slice) 10 ticks
                (void *)0,                  // Extension pointer (not needed)
                OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,  // Options
                &err);                      // return err code
	assertOSError(err);

    // Turn Contactor On
    BSP_Contactor_On(ALL_CONTACTORS);

    // launch check contactor task
    OSTaskCreate(&CheckContactor_TCB,                       // TCB
				"Task_CheckContactor",                      // Task Name (String)
				Task_CheckContactor,                        // Task function pointer
				(void *)0,                                  // Task function args
				TASK_CHECK_CONTACTOR_PRIO,                  // Priority
				CheckContactor_Stk,                         // Stack
				WATERMARK_STACK_LIMIT,                      // Watermark limit for debugging
				TASK_CHECK_CONTACTOR_STACK_SIZE,            // Stack size
				0,                                          // Queue size (not needed)
				10,                                         // Time quanta (time slice) 10 ticks
				(void *)0,                                  // Extension pointer (not needed)
				OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP,  // Options
				&err);                                      // return err code
    assertOSError(err);

    // Push All Clear message to CAN Q
    CANMSG.id = ALL_CLEAR;
    CANMSG.payload = CanPayload;
    CAN_Queue_Post(CANMSG);
    // Push Contactor State message to CAN Q
    CANMSG.id = CONTACTOR_STATE;
    CAN_Queue_Post(CANMSG);
    OSTaskDel(NULL, &err); // Delete task
    
}
