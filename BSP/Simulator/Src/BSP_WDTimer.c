/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

/**
 * BSP_WDTimer.c - Simulates Watchdog timer of BPS. NOTE: Should not be used to definitively test
 * timing capabilities of BPS.
*/

#include "BSP_WDTimer.h"
#include <stdint.h>
#include "Simulator.h"

volatile bool initialized = false;

/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(bool fault) {
    initialized = true;
    char* str = fault ? "Initialized the watchdog timer in fault state\n" : "Initialized the watchdog timer\n";
    Simulator_Log(LOG_INFO, str);
}

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void){
    // TODO
    return false;
}


/**
 * @brief   Start the watchdog timer countdown. Once this is started, the BSP_WDTimer_Reset
 *          must be called or else the system will reset.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Start(void) {
    if (!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used watchdog timer without initialization!\n");
        Fault_BitMap = Fault_WDOG;
        EnterFaultState();
    }
    Simulator_Log(LOG_INFO, "Started the watchdog timer\n");
}

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void) {
    if (!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used watchdog timer without initialization!\n");
        Fault_BitMap = Fault_WDOG;
        EnterFaultState();
    }

    Simulator_Log(LOG_INFO, "Reset the watchdog timer\n");
}
