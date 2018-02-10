#ifndef CAN_LIB_H
#define CAN_LIB_H

#include "mbed.h"

#define CAN_STACK_SIZE 250

class CAN_Interrupt_Handler{
    
    public:
    CAN_Interrupt_Handler(PinName rx, PinName tx, int priority, int frequency= 100000, int mode= 1, int filterId= 0x600);
    bool getNextMessage(CANMessage &canMsg);
    bool isMailBoxEmpty();
    void setPriority(int priority);
    void setMode(int mode, int filterId= 0x600);
    
    private:
    void interruptRoutine();
    void findCANnumber(PinName rx, PinName tx);
    
    CAN canBus;
    CANMessage messageMailBox[CAN_STACK_SIZE + 1];  // FIFO stack of CAN messages
    int mailBoxGetIndex;
    int mailBoxPutIndex;
    int canNumber;      // holds canbus number for Interrupt number
    int priority;       // Priority of receiver interrupt
    int id;             // ID to filter during mode 2
    
    int mode;   // interrupt mode of CAN ports
    //  0 : Ignore all messages
    //  1 : Accept all messages
    //  2 : Filter through messages
    
    // DEBUG useful?
    //DigitalOut overFlowError;   // Heartbeat, led light up if overflow error of stack
};


#endif
