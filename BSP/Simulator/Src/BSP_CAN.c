/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_CAN.h"
#include "simulator_conf.h"
#include <stdint.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>

static callback_t rxFunc;
static callback_t txFunc;
static const char* file = GET_CSV_PATH(CAN_CSV_FILE);

/**
 * @brief   Initializes the CAN module that communicates with the rest of the electrical system.
 * @param   rxEvent     : the function to execute when recieving a message. NULL for no action.
 * @param   txEvent     : the function to execute after transmitting a message. NULL for no action.
 * @param   loopback    : if we should use loopback mode (for testing)
 * @return  None
 */
void BSP_CAN_Init(callback_t rxEvent, callback_t txEvent, bool loopback) {
    FILE* fp = fopen(file, "w");
    if(!fp) {
        perror(CAN_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    flock(fno, LOCK_UN);
    fclose(fp);
    rxFunc = rxEvent;
    txFunc = txEvent;
}



/**
 * @brief   Transmits the data onto the CAN bus with the specified id
 * @param   id : Message of ID. Also indicates the priority of message. The lower the value, the higher the priority.
 * @param   data : data to be transmitted. The max is 8 bytes.
 * @param   length : num of bytes of data to be transmitted. This must be <= 8 bytes or else the rest of the message is dropped.
 * @return  ERROR if module was unable to transmit the data onto the CAN bus. SUCCESS indicates data was transmitted.
 */
ErrorStatus BSP_CAN_Write(uint32_t id, uint8_t data[8], uint8_t length) {
    FILE* fp = fopen(file, "a");
    if(!fp) {
        perror(CAN_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    
    if(length > 8){
        flock(fno, LOCK_UN);
        fclose(fp);
        return 0;
    }

    fprintf(fp, "0x%.3x,", id);
    for(int i = 0; i < length; i++){
        if(i == (length-1)){
            fprintf(fp, "0x%.2x", data[i]);
        }else{
            fprintf(fp, "0x%.2x,", data[i]); 
        }
             
    }
    fprintf(fp, "\n");
    flock(fno, LOCK_UN);
    fclose(fp);

    // spoof an interrupt
    txFunc();
    
    return 1;
}

/**
 * @brief   Gets the data that was received from the CAN bus.
 * @note    Non-blocking statement
 * @pre     The data parameter must be at least 8 bytes or hardfault may occur.
 * @param   id : pointer to store id of the message that was received.
 * @param   data : pointer to store data that was received. Must be 8bytes or bigger.
 * @return  ERROR if nothing was received so ignore id and data that was received. SUCCESS indicates data was received and stored.
 */
ErrorStatus BSP_CAN_Read(uint32_t *id, uint8_t *data) {
#if 0   // TODO: replace with interrupt-capable code
    FILE* fp = fopen(file, "r+");
    if(!fp) {
        perror(CAN_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);
    fseek(fp, 1, SEEK_SET);
    
    if(fgetc(fp) == 'x'){                       //check if file is empty or not, the first item in the .csv is always "0x" + <CAN id>
        fseek(fp, 2, SEEK_SET);
        fscanf(fp, "%3x", id);

        fseek(fp, 6 ,SEEK_SET);
        for(int i = 0; i < 8; i++){
            fscanf(fp, "%*c%*c%2hhx%*c", data);
            data++;
        }
        flock(fno, LOCK_UN);
        fclose(fp);
    
        fp = fopen(file, "w");                  //message was read, so clear .csv file
        if(!fp) {
            perror(CAN_CSV_FILE);
            exit(EXIT_FAILURE);
        }
        int fno = fileno(fp);
        flock(fno, LOCK_EX);
        flock(fno, LOCK_UN);
        fclose(fp);
        return 1;
    }else{
        flock(fno, LOCK_UN);
        fclose(fp);
        return 0;
    }
#else
    return 0;
#endif
}
