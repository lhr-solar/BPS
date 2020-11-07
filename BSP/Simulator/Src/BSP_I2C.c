#include "BSP_I2C.h"
#include "EEPROM.h"
#include "config.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>

#define CSV_ENTRY_LENGTH 5  //"0x??," is 5 characters
#define CSV_ENTRY_OFFSET 2  //"0x" is 2 characters

//only supports EEPROM peripheral (as of May 2020, the EEPROM is the only peripheral on the I2C bus)

//path for EEPROM file
static const char* file = GET_CSV_PATH(I2C_CSV_FILE);

/**
 * @brief   Initializes the I2C port that interfaces with the EEPROM. Creates EEPROM txt file if it does not already exist
 * @param   None
 * @return  None
 */
void BSP_I2C_Init(void) {
    // create and initialize csv file to simulate EEPROM if it does not exist already
    char hex[3];//to hold first 2 characters of file

    // open and close file (to create file if it does not exist)
    FILE *fp = fopen(file, "a");
    fclose(fp);

    // check if first 2 characters in file are equal to "0x", if not, reinitialize EEPROM
    fp = fopen(file, "r");

    // Lock the file so simulator.py/I2C.py can not write it during a read op
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    // File access
    fgets(hex, 3, fp);

    // Unlock the lock so the simulator can write to I2C.csv again
    flock(fno, LOCK_UN);
    fclose(fp);

    // if csv file does not exist (or has incorrect format of first element), initialize txt file EEPROM with all 0s
    if (strcmp(hex, "0x")){
        fp = fopen(file, "w");
        if (!fp) {
            // File doesn't exit if true
            perror(I2C_CSV_FILE);
            exit(EXIT_FAILURE);
        }

        fno = fileno(fp);
        flock(fno, LOCK_EX);
        
        for (uint32_t i = 0; i < (EEPROM_BYTES - 1); i++){
            fprintf(fp, "0x00\n");
        }

        fprintf(fp, "0x00");
        flock(fno, LOCK_UN);
        fclose(fp);
    }
}

/**
 * @brief   Transmits data onto the I2C bus.
 * @param   regAddr : the register address to write to in the IC's memory.
 * @param   txData : the data array to be sent onto the bus.
 * @param   txLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t  BSP_I2C_Write(uint8_t devAddr, uint16_t regAddr, uint8_t *txData, uint32_t txLen) {
    FILE *fp = fopen(file, "r+");
    if (!fp) {
        // File doesn't exit if true
        perror(I2C_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/I2C.py can not write it during a read op
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    // set file pointer to starting EEPROM address in txt file
    fseek(fp, regAddr * CSV_ENTRY_LENGTH + CSV_ENTRY_OFFSET, SEEK_SET);

    char data[3];
    for (uint32_t i = 0; i < txLen; i++){
        sprintf(data, "%x", txData[i]);//convert uint8_t to char[], so that it can be written to txt file
        if (txData[i] > 0x0f){
            fprintf(fp, "%c%c", data[0], data[1]);
        }else{//handle edge case where data is only one character
            fprintf(fp, "0%c", data[0]);
        }
        fseek(fp, CSV_ENTRY_LENGTH - 2 /*subtract 2, since 2 bytes were written*/, SEEK_CUR);//iterate to next "address" in txt file
    }

    // Unlock the lock so the simulator can write to I2C.csv again
    flock(fno, LOCK_UN);
    fclose(fp);

    return SUCCESS;
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param   regAddr : the register address to read from the IC's memory.
 * @param   rxData : the data array to store the data that is received.
 * @param   rxLen : the length of the data array.
 * @return  error status, 0 if fail, 1 if success
 */
uint8_t BSP_I2C_Read(uint8_t devAddr, uint16_t regAddr, uint8_t *rxData, uint32_t rxLen) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        // File doesn't exit if true
        perror(I2C_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // set file pointer to starting EEPROM address in txt file
    fseek(fp, regAddr * CSV_ENTRY_LENGTH, SEEK_SET);

    // Lock the file so simulator.py/I2C.py can not write it during a read op
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    char data[5];//want to include "0x" prefix
    data[4] = '\0';
    for (uint32_t i = 0; i < rxLen; i++){
        char *end;//needed for strtol
        fscanf(fp, "%c%c%c%c", &(data[0]), &(data[1]), &(data[2]), &(data[3]));
        rxData[i] = (uint8_t) strtol(data, &end, 16);
        fseek(fp, CSV_ENTRY_LENGTH - 4, SEEK_CUR);//iterate to next "address" in txt file
    }
    
    // Unlock the lock so the simulator can write to I2C.csv again
    flock(fno, LOCK_UN);
    fclose(fp);

    return SUCCESS;
}
