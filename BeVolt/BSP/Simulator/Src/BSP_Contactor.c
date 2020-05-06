#include "BSP_Contactor.h"

const char* ContactorFile = "BSP/Simulator/DataGeneration/Data/ADC.csv";
/**
 * @brief   Initializes the GPIO pins that interfaces with the Contactor.
 *          Two GPIO pins are initialized. One as an output and one as an input.
 *          The output pin controls the state and the input pin views what state the contactor is through the
 *          Aux pins.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Init(void) {
    /* Hardware: Initialize two pins that will be connected to the Contactor. One must be configued as an input
          and the other must be configued as an output.
    Software: Create file that contains one unsigned integer, 0 means off, 1 means on 
    */
   FILE* ContactorFilePtr;
   if (ContactorFilePtr = fopen(ContactorFile, 'r')) fclose(ContactorFilePtr); //if file exists
   else{
       ContactorFilePtr = fopen(ContactorFile, 'w+'); //if it doesn't exist, create it and allow reading and writing
       fclose(ContactorFilePtr);
   }

}

/**
 * @brief   Closes the Contactor switch i.e. turns on the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully close.
 * @param   None
 * @return  None
 */
void BSP_Contactor_On(void) {
    // Hardware: Set the state to high for the output pin.
    //      Use Positive Logic.
    // Software: Set integer to 1
    FILE* ContactorFilePtr = fopen(ContactorFile, 'w'); //Open file to write
    fseek(ContactorFilePtr, 0, SEEK_SET); //Go to start of file
    fprintf(ContactorFilePtr, "%d", 1); //Write 1 to file

}

/**
 * @brief   Opens the Contactor switch i.e. turns off the whole electrical system.
 * @note    May be good in the future to make this return something if the contactor could not successfully open.
 * @param   None
 * @return  None
 */
void BSP_Contactor_Off(void) {
    // Hardware: Set the state to low for the output pin.
    // Software: Set integer to 0
    FILE* ContactorFilePtr = fopen(ContactorFile, 'w'); //Open file to write
    fseek(ContactorFilePtr, 0, SEEK_SET); //Go to start of file
    fprintf(ContactorFilePtr, "%d", 1); //Write 0 to file
}

/**
 * @brief   Gets the state of the Contactor switch from one of its AUX pins.
 * @param   None
 * @return  0 if contactor is off/open, 1 if on/closed
 */
bool BSP_Contactor_GetState(void) {
    // Hardware: Return if the Contactor is on or off.
    //      Check for negative logic. The gpio input pin can read low but the contactor could be on. Just make sure
    //      you check that your return value is the same value of what the contactor is irl.
    // Software: Read integer stored in file
    int ContactorState;
    FILE* ContactorFilePtr = fopen(ContactorFile, 'r'); //Open file to read
    ReadValfscanf(ContactorFilePtr, "%d", &ContactorState)
    return ContactorState;
}
