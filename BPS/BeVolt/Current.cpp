#include "mbed.h"
#include "Current.h"

// datatype <class name>::<function>(inputs);


Current::Current(PinName highADC, PinName lowADC) : highPrec(highADC), lowPrec(lowADC){
    
}

int Current::getHighPrec(){
    
    return 0;
}
