#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "common.h"

/**
 * @note    This library also controls the strobe light.
 */

typedef enum {ERROR, RUN, UVOLT, OVOLT, OTEMP, OCURR, WDOG, CAN, EXTRA} Led;

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_LEDs_Init(void);

/**
 * @brief   Toggles a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_Toggle(Led signal);

/**
 * @brief   Turns on a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_On(Led signal);

/**
 * @brief   Turns off a certain LED
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_LED_Off(Led signal);

#endif
