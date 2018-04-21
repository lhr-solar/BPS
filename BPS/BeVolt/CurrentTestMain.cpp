#include "mbed.h"
#include "Current.h"

// datatype var = <object name>.<function>(inputs);

Current CurrentSlave(PA_5, PA_4);

int main(){
    int value = CurrentSlave.getHighPrec();
    
}

