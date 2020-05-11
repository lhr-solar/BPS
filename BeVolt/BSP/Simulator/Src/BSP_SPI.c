#include "BSP_SPI.h"
#include "config.h"

#define CSV_SPI_BUFFER_SIZE     1024

/**
 * @brief   10-bit Command Codes for the LTC6811
 * @note    Some commands can have certain bits that can be either high or low. By default, the macro
 *          definitions set these bits to lo e.g. check ADCV. Look at the LTC6811 datasheet for more info.
 */
#define WRCFGA      0x001
#define WRCFGB      0x024
#define RDCFGA      0x002
#define RDCFGB      0x026
#define RDCVA       0x004
#define RDCVB       0x006
#define RDCVC       0x008
#define RDCVD       0x00A
#define RDCVE       0x009
#define RDCVF       0x00B
#define RDAUXA      0x00C
#define RDAUXB      0x00E
#define RDAUXC      0x00D
#define RDAUXD      0x00F
#define RDSTATA     0x010
#define RDSTATB     0x012
#define WRSCTRL     0x014
#define WRPWM       0x020
#define WRPSB       0x01C
#define RDSCTRL     0x016
#define RDPWM       0x022
#define RDPSB       0x01E
#define STSCTRL     0x019
#define CLRSCTRL    0x018
#define ADCV        0x260       // 0 1 MD[1] MD[0] 1 1 DCP 0 CH[2] CH[1] CH[0]
#define ADOW        0x268       // 0 1 MD[1] MD[0] 1 1 DCP 1 CH[2] CH[1] CH[0]
#define CVST        0x207       // 0 1 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
#define ADOL        0x201       // 0 1 MD[1] MD[0] 0 0 DCP 0 0 0 1
#define ADAX        0x460       // 1 0 MD[1] MD[0] 1 1 0 0 CHG[2] CHG[1] CHG[0]
#define ADAXD       0x400       // 1 0 MD[1] MD[0] 0 0 0 0 CHG[2] CHG[1] CHG[0]
#define AXST        0x407       // 1 0 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
#define ADSTAT      0x468       // 1 0 MD[1] MD[0] 1 1 0 1 CHST[2] CHST[1] CHST[0]
#define ADSTATD     0x468       // 1 0 MD[1] MD[0] 0 0 0 1 CHST[2] CHST[1] CHST[0]
#define STATST      0x40F       // 1 0 MD[1] MD[0] ST[1] ST[0] 0 1 1 1 1
#define ADCVAX      0x46F       // 1 0 MD[1] MD[0] 1 1 DCP 1 1 1 1
#define ADCVSC      0x467       // 1 0 MD[1] MD[0] 1 1 DCP 0 1 1 1
#define CLRCELL     0x711
#define CLRAUX      0x712
#define CLRSTAT     0x713
#define PLADC       0x714
#define DIAGN       0x715
#define WRCOMM      0x721
#define RDCOMM      0x722
#define STCOMM      0x723

#define MUX1        0x90
#define MUX2        0x92

typedef struct {
    uint8_t config[6];              // Configuration data of the LTC6811
    uint16_t voltage_data[12];      // Each board can support 12 battery modules
    uint8_t temperature_mux;        // Holds the MUX addr that corresponds to the temperature_sel
    uint16_t temperature_sel;       // Holds which temperature sensors the LTC6811 driver wants to read from.
                                    //      The process of getting temperature data requires knowing what the MUX is set to.
                                    //      Only one temperature sensor is sent from the LTC6811 at a time.
    uint16_t temperature_data[18];  // Each board can support 18 temperature sensors
    uint16_t open_wire;             // Each bit indicates a battery node wire
} ltc6811_sim_t;

typedef enum {
    A=0, B, C, D, E, F
} Group;

// Path relative to the executable
const char* file = "BSP/Simulator/DataGeneration/Data/SPI.csv";

static uint8_t chipSelectState = 1;     // During idle, the cs pin should be high.
                                        // Knowing the cs pin's state is not needed for the simulator,
                                        // but checking if the pin is low before any file read/writes
                                        // will make sure the developer follows the correct SPI protocol.

