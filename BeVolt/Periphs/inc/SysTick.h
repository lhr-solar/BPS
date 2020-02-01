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

/** DelayInit
 * Initializes the core clock configuration
 */
void DelayInit(void);

/** DelayUs
 * Delays 'us' amount of us and then continues
 */
void DelayUs(uint32_t us);

/** DelayMs
 * Delay 'ms' amount of ms and then continues
 */
void DelayMs(uint32_t ms);
<<<<<<< HEAD
<<<<<<< HEAD

#endif 
=======
>>>>>>> Updated CLI with more explicit variables and documentation
=======
>>>>>>> 1348864e8fe5fd3561749cb6c0087fd5f40756db
