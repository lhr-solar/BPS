#ifndef CONTACTOR_H
#define CONTACTOR_H

#include "mbed.h"

class Contactor{
    public:
    Contactor(PinName out, PinName in);
    void openContactor();
    void closeContactor();
    int getFlagState();		// returns status of flag
    int getSignalState();  	// returns status of signal
    
    private:
    DigitalOut signal;
    DigitalIn flag;
};



#endif