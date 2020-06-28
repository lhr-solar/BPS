#ifndef STATUSSYNC_H__
#define STATUSSYNC_H__

#include <stdint.h>
#include "config.h"
#include <stdbool.h>

/** TIM2_IRQHandler
 * sets flag to send safety message to Dashboard
 * called every half millisecond
 */
void TIM2_IRQHandler(void);

/** TIM2_Init
 * initializes timer 2
 */
void TIM2Init(void);

/** EnableTIM2Interrupt
 * enables interrupts on timer 2
 * called by TIM2Init()
 */
void EnableTIM2Interrupt(void);

/** DashMessage
 * Sends "safe" or "dangerous" CAN message based on if override mode is active or not
 *@param flag bool that determines if the motor should be running or not
 */
void DashMessage(bool overrideFlag);
 

/** int DashboardMessageFlag
 * 1 = message should be sent, 0 = message should not be sent
 */
extern volatile int DashboardMessageFlag;



#endif
