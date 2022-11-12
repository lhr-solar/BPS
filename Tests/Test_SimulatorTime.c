#include "Simulator.h"
#include "BSP_Timer.h"
#define SIM_TIME_TEST_DELAY 1000000
/**
 * @brief Timer Sim Test
 * Simple test to test timer functions
 * Uses a while loop to delay and grab ticks elasped and microseconds elasped
 * change SIM_TIME_TEST_DELAY to test different delays
 */
int main(int argc, char **argv) {
    Simulator_Init(argv[1]);
    int delay = SIM_TIME_TEST_DELAY;
    BSP_Timer_Init();
    //BSP_Timer_GetTicksElasped();
    BSP_Timer_GetMicrosElapsed();
    while (delay > 0) {
        delay--;
    }
    //BSP_Timer_GetTicksElapsed();
    BSP_Timer_GetMicrosElapsed();
    return 0;
}
