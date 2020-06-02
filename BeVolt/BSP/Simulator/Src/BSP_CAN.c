#include "BSP_CAN.h"
#include <stdint.h>
#include <stdlib.h>

static const char* file = "BSP/Simulator/DataGeneration/Data/CAN.csv";



/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   None
 * @return  None
 */



void BSP_CAN_Init(void) {

    FILE* fp = fopen(file, "w"); 
    fclose(fp);
}



/**
 * @brief   Transmits the data onto the CAN bus with the specified id
 * @param   id : Message of ID. Also indicates the priority of message. The lower the value, the higher the priority.
 * @param   data : data to be transmitted. The max is 8 bytes.
 * @param   length : num of bytes of data to be transmitted. This must be <= 8 bytes or else the rest of the message is dropped.
 * @return  0 if module was unable to transmit the data onto the CAN bus. Any other value indicates data was transmitted.
 */

uint8_t BSP_CAN_Write(uint32_t id, uint8_t data[8], uint8_t length) {
    if(length > 8){
        return 0;
    }

    FILE* fp = fopen(file, "w");
    fprintf(fp, "0x%.3x,", id);
    for(int i = 0; i < 8; i++){
        if(i == (7)){
            fprintf(fp, "0x%.2x", data[i]);
        }else{
            fprintf(fp, "0x%.2x,", data[i]); 
        }
             
    }
    fclose(fp);

    return 1;
}

/**
 * @brief   Gets the data that was received from the CAN bus.
 * @note    Non-blocking statement
 * @pre     The data parameter must be at least 8 bytes or hardfault may occur.
 * @param   id : pointer to store id of the message that was received.
 * @param   data : pointer to store data that was received. Must be 8bytes or bigger.
 * @return  0 if nothing was received so ignore id and data that was received. Any other value indicates data was received and stored.
 * clear the message that was read from the file
 */
uint8_t BSP_CAN_Read(uint32_t *id, uint8_t *data) {
    char check;
    int meh;
    FILE* fp = fopen(file, "r+");
    fseek(fp, 2, SEEK_SET);
    fscanf(fp, "%3x", id);

    fseek(fp, 6 ,SEEK_SET);
    for(int i = 0; i < 8; i++){
        fscanf(fp, "%*c%*c%2x%*c", data);
        data++;
    }
    fclose(fp);
    
    fopen(file, "w");
    fclose(fp);
    return 1;
}