static uint16_t currCmd = 0;            // Before every BSP_SPI_Read, BSP_SPI_Write needs to be called to
                                        // specify the command to handle

static char csvBuffer[CSV_SPI_BUFFER_SIZE];
static ltc6811_sim_t simulationData[NUM_MINIONS];

/**
 * @brief   Data formating functions
 */
static void WRCommandHandler(uint8_t *buf, uint32_t len);
static void RDCommandHandler(uint8_t *buf, uint32_t len);
static void PEC15_Table_Init(void);
static uint16_t PEC15_Calc(char *data , int len);
static uint16_t ExtractCmdFromBuff(uint8_t *buf, uint32_t len);
static void ExtractDataFromBuff(uint8_t *data, uint8_t *buf, uint32_t len);
static void ExtractMUXAddrFromBuff(uint8_t *comm);
static void ExtractMUXSelFromBuff(uint8_t *comm);
static void CreateReadPacket(uint8_t *pkt, uint8_t *data, uint32_t dataSize);
static void CopyVoltageToByteArray(uint8_t *data, Group group);
static void CopyTemperatureToByteArray(uint8_t *data, Group group);

/**
 * @brief   File access functions
 */
static bool LoadCSV(void);
static bool UpdateSimulationData(void);

/**
 * @brief   Initializes the SPI port connected to the LTC6820.
 *          This port communicates with the LTC6811 voltage and temperature
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          Look at analog devices website and LTC6811's or LTC6820's datasheets.
 * @param   None
 * @return  None
 */
