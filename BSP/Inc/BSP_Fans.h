#ifndef __BSP_FANS_H
#define __BSP_FANS_H

#include "common.h"



/**
 * @brief   Initialize all the GPIO pins connected to each Fan
 * @param   None
 * @return  None
 */
void BSP_Fans_Init(void);

/**
 * @brief   Sets a certain LED/Light
 * @param   signal : choose one of the Light enums to control that specific Light.
 * @return  None
 */
void BSP_Fans_Set(int dutyCycle);

/**
 * @brief   Turns fans off
 * @param   None
 * @return  None
 */
void BSP_Fans_Off(void);

/**
 * @brief   Get current duty cycle of fans
 * @param   None
 * @return  Current PWM duty cycle if fans
 */
int BSP_Fans_GetDuty(void);

#endif
