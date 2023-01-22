/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */
#include "Contactor.h"

/* This is a basic test to verify that our contactor
 * code and hardware work. This should start with the
 * contactor enabled, read the state of the contactor,
 * and then toggle the contactor state.
 */

int main() {
    Contactor_Init();   // Initialize the contactor

    Contactor_On(ALL_CONTACTORS);     // Enable the contactor

    while(1) {
        // Delay for some time before doing anything else
        for(volatile int i = 0; i < 1000000; i++);
    
        // Read the contactor state
        bool contactorEnabled = Contactor_GetState(ARRAY_CONTACTOR);
        
        // Toggle the contactor state
        if(contactorEnabled) {
            Contactor_Off(ALL_CONTACTORS);
        } else {
            Contactor_On(ALL_CONTACTORS);
        }
    }
}
