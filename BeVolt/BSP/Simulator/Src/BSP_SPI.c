#include "BSP_SPI.h"

#define CSV_SPI_BUFFER_SIZE     1024

/**
 * @brief   Command Codes for the LTC6811
 */
#define WRCFGA
#define WRCFGB
#define RDCFGA
#define RDCFGB
#define RDCVA
#define RDCVB
#define RDCVC
#define RDCVD
#define RDCVE
#define RDCVF
#define RDAUXA
#define RDAUXB
#define RDAUXC
#define RDAUXD
#define RDSTATA
#define RDSTATB
#define WRSCTRL
#define WRPWM
#define WRPSB
#define RDSCTRL
#define RDPWM

// Path relative to the executable
const char* file = "BSP/Simulator/DataGeneration/Data/SPI.csv";

static uint8_t chipSelectState = 1;     // During idle, the cs pin should be high.
                                        // Knowing the cs pin's state is not needed for the simulator,
                                        // but checking if the pin is low before any file read/writes
                                        // will make sure the developer follows the correct SPI protocol.

static char csvBuffer[CSV_SPI_BUFFER_SIZE];

static void PEC15_Table_Init(void);
static uint16_t PEC15_Calc(char *data , int len);
static void GetLTC6811Command(void);

/**
 * @brief   Initializes the SPI port connected to the LTC6820. This port communicates with the LTC6811
 *          voltage and temperature monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          Look at analog devices website and LTC6811's or LTC6820's datasheets.
 * @param   None
 * @return  None
 */
void BSP_SPI_Init(void) {
    // No intialization required.
}

/**
 * @brief   Transmits data to through SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not send anything during a transmit for uC to LTC6811. This is unlike what
 *          the SPI protocol expects where a transmit and receive happen simultaneously.
 * @note    Blocking statement
 * @param   txBuf : data array that contains the data to be sent.
 * @param   txLen : length of data array.
 * @return  None
 */
void BSP_SPI_Write(uint8_t *txBuf, uint32_t txLen) {
    FILE* fp = fopen(file, "r");
    if (!fp) {
        printf("SPI not available\n\r");
        return;
    }

    fgets(csvBuffer, 1024, fp);

}

/**
 * @brief   Gets the data from SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not be expecting anything from the uC. The SPI protocol requires the uC to
 *          transmit data in order to receive anything so the uC will send junk data.
 * @note    Blocking statement
 * @param   rxBuf : data array to store the data that is received.
 * @param   rxLen : length of data array.
 * @return  None
 */
void BSP_SPI_Read(uint8_t *rxBuf, uint32_t rxLen) {
    FILE* fp = fopen(file, "r");
    if (!fp) {
        printf("SPI not available\n\r");
        return;
    }

    fgets(csvBuffer, 1024, fp);

}

/**
 * @brief   Sets the state of the chip select output pin. Set the state to low/0 to notify the LTC6811 that
 *          the data sent on the SPI lines are for it. Set the state to high/1 to make the LTC6811 go to standby.
 * @param   state : 0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI_SetStateCS(uint8_t state) {
    chipSelectState = state;
}



/**
 * @brief   PRIVATE FUNCTIONS
 */




/************************************
Copyright 2012 Linear Technology Corp. (LTC)
Permission to freely use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies:
THIS SOFTWARE IS PROVIDED “AS IS” AND LTC DISCLAIMS ALL WARRANTIES
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
EVENT SHALL LTC BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM ANY USE OF SAME, INCLUDING
ANY LOSS OF USE OR DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
***********************************************************/
static int16_t pec15Table[256];
static int16_t CRC15_POLY = 0x4599;
static void PEC15_Table_Init(void) {
    for(int i = 0; i < 256; i++) {
        int16_t remainder = i << 7;
        for(int bit = 8; bit > 0; --bit) {
            if(remainder & 0x4000) {
                remainder = ((remainder << 1));
                remainder = (remainder ^ CRC15_POLY)
            } else {
                remainder = ((remainder << 1));
            }
        }
        pec15Table[i] = remainder&0xFFFF;
    }
}

static uint16_t PEC15_Calc(char *data , int len) {
    uint16_t remainder,address;
    remainder = 16;//PEC seed
    for(int i = 0; i < len; i++) {
        address = ((remainder >> 7) ^ data[i]) & 0xff;//calculate PEC table address
        remainder = (remainder << 8 ) ^ pec15Table[address];
    }
    return (remainder*2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}
