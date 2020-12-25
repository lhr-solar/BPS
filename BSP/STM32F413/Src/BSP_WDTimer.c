/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_WDTimer.h"
#include "stm32f4xx.h"

/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(void) {
    // Independent Watchdog Init
    // IWDG has a timeout value of 4.096 seconds.
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);       // Enable access to the IWDG_PR and IWDG_RLR registers
	IWDG_SetPrescaler(IWDG_Prescaler_32);               // Prescaler divider feeding the counter clock
	IWDG_SetReload(IWDG_RLR_RL);                        // Reload value set to 0xFFF
}

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void) {
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
        RCC_ClearFlag();		// Clear reset flags. May be optional
        return true;
    }
    return false;
}

/**
 * @brief   Start the watchdog timer countdown. Once this is started, the BSP_WDTimer_Reset
 *          must be called or else the system will reset.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Start(void) {
    IWDG_ReloadCounter();       // Reloads the Independent Watchdog Timer
    IWDG_Enable();              // Starts the watchdog
}

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void) {
    IWDG_ReloadCounter();       // Reloads the Independent Watchdog Timer
}
/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_WDTimer.h"
#include "stm32f4xx.h"

/**
 * @brief   Initialize the watch dog timer.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Init(void) {
    // Independent Watchdog Init
    // IWDG has a timeout value of 4.096 seconds.
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);       // Enable access to the IWDG_PR and IWDG_RLR registers
    IWDG_SetPrescaler(IWDG_Prescaler_32);               // Prescaler divider feeding the counter clock
    IWDG_SetReload(IWDG_RLR_RL);                        // Reload value set to 0xFFF
}

/**
 * @brief   Check if the watchdog made the uC reset previously.
 * @param   None
 * @return  true if a reset occurred previously, false if system started up normally.
 */
bool BSP_WDTimer_DidSystemReset(void) {
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET) {
        RCC_ClearFlag();        // Clear reset flags. May be optional
        return true;
    }
    return false;
}

/**
 * @brief   Start the watchdog timer countdown. Once this is started, the BSP_WDTimer_Reset
 *          must be called or else the system will reset.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Start(void) {
    IWDG_ReloadCounter();       // Reloads the Independent Watchdog Timer
    IWDG_Enable();              // Starts the watchdog
}

/**
 * @brief   Reset the watchdog countdown. This function must be called before the watchdog timer
 *          finishes counting down. Nothing will happen if BSP_WDTimer_Start was not called recently.
 * @param   None
 * @return  None
 */
void BSP_WDTimer_Reset(void) {
    IWDG_ReloadCounter();       // Reloads the Independent Watchdog Timer
}
