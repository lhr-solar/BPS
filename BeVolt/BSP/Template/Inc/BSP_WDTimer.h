#ifndef __BSP_WDTIMER_H
#define __BSP_WDTIMER_H

#include "common.h"

void BSP_WDTimer_Init(void);

bool BSP_WDTimer_DidSystemReset(void);

void BSP_WDTimer_Start(void);

void BSP_WDTimer_Reset(void);

#endif
