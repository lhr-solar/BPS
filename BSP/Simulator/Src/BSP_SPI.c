#include "BSP_SPI.h"
#include "config.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>

#define CSV_SPI_BUFFER_SIZE     64

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
	FILE* fp = fopen(write_file, "w+");
    if (!fp) {
        // File doesn't exit if true
        perror(SPI_W_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/ADC.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    for(uint32_t i = 0; i < txLen; i++) {
        fprintf(fp, "%d", txBuf[i]);

        // Do not add comma at the very end
        if(i < txLen-1) {   
            fprintf(fp, ",");
        }
    }

    // Unlock the lock so the simulator can write to ADC.csv again
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
    // Creates file if none exists
	FILE* fp = fopen(read_file, "r");

    // Lock the file so simulator.py/ADC.py can not write it during a read op.
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    // Check until SPIW.csv file is empty. The simulator indicates the SPIR.csv
    // file has been updated by deleting the command in SPIW.csv

    fgets(csvBuffer, CSV_SPI_BUFFER_SIZE, fp);

    // Unlock the lock so the simulator can write to ADC.csv again
    flock(fno, LOCK_UN);

    fclose(fp);
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

