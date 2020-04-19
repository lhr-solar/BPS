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
    
}

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(void) {

}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(void) {

}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(void) {

}
