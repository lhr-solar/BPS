//#include "Simulator.h"
#include "Amps.h"
#include "common.h"
#include "config.h"
#include "CANbus.h"
#include "os.h"
#include "Tasks.h"
#ifndef SIMULATION
#include "stm32f4xx.h"
#else
#include "Simulator.h"
#endif
#include "BSP_Lights.h"
#include "BSP_PLL.h"
#include "CAN_Queue.h"
#include "BSP_WDTimer.h"
#include "BSP_Contactor.h"

/**
 * @brief Test for getting current
 * Call Amps_UpdateMeasurements() anytime to get current reading based on JSON file
*/
OS_TMR tmr;
OS_SEM tmr_Sem;

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[256];

OS_TCB Task1_TCB;
CPU_STK Task1_Stk[DEFAULT_STACK_SIZE];

OS_TCB Task2_TCB;
CPU_STK Task2_Stk[DEFAULT_STACK_SIZE];

OS_ERR p_err;
void EnterFaultState(void);


int main(int argc, char **argv) {
    Simulator_Init(argv[1]);
    BSP_PLL_Init();
    
    Amps_Init();
    Amps_UpdateMeasurements();
    return 0;
}
