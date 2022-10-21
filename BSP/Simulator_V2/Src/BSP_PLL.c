/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_PLL.h"
#include "Simulator.h"
#include <stdint.h>
#include <stdlib.h>

static uint32_t SystemCoreClock = 16000000;
/**
 * @brief   Initialize the PLL so the system core frequency runs at your preferred frequency.
 * @param   None
 * @return  None
 */
void BSP_PLL_Init(void) {
    SystemCoreClock = 80000000;
    Simulator_log(LOG, "Initialized PLL\n");
}

/**
 * @brief   Gets the system core clock frequency that was set by PLL_Init
 * @param   None
 * @return  System core clock frequency in Hz
 */
uint32_t BSP_PLL_GetSystemClock(void) {
    return SystemCoreClock;
}
