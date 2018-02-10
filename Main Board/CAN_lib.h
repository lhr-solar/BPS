#ifndef CAN_LIB_H
#define CAN_LIB_H

#include "mbed.h"

#define CAN_STACK_SIZE 250

class CAN_Interrupt_Handler{
    
    public:
    CAN_Interrupt_Handler(PinName rx, PinName tx, int priority, int frequency= 100000);
    bool getNextMessage(CANMessage &canMsg);
    bool isMailBoxEmpty();
    void setPriority(int priority);
    
    private:
    void interruptRoutine();
    void findCANnumber(PinName rx, PinName tx);
    
    CAN canBus;
    CANMessage messageMailBox[CAN_STACK_SIZE + 1];  // FIFO stack of CAN messages
    int mailBoxGetIndex;
    int mailBoxPutIndex;
    int canNumber;      // holds canbus number 
    int priority;       // Priority of receiver interrupt
    
    // DEBUG useful?
    //DigitalOut overFlowError;   // Heartbeat, led light up if overflow error of stack
};


#endif
