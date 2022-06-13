#include "BSP_Contactor.h"
#include "BSP_Fans.h"
#include "BSP_Lights.h"

/* This is a basic test to verify that our contactor
 * code and hardware work. This should start with the
 * contactor enabled, read the state of the contactor,
 * and then toggle the contactor state.
 */

int main() {
    BSP_Contactor_Init();   // Initialize the contactor
    BSP_Fans_Init();
    
    BSP_Fans_SetAll(TOPSPEED);

    for (volatile int i = 0; i < 1000000; ++i);

    BSP_Contactor_On();     // Enable the contactor
    

    while(1) {
        // Delay for some time before doing anything else
        /*for(volatile int i = 0; i < 1000000; i++);
    
        // Read the contactor state
        bool contactorEnabled = BSP_Contactor_GetState();
        
        // Toggle the contactor state
        if(contactorEnabled) {
            BSP_Contactor_Off();
        } else {
            BSP_Contactor_On();
        }*/
    }
}
