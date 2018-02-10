#include "CAN_lib.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);     // For Debugging
Serial pcConnection(SERIAL_TX, SERIAL_RX);

//******************************************************************************
// CAN_Interrupt_Handler constructor
CAN_Interrupt_Handler::CAN_Interrupt_Handler(PinName rx, PinName tx, int priority, int frequency) : canBus(rx, tx){
    canBus.frequency(frequency);
    canBus.attach(callback(this, &CAN_Interrupt_Handler::interruptRoutine), CAN::RxIrq);    // Initialize interrupt for whenever CAN receives message
    
    // Set priority for interrupts
    findCANnumber(rx, tx);          // Deterine which CAN bus on F429ZI is being used to get interrupt number
    setPriority(this->priority);
        
    // Indexes for FIFO
    mailBoxGetIndex = 0;
    mailBoxPutIndex = 0;
    
    // DEBUG
    //pcConnection.baud(9600);
    //pcConnection.format(8, Serial::None, 1);
    
    led2 = !led2;
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
        if(led1 == 1){
            led1 = 0;
        }else{
            led1 = 1;
        }
        //Serial.printf("Yes");
        
    }else{
        // DEBUG
        // Serial.printf("No");
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

//******************************************************************************
// findCANnumber
// The following function finds which canbus is used from the pins
// Input: PinName of rx and tx
// Output: None
void CAN_Interrupt_Handler::findCANnumber(PinName rx, PinName tx){
    if((rx == PD_0 && tx == PD_1) || (rx == PB_8 && tx == PB_9)){
        canNumber = 1;
    }else if((rx == PB_12 && tx == PB_13) || (rx == PB_5 && tx == PB_6)){
        canNumber = 2;
    }else{
        canNumber = 0;  // pins are not right
    }
}

//******************************************************************************
// setPriority
// The followinng function sets the priority of CAN interrupt
// Input: 8-bit priority number
// Output: None
void CAN_Interrupt_Handler::setPriority(int priority){
    priority = this->priority;
    switch(canNumber){
        case 1:
            NVIC_SetPriority(CAN1_RX0_IRQn, priority);
            break;
        case 2:
            NVIC_SetPriority(CAN2_RX0_IRQn, priority);
            break;
        //default:
        //Not correct bus. Add more later
    }
}
