#include "BSP_CAN.h"

/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   None
 * @return  None
 */
void BSP_CAN_Init(void) {

    // TODO: Initialize the CAN port that connects to the rest of the car.
}

/**
 * @brief   Transmits the data onto the CAN bus with the specified id
 * @param   id : Message of ID. Also indicates the priority of message. The lower the value, the higher the priority.
 * @param   data : data to be transmitted. The max is 8 bytes.
 * @param   length : num of bytes of data to be transmitted. This must be <= 8 bytes or else the rest of the message is dropped.
 * @return  0 if module was unable to transmit the data onto the CAN bus. Any other value indicates data was transmitted.
 */
uint8_t BSP_CAN_Write(uint32_t id, uint8_t data[8], uint8_t length) {
    
    // TODO: Transmit the data onto the CAN bus with the specified ID.

    return 0;
}

/**
 * @brief   Gets the data that was received from the CAN bus.
 * @note    Non-blocking statement
 * @pre     The data parameter must be at least 8 bytes or hardfault may occur.
 * @param   id : pointer to store id of the message that was received.
 * @param   data : pointer to store data that was received. Must be 8bytes or bigger.
 * @return  0 if nothing was received so ignore id and data that was received. Any other value indicates data was received and stored.
 */
uint8_t BSP_CAN_Read(uint32_t *id, uint8_t *data) {

    // TODO: Check if a message has been received.
    //      If message was received, then store the id and data of the message into the pointers and return a value other than 0.

    return 0;
}
