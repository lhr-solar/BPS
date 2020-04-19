#ifndef __BSP_LIGHTS_H
#define __BSP_LIGHTS_H

#include "common.h"
#include "config.h"

/**
 * @note    This library also controls the strobe light.
 */

typedef enum {FAULT, RUN, UVOLT, OVOLT, OTEMP, OCURR, WDOG, CAN, EXTRA} Led;

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void);

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_Light_Toggle(Led signal);

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_Light_On(Led signal);

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
void BSP_Light_Off(Led signal);

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Led enums to control that specific LED.
 * @return  None
 */
State BSP_Light_GetState(Led signal);

#endif
