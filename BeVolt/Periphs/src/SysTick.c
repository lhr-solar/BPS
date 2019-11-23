/* SysTick.c
 * SysTick wrapper that accesses the system ticker for 
 * clock and delay funcitonality
 */
#include "SysTick.h"

static volatile uint32_t usTicks;

/* SysTick_Handler
 * Called every us; updates the usTicks counter
 */
void SysTick_Handler() {
	if (usTicks != 0) {
		usTicks--;
	}
}

/* DelayInit
 * Initializes the core clock configuration
 */
void DelayInit() {
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000000);
}

/* DelayUs
 * Delays 'us' amount of us and then continues
 */
void DelayUs(uint32_t us) {
	// Reload us value
	usTicks = us;
	// Wait until usTick reach zero
	while (usTicks);
}

/* DelayMs
 * Delay 'ms' amount of ms and then continues
 */
void DelayMs(uint32_t ms) {
	// Wait until ms reach zero
	while (ms--) {
		// Delay 1 ms using DelayUs
		DelayUs(1000);
	}
}
