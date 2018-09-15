// CANlib.h
/**
 * Driver for CAN bus. Interrupts whenever a new message has been received
 * @authors Sijin Woo, Corey Hulse
 * @lastRevised 9/4/2018
 */

#include "CANlib.h"

// For Debugging
DigitalOut led1(LED1);
DigitalOut led2(LED2);
//Serial serial(SERIAL_TX, SERIAL_RX);

/** Constructor
 * Initializes CAN bus pins and receiver interrupt
 * @param rx pin
 * @param tx pin
 * @param priority of the interrupt
 * @param frequency of the CAN bus lines
 * @param mode for filtering (0 : ignore all msgs, 1 : accept all msgs, 2 : filter messages)
 * @param filterId when filter mode (2) is enabled. Only records messages that matches filterId
 */
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

/** interruptyRoutine
 * ISR that is called whenever a new message arrives on the CANB bus
 */
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

/** getNextMessage
 * Gets a message from the CAN FIFO
 * @param CANMessage object
 */
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

/** isFIFOEmpty
 * Checks if FIFO is empty
 * @return True if empty, False if not-empty
 */
bool CAN_Interrupt_Handler::isFIFOEmpty(){
    if(mailBoxPutIndex == mailBoxGetIndex){
        return true;
    }else{
        return false;
    }
}

/** isFIFOFull
 * Checks if FIFO is empty
 * @return True if full, False if still room left
 */
bool CAN_Interrupt_Handler::isFIFOFull(){
    if((((mailBoxPutIndex + 1) % CAN_STACK_SIZE)) == mailBoxGetIndex){
        return true;
    }else{
        return false;
    }
}

/** findCANnumber
 * Not necessary but it checks which CAN module the pins are using
 * @param rx pin
 * @param tx pin
 */
void CAN_Interrupt_Handler::findCANnumber(PinName rx, PinName tx){
    if((rx == PD_0 && tx == PD_1) || (rx == PB_8 && tx == PB_9)){
        canNumber = 1;
    }else if((rx == PB_12 && tx == PB_13) || (rx == PB_5 && tx == PB_6)){
        canNumber = 2;
    }else{
        canNumber = 0;  // pins are not right
    }
}

/** setPriority
 * Sets the priority of the CAN receiver interrupt
 * @param priority number (0-255), 0 with highest priority
 */
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

/** setMode
 * Sets mode if receiver interrupt should ignore, accept, filter messages
 * @param mode for filtering (0 : ignore all msgs, 1 : accept all msgs, 2 : filter messages)
 */
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

/** sendMessage
 * Sends message through CAN bus
 * @param CANMessage object
 */
void CAN_Interrupt_Handler::sendMessage(CANMessage canMsg){
    canBus.write(canMsg);
}
