#ifndef StatusSync_H_
#define StatusSync_H_

#include <stdint.h>
#include "config.h"
#include <stdbool.h>

void TIM2_IRQHandler(void);
void TIM2Init(void);
void EnableTIM2Interrupt(void);
 

extern volatile int DashboardMessageFlag;



#endif
