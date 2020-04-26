#include "BSP_Timer.h"

/**
 * @brief   Initialize the timer for time measurements.
 * @param   None
 * @return  None
 */
void BSP_Timer_Init(void) {
    // TODO: Initialize a regular timer with no interrupts
}

/**
 * @brief   Starts the timer.
 * @param   None
 * @return  None
 */
void BSP_Timer_Start(void) {
    // TODO: Start the timer
}

/**
 * @brief   Gets the number of ticks that has elapsed since the last time this function was called.
 * @param   None
 * @return  Number of ticks
 */
uint32_t BSP_Timer_GetTicksElapsed(void) {
    // TODO: return the number of ticks from this last function call to now
    return 0;
}

/**
 * @brief   Gets the running frequency of the timer (time per tick)
 * @param   None
 * @return  frequency in Hz
 */
uint32_t BSP_Timer_GetRunFreq(void) {
    return 16000000;
}
