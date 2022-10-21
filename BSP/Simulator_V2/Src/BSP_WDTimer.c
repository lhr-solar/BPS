/* Copyright (c) 2022 UT Longhorn Racing Solar */

/**
 * BSP_WDTimer.c - Simulates Watchdog timer of BPS. NOTE: Should not be used to definitively test
 * timing capabilities of BPS.
*/

#include "BSP_WDTimer.h"
#include <stdint.h>
#include "Simulator.h"

bool initialized = false;

/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(void) {
    initialized = true;
    Simulator_log(LOG, "Initialized the watchdog timer\n");
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
        Simulator_log(ERROR, "Used watchdog timer without initialization!\n");
        exit(-1);
    }


    Simulator_log(LOG, "Started the watchdog timer\n");
}

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void) {
    if (!initialized) {
        Simulator_log(ERROR, "Used watchdog timer without initialization!\n");
        exit(-1);
    }

    Simulator_log(LOG, "Reset the watchdog timer\n");
}
