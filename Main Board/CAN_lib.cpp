#include "CAN_lib.h"

DigitalOut heartbeat(LED1);     // For Debugging

//******************************************************************************
// CAN_Interrupt_Handler constructor
CAN_Interrupt_Handler::CAN_Interrupt_Handler(PinName rx, PinName tx, int frequency) : canBus(rx, tx){
    canBus.frequency(frequency);
    canBus.attach(callback(this, &CAN_Interrupt_Handler::interruptRoutine), CAN::RxIrq);    // Initialize interrupt for whenever CAN receives message
    
    // Indexes for FIFO
    mailBoxGetIndex = 0;
    mailBoxPutIndex = 0;
}

//******************************************************************************
// interruptRoutine
// The following function is called whenever the CAN bus receives a new message
void CAN_Interrupt_Handler::interruptRoutine(){
    if(mailBoxPutIndex % CAN_STACK_SIZE + 1 != mailBoxGetIndex){
        CANMessage newCanMsg;       // declare empty message
        canBus.read(newCanMsg);     // place newly received CAN message into newCanMsg
        messageMailBox[mailBoxPutIndex++] = newCanMsg;
        
        // mailBoxPutIndex will return to 0 if index exceeds size of stack
        mailBoxPutIndex %= CAN_STACK_SIZE;
        
        // DEBUG
        heartbeat = !heartbeat;
    }else{
        // DEBUG FUNCTION
    }
}

//******************************************************************************
// getNextMessage
// The following function returns a true if there is an available message to be retrieved,
//    false if no messages available
// Input: reference parameter for CANMessage to be stored
// Output: true if message is returned, false if not
bool CAN_Interrupt_Handler::getNextMessage(CANMessage &canMsg){
    if(!isMailBoxEmpty()){
        canMsg = messageMailBox[mailBoxGetIndex++];
        
        // mailBoxGetIndex will return to 0 if index exceeds size of stack
        mailBoxGetIndex %= CAN_STACK_SIZE;
        
        return true;
    }else{
        return false;
    }
}

//******************************************************************************
// isMailBoxEmpty
// The following function returns if true if the mailbox is empty, false if not
// Input: None
// Output: true for empty mailbox, false if mailbox have messages
bool CAN_Interrupt_Handler::isMailBoxEmpty(){
    if(mailBoxPutIndex == mailBoxGetIndex){
        return true;
    }else{
        return false;
    }
}
