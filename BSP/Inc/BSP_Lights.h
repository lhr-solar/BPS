/* Copyright (c) 2020 UT Longhorn Racing Solar */

#ifndef __BSP_LIGHTS_H
#define __BSP_LIGHTS_H

#include "common.h"
#include "config.h"

/**
 * @note    This library also controls the strobe light.
 */

typedef enum {RUN, UVOLT, OVOLT, OTEMP, OCURR, WDOG, CAN, EXTRA, WIRE, LIGHTS_MAX} Light;

/**
 * @brief   Initialize all the GPIO pins connected to each LED/Light
 * @param   None
 * @return  None
 */
void BSP_Lights_Init(void);

/**
 * @brief   Toggles a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Toggle(Light signal);

/**
 * @brief   Turns on a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_On(Light signal);

/**
 * @brief   Turns off a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Light_Off(Light signal);

/**
 * @brief   Gets the state of the LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  State of the LED/Light (ON/OFF)
 */
State BSP_Light_GetState(Light signal);

#endif
