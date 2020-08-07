#include "BSP_Contactor.h"

/**
 * @brief   Initializes the GPIO pins that interfaces with the Contactor.
 *          Two GPIO pins are initialized. One as an output and one as an input.
 *          The output pin controls the state and the input pin views what state the contactor is through the
 *          Aux pins.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Init(void) {
    // TODO: Initialize two pins that will be connected to the Contactor. One must be configued as an input
    //      and the other must be configued as an output.
}

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(void) {
    // TODO: Set the state to high for the output pin.
    //      Use Positive Logic.
}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(void) {
    // TODO: Set the state to low for the output pin.
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(void) {
    // TODO: Return if the Contactor is on or off.
    //      Check for negative logic. The gpio input pin can read low but the contactor could be on. Just make sure
    //      you check that your return value is the same value of what the contactor is irl.
    return 0;
}