// CANlib.h
/**
 * Driver for CAN bus. Interrupts whenever a new message has been received
 * @authors Sijin Woo, Corey Hulse
 * @lastRevised 9/4/2018
 */

#ifndef __CANLIB_H
#define __CANLIB_H

#include "mbed.h"

#define CAN_STACK_SIZE 5

/** CAN_Interrupt_Handler
 * CAN bus class that implements receiver interrupts.
 */
class CAN_Interrupt_Handler{
public:

    /** Constructor
     * Initializes CAN bus pins and receiver interrupt
     * @param rx pin
     * @param tx pin
     * @param priority of the interrupt
     * @param frequency of the CAN bus lines
     * @param mode for filtering (0 : ignore all msgs, 1 : accept all msgs, 2 : filter messages)
     * @param filterId when filter mode (2) is enabled. Only records messages that matches filterId
     */
    CAN_Interrupt_Handler(PinName rx, PinName tx, int priority, int frequency= 100000, int mode= 1, int filterId= 0x600);
    
    /** getNextMessage
     * Gets a message from the CAN FIFO
     * @param CANMessage object
     */
    bool getNextMessage(CANMessage &canMsg);

    /** isFIFOEmpty
     * Checks if FIFO is empty
     * @return True if empty, False if not-empty
     */
    bool isFIFOEmpty();

    /** isFIFOFull
     * Checks if FIFO is empty
     * @return True if full, False if still room left
     */
    bool isFIFOFull();

    /** setPriority
     * Sets the priority of the CAN receiver interrupt
     * @param priority number (0-255), 0 with highest priority
     */
    void setPriority(int priority);     // TODO: Test. Skeptical if this works

    /** setMode
     * Sets mode if receiver interrupt should ignore, accept, filter messages
     * @param mode for filtering (0 : ignore all msgs, 1 : accept all msgs, 2 : filter messages)
     */
    void setMode(int mode, int filterId= 0x6);

    /** sendMessage
     * Sends message through CAN bus
     * @param CANMessage object
     */
    void sendMessage(CANMessage canMsg);
    
private:
    
    /** interruptyRoutine
     * ISR that is called whenever a new message arrives on the CANB bus
     */
    void interruptRoutine();

    /** findCANnumber
     * Not necessary but it checks which CAN module the pins are using
     * @param rx pin
     * @param tx pin
     */
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
};


#endif
