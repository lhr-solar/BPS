#include "BSP_Contactor.h"

/* This is a basic test to verify that our contactor
 * code and hardware work. This should start with the
 * contactor enabled, read the state of the contactor,
 * and then toggle the contactor state.
 */

int main() {
    BSP_Contactor_Init();   // Initialize the contactor

    BSP_Contactor_On(ALL_CONTACTORS);     // Enable the contactor

    while(1) {
        // Delay for some time before doing anything else
<<<<<<< Updated upstream
        for(volatile int i = 0; i < 1000000; i++);
    
        // Read the contactor state
        bool contactorEnabled = BSP_Contactor_GetState(ARRAY_CONTACTOR);
        
        // Toggle the contactor state
        if(contactorEnabled) {
            BSP_Contactor_Off(ALL_CONTACTORS);
        } else {
            BSP_Contactor_On(ALL_CONTACTORS);
        }
=======
        Contactor_On(HVHIGH_CONTACTOR_OUT);
        Contactor_Off(HVHIGH_CONTACTOR_OUT);
        for(volatile int i = 0; i < 10000000; i++);
        Contactor_Off(HVHIGH_CONTACTOR_OUT);
        Contactor_On(ARRAY_CONTACTOR_OUT);
        for(volatile int i = 0; i < 10000000; i++);

>>>>>>> Stashed changes
    }
}
