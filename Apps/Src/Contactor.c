/* Copyright (c) 2018-2022 UT Longhorn Racing Solar */

#include "Contactor.h"

/**
 * @brief Initializes all Contactor pins used by the BPS
 * @param none
 * @return none
 */
void Contactor_Init(void) {
    //Contactor Init is called before fan init, so initialize pwm here first. This will setup the contactor outputs
    #ifdef SIMULATION
        Simulator_Log(LOG_INFO, "Contactor Initialized\n");
    #endif
    BSP_PWM_Init();
}

/**
 * @brief   Closes the specified Contactor switch(es) i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   Contactor to turn on
 * @return  None
 */
void Contactor_On(uint8_t contactorChoice) {
    // set output pins to start outputing with a duty cycle set by PWM_ON_TIME in the header file
    BSP_PWM_Set(contactorChoice, PWM_ON_TIME);
}

/**
 * @brief   Opens the specified Contactor switch(es) i.e. turns off the whole electrical system.
 * @param   Contactor to turn off
 * @return  None
 */
void Contactor_Off(uint8_t contactorChoice) {
    // set output pin to a duty cycle of 0
    BSP_PWM_Set(contactorChoice, 0);
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @note	If you pass in All_Contactors, you will get the state as true if any of the contactors are on.
 * @param   Contactor to get state of
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool Contactor_GetState(uint8_t contactorChoice) {
    return BSP_PWM_Get(contactorChoice);
}
