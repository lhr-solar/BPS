// WDTimer.h
/**
 * Driver for the watch dog timer. Whenever the Watch Dog Timer reaches 0,
 * all contactors will be opened (the battery pack will be disconnected from the car).
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/4/2018
 */

#ifndef WDTIMER_H__
#define WDTIMER_H__

#include <stdint.h>

/** WDTimer_Init
 * Initializes the Watch Dog Timer. Does not start the timer
 */
void WDTimer_Init(void);

/** WDTimer_Start
 * Starts the Watch Dog Timer count down
 */
void WDTimer_Start(void);

/** WDTimer_Reset
 * Resets the Watch Dog Timer. Does not stop the counter.
 */
void WDTimer_Reset(void);

/** WDTimer_Stop
 * Stops the Watch Dog Timer. Does not reset the counter.
 */
void WDTimer_Stop(void);

/** WDTimer_FaultDetection
 *
 */
uint8_t WDTimer_FaultDetection(void);

#endif
