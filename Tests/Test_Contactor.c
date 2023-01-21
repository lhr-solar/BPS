#include "Contactor.h"

/* This is a basic test to verify that our contactor
 * code and hardware work. This should start with the
 * contactor enabled, read the state of the contactor,
 * and then toggle the contactor state.
 */

int main() {
    Contactor_Init();   // Initialize the contactor

    while(1) {
        // Delay for some time before doing anything else
        Contactor_On(ARRAY_CONTACTOR);
        for(volatile int i = 0; i < 5000000; i++);
        Contactor_On(HVLOW_CONTACTOR);
        for(volatile int i = 0; i < 5000000; i++);
        Contactor_Off(ARRAY_CONTACTOR);
        for(volatile int i = 0; i < 5000000; i++);
        Contactor_Off(HVLOW_CONTACTOR);
        for(volatile int i = 0; i < 5000000; i++);
    }
}
