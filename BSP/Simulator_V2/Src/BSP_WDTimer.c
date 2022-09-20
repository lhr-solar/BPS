/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_WDTimer.h"
#include <stdint.h>


/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(void) {
    // TODO
}

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void){
    // TODO
}


/**
 * @brief   Start the watchdog timer countdown. Once this is started, the BSP_WDTimer_Reset
 *          must be called or else the system will reset.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Start(void) {
    // TODO
}

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void) {
    // TODO
}



