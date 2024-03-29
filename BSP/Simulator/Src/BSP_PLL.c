/* Copyright (c) 2022 UT Longhorn Racing Solar */
/**
 * BSP_PLL.c - Simulates PLL on BPS board.
*/

#include "BSP_PLL.h"
#include "Simulator.h"
#include <stdint.h>
#include <stdlib.h>

static uint32_t SystemCoreClock = 16000000;
static bool initialized = false;
/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void) {
    SystemCoreClock = 80000000;
    initialized = true;
    Simulator_Log(LOG_INFO, "Initialized PLL\n");
}

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void) {
    if(!initialized) {
        Simulator_Log_Location(LOG_ERROR, "Used PLL without initialization!\n");
        exit(-1);
    }
    return SystemCoreClock;
}

/**
 * @brief   Delays in param micro seconds
 * @param   # of microseconds to delay
 * @return  None
*/
void BSP_PLL_DelayUs(uint32_t micro)
{
    return;
}

/**
 * @brief   Delays in param milli seconds
 * @param   # of milliseconds to delay
 * @return  None
*/
void BSP_PLL_DelayMs(uint32_t milli)
{
    return;
}