void BSP_SPI_Init(void) {

    for(int i = 0; i < NUM_MINIONS; i++) {
        for(int j = 0; j < 12; j++) {
            simulationData[i].voltage_data[j] = j+1;
            simulationData[i].temperature_data[j] = 12-j;
        }
    }

    PEC15_Table_Init();
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
    currCmd = ExtractCmdFromBuff(txBuf, txLen);
    currCmd &= ~0x180;  // Bit Mask to ignore any cmd configuration bits i.e. ignore the MD, DCP, etc. bits

    // Ignore PEC (bits 2 and 3), PEC is meant to be able to check if EMI/noise affected the data

    WRCommandHandler(txBuf, txLen);
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
    RDCommandHandler(rxBuf, rxLen);
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




/**
 * @brief   PRIVATE FUNCTIONS
 */

/**
 * @brief   Handles are WR command codes.
 *          Generic commands where commands with no following data is sent is also handled here.
 * @param   buf     raw data the LTC6811 drivers sent into BSP_SPI_Write.
 * @param   len 
 */
static void WRCommandHandler(uint8_t *buf, uint32_t len) {

    const uint8_t BYTES_PER_REG = 6;
    uint8_t data[NUM_MINIONS * BYTES_PER_REG];

    switch(currCmd) {
        // LTC6811 Configuration
        case WRCFGA: {
            ExtractDataFromBuff(data, buf, len);
            int dataIdx = 0;
            for(int i = NUM_MINIONS - 1; i >= 0; i--) {
                // Copy data to config register
                memcpy(simulationData[i].config, &data[dataIdx*BYTES_PER_REG], BYTES_PER_REG);
                dataIdx++;
            }
            break;
        }

        // Start ADC Conversion
        case ADCV:
            UpdateSimulationData();
            break;

        case ADAX:
            UpdateSimulationData();
            break;

        case WRCOMM:
            
            break;

        default:
            break;
    }
}

/**
 * @brief   Handles are RD command codes
 * @param   buf     used to store all the data that the LTC6811 driver functions will be expecting.
 *                  drivers sent into BSP_SPI_Write.
 * @param   len 
 */
static void RDCommandHandler(uint8_t *buf, uint32_t len) {

    const uint8_t BYTES_PER_REG = 6;
    uint8_t data[NUM_MINIONS * BYTES_PER_REG];

    switch(currCmd) {
        // LTC6811 Configuration
        case RDCFGA: {
            // store config registers of all LTC6811s into one continuous array
            int dataIdx = 0;
            for(int i = NUM_MINIONS - 1; i >= 0; i--) {
                memcpy(&data[dataIdx * BYTES_PER_REG], simulationData[i].config, BYTES_PER_REG);
                dataIdx++;
            }
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;
        }

        // Read Cell Voltages
        case RDCVA:
            CopyVoltageToByteArray(data, A);
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;

        case RDCVB:
            CopyVoltageToByteArray(data, B);
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;

        case RDCVC:
            CopyVoltageToByteArray(data, C);
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;

        case RDCVD:
            CopyVoltageToByteArray(data, D);
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;

        case RDAUXA:
            ExtractMUXAddrFromBuff(buf);
            ExtractMUXSelFromBuff(buf);
            CopyTemperatureToByteArray(data, A);
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;

        default:
            break;
    }
}


/**
 * @brief FILE ACCESSING FUNCTIONS
 */

/**
 * @brief   Reads the CSV file and loads the data into the buffer
 * @return  true if data was read successfully, false if failed
 */
static bool LoadCSV(void) {
    FILE* fp = fopen(file, "r");
    if (!fp) {
        printf("SPI not available\n\r");
        return false;
    }

    fgets(csvBuffer, 1024, fp);
    
    return true;
}

/**
 * @brief   Updates the simulationData struct from the CSV file
 * @return  true if data was read successfully, false if failed
 */
static bool UpdateSimulationData(void) {
    bool success = LoadCSV();
    if(!success) {
        return false;
    }



}




/**
 * @brief   DATA FORMATTING FUNCTIONS
 */

/**
 * @brief   Get the command code from the buffer
 * @param   buf 
 * @param   len
 */
static uint16_t ExtractCmdFromBuff(uint8_t *buf, uint32_t len) {
    return (uint16_t)((buf[0] << 8) | buf[1]);
}

/**
 * @brief   Deletes all the PEC codes of the buffer
 *          Depending on how many LTC6811 ICs are being used, the buf is formatted
 *          as [dataN-1:6B][pecN-1:2B][dataN-2:6B][pecN-2:2B]...[data0:6B][pec0:2B].
 *          data will hold [dataN-1:6B][dataN-2:6B]...[data0:6B].
 * @param   data
 * @param   buf 
 * @param   len
 */
static void ExtractDataFromBuff(uint8_t *data, uint8_t *buf, uint32_t len) {
    const uint8_t BYTES_PER_REG = 6;
    const uint8_t BYTES_PER_IC = 8;     // Register size (6B) + PEC (2B)

    // Extract only the data so ignore PEC and command code
    for(int i = 0; i < NUM_MINIONS; i++) {
        // The +4 is because bytes [0:1] holds the command code and [2:3] holds
        //  the PEC for the command code.
        memcpy(&data[i*BYTES_PER_REG], &buf[i*BYTES_PER_IC+4], BYTES_PER_REG);
    }

}

/**
 * @brief   Extracts the MUX address from the 6B COMM register.
 * @note    There are two 1:8 MUXs on each Minion Board. There are 16 temperature sensors that
 *          can read on each board. Each MUX has different addresses. MUX1 is connected to the
 *          first 8 temperature sensors and MUX2 is connected to the other 8 temperature sensors.
 * @note    MUX1 addr: 0x90
 *          MUX2 addr: 0x92
 * @param   buf     raw data the LTC6811 drivers sent into BSP_SPI_Write.
 */
static void ExtractMUXAddrFromBuff(uint8_t *buf) {
    const uint8_t BYTES_PER_REG = 6;

    buf = buf + 4;

    int minionIdx = 0;
    for(int i = NUM_MINIONS-1; i >= 0; i--) {
        simulationData[minionIdx].temperature_mux = ((buf[i*BYTES_PER_REG] << 4) & 0xFF00)
                                                    | ((buf[i*BYTES_PER_REG+1] >> 4) & 0x00FF);
        minionIdx++;
    }
}

/**
 * @brief   Extracts the select bits for the MUXs from the 6B COMM register.
 * @note    
 * @param   buf     raw data the LTC6811 drivers sent into BSP_SPI_Write.
 */
static void ExtractMUXSelFromBuff(uint8_t *buf) {
    const uint8_t BYTES_PER_REG = 6;

    buf = buf + 4;

    int minionIdx = 0;
    for(int i = NUM_MINIONS-1; i >= 0; i--) {
        uint8_t sel = 8 - ((buf[i*BYTES_PER_REG+3] >> 4) & 0x00FF);     // Check LTC1380 as to why there is an 8
        simulationData[minionIdx].temperature_sel = sel;
        minionIdx++;
    }
}

/**
 * @brief   Create a Packet that the LTC6811 will usually send back to uC
 * @param   pkt         array that will be filled with the formated cmd+data with respective PECs
 * @param   data        data array for all modules
 * @param   dataSize    size of data array
 */
static void CreateReadPacket(uint8_t *pkt, uint8_t *data, uint32_t dataSize) {
    const uint8_t BYTES_PER_REG = 6;

    uint32_t pktIdx = 0;
    for (uint8_t currIC = NUM_MINIONS; currIC > 0; currIC--) {
        // executes for each LTC681x in daisy chain, this loops starts with
        // the last IC on the stack. The first configuration written is
        // received by the last IC in the daisy chain

        for (uint8_t currByte = 0; currByte < BYTES_PER_REG; currByte++) {
            pkt[pktIdx] = data[((currIC-1)*6)+currByte];
            pktIdx = pktIdx + 1;
        }

        uint16_t dataPEC = PEC15_Calc((char *)&data[(currIC-1)*6], BYTES_PER_REG);    // calculating the PEC for each Iss configuration register data
        pkt[pktIdx] = (dataPEC >> 8) & 0x00FF;
        pkt[pktIdx + 1] = dataPEC & 0x00FF;
        pktIdx = pktIdx + 2;
    }
}

/**
 * @brief   Copies the voltage data in each array into one continuous array
 * @param   data      array that will be filled
 * @param   group     enum of [A,F]
 */
static void CopyVoltageToByteArray(uint8_t *data, Group group) {
    const uint8_t BYTES_PER_REG = 6;

    // The LTC6811 returns only 3 voltage values at a time depending on the group,
    // i.e. group A will only send the voltage values [0,2], group B will send [3,5], and so on
    int voltageStartIdx = group * 3;

    int dataIdx = 0;
    for(int i = NUM_MINIONS-1; i >= 0; i--) {
        memcpy(&data[dataIdx * BYTES_PER_REG], (uint8_t *)&(simulationData[i].voltage_data[voltageStartIdx]), BYTES_PER_REG);
        dataIdx++;
    }
}

/**
 * @brief   Copies the temperature data into one continuous array.
 * @note    Only 2 Bytes of the 6 Bytes are written to depending on the group.
 * @note    Only one temperature sensor is placed into the array, that's determined by
 *          the temperature_mux and temperature_sel pins of the simulationData.
 * @param   data      array that will be filled
 * @param   group     enum of [A,F]
 */
static void CopyTemperatureToByteArray(uint8_t *data, Group group) {
    const uint8_t BYTES_PER_REG = 6;

    // Only GPIO1 is connected to an analog voltage so group A Bytes[0:1] is the only
    // location that is updated.
    if(group == A) {
        int dataIdx = 0;
        for(int i = NUM_MINIONS-1; i >= 0; i--) {

            uint8_t temperatureIdx = simulationData[i].temperature_sel;

            if(simulationData[i].temperature_mux == MUX2) {
                temperatureIdx += 8;
            }

            memcpy(&data[dataIdx * BYTES_PER_REG], (uint8_t *)&(simulationData[i].temperature_data[temperatureIdx]), 2);
            dataIdx++;
        }
    }
}


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
                remainder = (remainder ^ CRC15_POLY);
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
