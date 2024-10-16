/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#ifndef BSP_WDTIMER_H
#define BSP_WDTIMER_H

#include "common.h"

/**
 * @brief   Initialize the watch dog timer. If in fault state, set watchdog reload to max value so we don't trip for watchdog in fault state again.
 * @param   fault: true if in fault state, false otherwise
 * @return  None
 */
void BSP_WDTimer_Init(bool fault);

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void);

/**
 * @brief   Start the watchdog timer countdown. Once this is started, the BSP_WDTimer_Reset
 *          must be called or else the system will reset.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Start(void);

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void);

#endif
