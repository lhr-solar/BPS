/** Contactor.h
 * Sets Contactor on or off
 * Checks if flag is high or low. If high, return 1, if low, return 0.
 */

#include <stdint.h>
#include "config.h"

#ifndef CONTACTOR_H__
#define CONTACTOR_H__

/** Contactor_Init
 * Initialize Pin 0 as Output
 * Initialize Pin 1 as Input
 */
void Contactor_Init(void);

/** Contactor_On
 * Closes contactor, GPIO_Pin_0 = 1
 */
void Contactor_On(void);

/** Contactor_Off
 * Opens contactor, GPIO_Pin_0 = 0
 */
void Contactor_Off(void);
 
/** Contactor_Flag
 * Checks the current status of the contactor
 * @return ENABLE or DISABLE
 */
FunctionalState Contactor_Flag(void);

#endif
