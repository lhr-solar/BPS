#include "mbed.h"
#include "Contactor.h"

int main2(){
    Contactor batteryContactor(PC_5, PC_6);
    DigitalOut testLed(LED1);
    batteryContactor.closeContactor();
    if(batteryContactor.getSignalState()){
        testLed = 1;
    }
    wait(2);
    testLed = 0;
    wait(2);
    batteryContactor.openContactor();
    if(!batteryContactor.getSignalState()){
        testLed = 1;
    }
    while(1){};
}

int main1() {
    Contactor batteryContactor(LED1, PC_6);
    while(1){
        batteryContactor.openContactor();
        wait(.2);
        batteryContactor.closeContactor();
        wait(.2);
    }
}
