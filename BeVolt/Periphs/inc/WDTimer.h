/** WDTimer.h
 * Driver for the watch dog timer. Whenever the Watch Dog Timer reaches 0,
 * all contactors will be opened (the battery pack will be disconnected from the car).
 */

#ifndef WDTIMER_H__
#define WDTIMER_H__

#include <stdint.h>
#include "config.h"

/** WDTimer_Init
 * Initializes the Watch Dog Timer. Does not start the timer
 */
void WDTimer_Init(void);

/**
 * Checks if the system has resumed from a Watchdog reset.
 * If the watchdog timer previously made the system reset, we don't want
 * the BPS to keep running. We want to see what cause the system to freeze.
 * @return SAFE if system did not freeze, DANGER if system reset occurred
 */
SafetyStatus WDTimer_DidSystemReset(void);

/** WDTimer_Start
 * Starts the Watch Dog Timer count down
 */
void WDTimer_Start(void);

/** WDTimer_Reload
 * Resets the Watch Dog Timer. Does not stop the counter. Counts Up!!
 */
void WDTimer_Reset(void);

#endif
