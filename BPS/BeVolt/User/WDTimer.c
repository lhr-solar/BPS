// WDTimer.cpp
/**
 * Driver for the watch dog timer. Whenever the Watch Dog Timer reaches 0,
 * all contactors will be opened (the battery pack will be disconnected from the car).
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/4/2018
 */
 #include "WDTimer.h"


/* 
A system reset sets all registers to their reset values except the reset flags in the clock
controller CSR register and the registers in the Backup domain
*/




 /** WDTimer_Init
 * Initializes the Watch Dog Timer. Does not start the timer
 */
void WDTimer_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);    // Initiliazes WWDG clock at APB1 Bus speed
    WWDG_CFR = WWDG_CFR_W | WWDG_CFR_WDGTB | WWDG_CFR_EWI;

 }

 /** WDTimer_Start
 * Starts the Watch Dog Timer count down
 */
void WDTimer_Start(void){
    WWDG_CR = WWDG_CR_WDGA | WWDG_CR_T;                     // Activates Watchdog
    // Warning: when writing to the WWDG_CR register, always write 1 in the T6 bit to avoid generating an immediate reset. 
 }

 /** WDTimer_Reload
 * Reloads the Watch Dog Timer. Does not stop the counter.
 */
void WDTimer_Reload(void){
    WWDG_CR = WWDG_CR_WDGA | WWDG_CR_T;                    // Reloads the Watchdog Timer
 }

 /** WDTimer_Freeze
 * Freezes the Watch Dog Timer. Does not reset the counter.
 */
void WDTimer_Freeze(void){
    WWDG_CR = WWDG_CR_T6;                                   // Disables watchdog without generating a reset.
 }

 /** WWDG_IRQHandler
 *
 */
uint8_t WWDG_IRQHandler(){

    if (/*IWDG = WWDG*/){
        /* reset */
    }
    else{
        /* don't reset*/
    }
    WWDG_SR = 0;                            // Clear EWI Flag
}