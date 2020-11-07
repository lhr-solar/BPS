/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_SPI.h"
#include "config.h"
#include "common.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>

#define CSV_SPI_BUFFER_SIZE     256

// Path relative to the executable
static const char* read_file = GET_CSV_PATH(SPI_R_CSV_FILE);
static const char* write_file = GET_CSV_PATH(SPI_W_CSV_FILE);

static uint8_t chipSelectState = 1;     // During idle, the cs pin should be high.
                                        // Knowing the cs pin's state is not needed for the simulator,
                                        // but checking if the pin is low before any file read/writes
                                        // will make sure the developer follows the correct SPI protocol.

static char csvBuffer[CSV_SPI_BUFFER_SIZE];


/**
 * @brief   Initializes the SPI port connected to the LTC6820.
 *          This port communicates with the LTC6811 voltage and temperature
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          Look at analog devices website and LTC6811's or LTC6820's datasheets.
 * @param   None
 * @return  None
 */
void BSP_SPI_Init(void) {
    // Check if simulator is running i.e. were the csv files created?
    if(access(read_file, F_OK) != 0) {
        // File doesn't exit if true
        perror(SPI_R_CSV_FILE);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief   Transmits data to through SPI.
 *          With the way the LTC6811 communication works, the LTC6811 will not send
 *          anything during a transmit for uC to LTC6811. This is unlike what
 *          the SPI protocol expects where a transmit and receive happen
 *          simultaneously.
 * @note    Blocking statement
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI_Write(uint8_t *txBuf, uint32_t txLen) {
    // Creates file if none exists
	FILE* fp = fopen(write_file, "r+");
    if (!fp) {
        // File doesn't exit if true
        perror(SPI_W_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/SPI.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);

    // Check if the file is empty
    int size = 0;
    do {
        flock(fno, LOCK_EX);

        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        
        // Unlock the lock so the simulator can write to SPI.csv again
        flock(fno, LOCK_UN);
    } while (size > 0);     // Loop until file is empty

    flock(fno, LOCK_EX);

    fseek(fp, 0, SEEK_SET);

    for(uint32_t i = 0; i < txLen; i++) {
        fprintf(fp, "%d", txBuf[i]);

        // Do not add comma at the very end
        if(i < txLen-1) {   
            fprintf(fp, ",");
        }
    }

    // Unlock the lock so the simulator can write to SPI.csv again
    flock(fno, LOCK_UN);

    fclose(fp);
}

/**
 * @brief   Gets the data from SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not be expecting anything from the uC.
 *          The SPI protocol requires the uC to transmit data in order to receive
 *          anything so the uC will send junk data.
 * @note    Blocking statement
 * @param   rxBuf   data array to store the data that is received.
 * @param   rxLen   length of data array.
 * @return  None
 */
void BSP_SPI_Read(uint8_t *rxBuf, uint32_t rxLen) {

    if(rxLen == 1) {
        rxBuf[0] = 1;
        return;
    }


    //========= Check SPIR.csv file until it's empty =========
    // Check until SPIR.csv file has contents. 
    // The simulator will update SPIR.csv once it's ready.

	FILE* fp_r = fopen(read_file, "r+");
    if (!fp_r) {
        // File doesn't exit if true
        perror(SPI_R_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/SPI.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp_r);

    // Check if the file is empty
    int size = 0;
    do {
        flock(fno, LOCK_EX);

        fseek(fp_r, 0, SEEK_END);
        size = ftell(fp_r);
        
        // Unlock the lock so the simulator can write to SPI.csv again
        flock(fno, LOCK_UN);
    } while (size == 0);     // Loop until something is in the file.

    flock(fno, LOCK_EX);

    // Grab first line only. There shouldn't be any other lines to grab with
    // current scheme of interface.
    fseek(fp_r, 0, SEEK_SET);
    fgets(csvBuffer, CSV_SPI_BUFFER_SIZE, fp_r);

    // Copy each byte of data in SPIR.csv into rxBuf. The bytes of data in
    // SPIR.csv are delimited by a comma.
    int byte_idx = 0;
    char *token_save_byte = NULL;
    char *token_byte = __strtok_r(csvBuffer, ",", &token_save_byte);
    while((token_byte != NULL) && (byte_idx < rxLen)) {
        sscanf(token_byte, "%hu", (short unsigned int *)&rxBuf[byte_idx]);
        token_byte = __strtok_r(NULL, ",", &token_save_byte);
        byte_idx++;
    }

    // Pad the rest with 0s
    while(byte_idx < rxLen) {
        rxBuf[byte_idx++] = 0;
    }

    // Unlock the lock so the simulator can write to SPI.csv again
    flock(fno, LOCK_UN);

    fclose(fp_r);

    // Delete contents of file
    fp_r = fopen(read_file, "w+");
    fclose(fp_r);

}

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the LTC6811 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the LTC6811
 *          go to standby.
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI_SetStateCS(uint8_t state) {
    chipSelectState = state;
}

