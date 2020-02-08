/* SysTick.c
 * SysTick wrapper that accesses the system ticker for 
 * clock and delay funcitonality
 */
#include "SysTick.h"
#include "stdio.h"

static volatile uint32_t usTicks;

/* SysTick_Handler
 * Called every us; updates the usTicks counter
 */
 //SystemCoreClock 
void SysTick_Handler() {
	if (usTicks != 0) {
		usTicks--;
	}
	else {
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // disable system clock if tick reaches 0
	}
}
/* DelayMs
 * Delay 'ms' amount of ms and then continues
 */
void DelayMs(uint32_t ms) {
	usTicks = ms * 500; //convert to microseconds, used 500 because 1000 was double in time
	//Initializing core clock configuration
	// Update SystemCoreClock value
	SystemCoreClockUpdate();
	// Configure the SysTick timer to overflow every 1 us
	SysTick_Config(SystemCoreClock / 1000000);
	while(usTicks); //wait for interrupts to reach 0
	}
