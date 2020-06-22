#ifndef __BSP_STROBELIGHT_H
#define __BSP_STROBELIGHT_H

#include "common.h"
#include "config.h"

/**
 * @brief Initialize all GPIO pins connected to Strobe
 * @param None
 * @return None
 */
 void BSP_Strobe_Init(void);

/**
 * @brief   Turns on Strobe (pulse once per second)
 * @param   None
 * @return  None
 */
void BSP_Strobe_On(Light signal);

/**
 * @brief   Turns off Strobe
 * @param   None
 * @return  None
 */
void BSP_Strobe_Off(Light signal);

#endif