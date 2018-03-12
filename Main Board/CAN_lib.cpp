#include "CAN_lib.h"

// For Debugging
DigitalOut led1(LED1);
DigitalOut led2(LED2);
//Serial serial(SERIAL_TX, SERIAL_RX);

//******************************************************************************
// CAN_Interrupt_Handler constructor
CAN_Interrupt_Handler::CAN_Interrupt_Handler(PinName rx, PinName tx, int priority, int frequency, int mode, int filterId) : canBus(rx, tx){
    canBus.frequency(frequency);
    setMode(mode, filterId);    // attaches Handler depending on mode
    
    // Set priority for interrupts
    findCANnumber(rx, tx);      // Deterine which CAN bus on F429ZI is being used to get interrupt number
    setPriority(priority);
        
    // Indexes for FIFO
    mailBoxGetIndex = 0;
    mailBoxPutIndex = 0;
    
    // DEBUG
    //sserial.baud(9600);
    //serial.format(8, Serial::None, 1);
    
    //led1 = 1;
}

//******************************************************************************
// interruptRoutine
// Interrupts for all messages that is received
void CAN_Interrupt_Handler::interruptRoutine(){    
    if(!isMailBoxFull()){
        CANMessage newCanMsg;       // declare empty message
        canBus.read(newCanMsg);     // place newly received CAN message into newCanMsg
        messageMailBox[mailBoxPutIndex] = newCanMsg;
        
        // mailBoxPutIndex will return to 0 if index exceeds size of stack
        mailBoxPutIndex = (mailBoxPutIndex + 1) % CAN_STACK_SIZE;
        
        // DEBUG
        if(led1 == 1){
            led1 = 0;
        }else{
            led1 = 1;
        }
        //serial.printf("Yes");
        
    }else{
        // DEBUG
        if(led2 == 1){
            led2 = 0;
        }else{
            led2 = 1;
        }
        //serial.printf("No");
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
        canMsg = messageMailBox[mailBoxGetIndex];
        
        // mailBoxGetIndex will return to 0 if index exceeds size of stack
        mailBoxGetIndex = (mailBoxGetIndex + 1) % CAN_STACK_SIZE;
        
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
// isMailBoxFull
// The following function returns if true if the mailbox is full, false if not
// Input: None
// Output: true for full mailbox, false if mailbox still has room
bool CAN_Interrupt_Handler::isMailBoxFull(){
    if((((mailBoxPutIndex + 1) % CAN_STACK_SIZE)) == mailBoxGetIndex){
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
    this->priority = priority;
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

//******************************************************************************
// setMode
// Changes modes of CAN ports to the following:
//      0 : Ignore all messages
//      1 : Accept all messages
//      2 : Filter through messages
// The function wil attach the appropriate interrupt handler accodrding to the mode
void CAN_Interrupt_Handler::setMode(int mode, int filterId){
    //led2 = 1;
    this->mode = mode;
    id = filterId;
    switch(this->mode){       
        case 1:     // Change interrupt for whenever CAN receives message
            canBus.reset();
            canBus.attach(callback(this, &CAN_Interrupt_Handler::interruptRoutine), CAN::RxIrq);
            break;
        case 2:     // Change interrupt to filter CAN
            canBus.filter(id, 0xE, CANStandard);    // id to filter, 0xE is MASK, CANStandard for 11-bit
            canBus.attach(callback(this, &CAN_Interrupt_Handler::interruptRoutine), CAN::RxIrq);
            break;
        default:    // mode = 0 or invalid mode number
            canBus.reset();
            canBus.attach(0);
            break; 
    }
}

//******************************************************************************
// sendMessage
// sends a message through CAN
// Input: CANMessage msg
// Output: None
void CAN_Interrupt_Handler::sendMessage(CANMessage canMsg){
    canBus.write(canMsg);
}
