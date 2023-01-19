#ifndef BSP_PWM_H
#define BSP_PWM_H

#ifndef SIMULATION
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#endif
#include "config.h"
#include "common.h"

#define C1_PORT GPIOB //TO MAKE THE CONTACTOR CODE WORK, REMOVE ONCE IT HAS A NEW HOME. This is the port for the input pin

#define PWM_PERIOD 4000


/**
 * @brief   Initialize all the GPIO pins meant for pwm
 * @param   None
 * @return  None
 */
void BSP_PWM_Init(void);

/**
 * @brief   Sets a pin's duty cycle
 * @param   dutyCycle: duty cycle amount between 0 and the period
 * @param   pin: pin number whose speed should be changed
 * @return  ErrorStatus
 */
ErrorStatus BSP_PWM_Set(uint8_t pin, uint32_t speed);

/**
 * @brief   Get current duty cycle of a single pin, return -1 if input is invalid
 * @param   pin Number
 * @return  Current PWM duty cycle of pin
 */
int BSP_PWM_Get(uint8_t pin);

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @note	THIS IS ALSO CODE THAT HAS NO HOME. You cannot get the state of ALL_CONTACTORS. As such, if that param is passed, it will return the state of the array contactor.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool Contactor_Get(uint8_t contactorChoice);

#endif
