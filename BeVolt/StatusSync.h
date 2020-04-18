#ifndef STATUSSYNC_H__
#define STATUSSYNC_H__

#include <stdint.h>
#include "config.h"
#include <stdbool.h>

void TIM2_IRQHandler(void);
void TIM2Init(void);
void EnableTIM2Interrupt(void);
void DashMessage(bool overrideFlag);
 

extern volatile int DashboardMessageFlag;



#endif
