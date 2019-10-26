/** WDTimer.c
 * Driver for the watch dog timer. Whenever the Watch Dog Timer reaches 0,
 * all contactors will be opened (the battery pack will be disconnected from the car).
 */
#include "WDTimer.h"
#include "stm32f4xx.h"

/** 
 * Window Watchdog is used for its interrupt feature, and ceiling (if timer is reloaded too fast it may interrupt. Not set up, but it is an option)
 * Independent watchdog (separate clock) serves as back up to the Window Watchdog to reset but not to open the contactor. IWDG has a timeout value of 4.096 seconds.
 * A system reset sets all registers to their reset values except the reset flags in the clock controller CSR register and the registers in the Backup domain.
 */

/** WDTimer_Init
 * Initializes the Watch Dog Timer. Does not start the timer
 */
void WDTimer_Init(void){
	/* Independent Watchdog Init */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);           // Enable access to the IWDG_PR and IWDG_RLR registers
	IWDG_SetPrescaler(IWDG_Prescaler_32);                  // Prescaler divider feeding the counter clock
	IWDG_SetReload(IWDG_RLR_RL);                            // Reload value set to 0xFFF
}

/**
 * Checks if the system has resumed from a Watchdog reset.
 * If the watchdog timer previously made the system reset, we don't want
 * the BPS to keep running. We want to see what cause the system to freeze.
 * @return SAFE if system did not freeze, DANGER if system reset occurred
 */
SafetyStatus WDTimer_DidSystemReset(void){
	if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET){
		RCC_ClearFlag();		// Clear reset flags. May be optional
		return DANGER;
  }
	return SAFE;
}

/** WDTimer_Start
 * Starts the Watch Dog Timer count down
 */
void WDTimer_Start(void){
	IWDG_ReloadCounter();			// Reloads the Independent Watchdog Timer
	IWDG_Enable();							// Starts the watchdog
}

/** WDTimer_Reload
 * Reloads the Watch Dog Timer. Does not stop the counter.
 */
void WDTimer_Reset(void){
	IWDG_ReloadCounter();			// Reloads the Independent Watchdog Timer
}
