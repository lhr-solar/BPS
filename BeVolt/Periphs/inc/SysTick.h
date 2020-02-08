/* SysTick.c
 * SysTick file that accesses the system ticker for 
 * clock and delay funcitonality
 */
#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include "config.h"

/** SysTick_Handler
 * Called every us; updates the usTicks counter
 */
void SysTick_Handler(void);

/* DelayMs
 * Delay 'ms' amount of ms and then continues
 */
void DelayMs(uint32_t ms);

#endif
