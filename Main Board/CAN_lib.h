#ifndef CAN_LIB_H
#define CAN_LIB_H

#include "mbed.h"

#define CAN_STACK_SIZE 250

class CAN_Interrupt_Handler{
    
    public:
    CAN_Interrupt_Handler(PinName rx, PinName tx, int frequency= 100000);
    bool getNextMessage(CANMessage &canMsg);
    bool isMailBoxEmpty();
    
    private:
    void interruptRoutine();
    
    CAN canBus;
    CANMessage messageMailBox[CAN_STACK_SIZE + 1];    // FIFO stack of CAN messages
    int mailBoxGetIndex;
    int mailBoxPutIndex;
    
    // DEBUG useful?
    //DigitalOut overFlowError;   // Heartbeat, led light up if overflow error of stack
};


#endif
