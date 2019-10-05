/** WDTimer.c
 * Driver for the watch dog timer. Whenever the Watch Dog Timer reaches 0,
 * all contactors will be opened (the battery pack will be disconnected from the car).
 */
#include "WDTimer.h"
#include "Contactor.h"
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
	/* Window Watchdog Init */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);    // Initializes WWDG clock at APB1 Bus speed
	WWDG_EnableIT();                                        // Enables the WWDG Early Wakeup interrupt(EWI)
	WWDG_SetCounter(WWDG_CFR_W);                            // Sets the WWDG counter value. 
	WWDG_SetPrescaler(WWDG_Prescaler_8);                    // Sets the WWDG Prescaler.
	WWDG_SetWindowValue(WWDG_CFR_W);                        // Sets the WWDG window value.
	
	/* Independent Watchdog Init */
	FLASH_OB_Unlock();                                      // Enable the FLASH option control register access
	FLASH_OB_UserConfig(OB_IWDG_SW, OB_STOP_NoRST, OB_STDBY_NoRST); // Confiuge to software IWDG, no reset during stop and standby mode
	FLASH_OB_Launch();                                      // Launch the Option Bytes programming process
	FLASH_OB_Lock();                                        // Disable the FLASH option control register
	RCC_LSICmd(ENABLE);                                     // Initializes LSI clock at 32KHz used for IWDG
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);           // Enable access to the IWDG_PR and IWDG_RLR registers
	IWDG_SetPrescaler(IWDG_Prescaler_32);                  // Prescaler divider feeding the counter clock
	IWDG_SetReload(IWDG_RLR_RL);                            // Reload value set to 0xFFF
}

/** WDTimer_Start
 * Starts the Watch Dog Timer count down
 */
void WDTimer_Start(void){
	WWDG_Enable(WWDG_CFR_W);                     // Activates Window Watchdog
	IWDG_Enable();                               // Starts the watchdog
}

/** WDTimer_Reload
 * Reloads the Watch Dog Timer. Does not stop the counter.
 */
void WDTimer_Reset(void){
	WWDG_Enable(WWDG_CFR_W);                    // Reloads the Window Watchdog Timer
	IWDG_ReloadCounter();                       // Reloads the Independent Watchdog Timer
}

/** WWDG_IRQHandler
 * Interrupts every 41.94304 ms when running at APB1 Bus Speed of 50MHz
 */
void WWDG_IRQHandler(void){
	Contactor_Off();                             // Fault: open contactor
	WWDG_ClearFlag();                            // Clear EWI Flag
}
