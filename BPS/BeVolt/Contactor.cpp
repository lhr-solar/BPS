#include "Contactor.h"

// class constructor
Contactor::Contactor(PinName out, PinName in) : signal(out), flag(in){
    signal = 0;
}

//******************************************************************************
// openContactor
// opens contactor by setting signal pin to 0
// Input: None
// Output: None
void Contactor::openContactor(){
    signal = 0;
}

//******************************************************************************
// closeContactor
// closes contactor by setting signal pin 1
// Input: None
// Output: None
void Contactor::closeContactor(){
    signal = 1;
}

//******************************************************************************
// getFlagState
// checks the flag if the contactor is operating correctly
// Input: None
// Output: returns 1 if functioning correctly, 0 if not
int Contactor::getFlagState(){
    return flag;
}

//******************************************************************************
// getSignalState
// checks if the contactor is on or off
// Input: None
// Output: returns 1 if closed, 0 if open
int Contactor::getSignalState(){
    return signal;
}
