#include "Contactor.h"
#include "BSP_PLL.h"


/* This is a basic test to verify that our contactor
 * code and hardware work. This should start with the
 * contactor enabled, read the state of the contactor,
 * and then toggle the contactor state.
 */

int main() {
    BSP_PLL_Init();
    Contactor_Init();   // Initialize the contactor

    while(1) {
        // Delay for some time before doing anything else
        Contactor_On(ARRAY_CONTACTOR_OUT);
        for(volatile int i = 0; i < 5000000; i++);
        Contactor_On(HV_CONTACTOR_OUT);
        for(volatile int i = 0; i < 5000000; i++);
        Contactor_Off(ARRAY_CONTACTOR_OUT);
        for(volatile int i = 0; i < 5000000; i++);
        Contactor_Off(HV_CONTACTOR_OUT);
        for(volatile int i = 0; i < 5000000; i++);
    }
}
