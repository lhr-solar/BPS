#include "BSP_SPI.h"
#include "config.h"
#include "simulator_conf.h"
#include <unistd.h>
#include <sys/file.h>

#define CSV_SPI_BUFFER_SIZE     1024

static const char* file_w = GET_CSV_PATH(SPIW_CSV_FILE);
static const char* file_r = GET_CSV_PATH(SPIR_CSV_FILE);
/**
 * @brief   10-bit Command Codes for the LTC6811
 * @note    Some commands can have certain bits that can be either high or low. By default, the macro
 *          definitions set these bits to lo e.g. check ADCV. Look at the LTC6811 datasheet for more info.
 */
#define SIM_LTC6811_WRCFGA      0x001
#define SIM_LTC6811_WRCFGB      0x024
#define SIM_LTC6811_RDCFGA      0x002
#define SIM_LTC6811_RDCFGB      0x026
#define SIM_LTC6811_RDCVA       0x004
#define SIM_LTC6811_RDCVB       0x006
#define SIM_LTC6811_RDCVC       0x008
#define SIM_LTC6811_RDCVD       0x00A
#define SIM_LTC6811_RDCVE       0x009
#define SIM_LTC6811_RDCVF       0x00B
#define SIM_LTC6811_RDAUXA      0x00C
#define SIM_LTC6811_RDAUXB      0x00E
#define SIM_LTC6811_RDAUXC      0x00D
#define SIM_LTC6811_RDAUXD      0x00F
#define SIM_LTC6811_RDSTATA     0x010
#define SIM_LTC6811_RDSTATB     0x012
#define SIM_LTC6811_WRSCTRL     0x014
#define SIM_LTC6811_WRPWM       0x020
#define SIM_LTC6811_WRPSB       0x01C
#define SIM_LTC6811_RDSCTRL     0x016
#define SIM_LTC6811_RDPWM       0x022
#define SIM_LTC6811_RDPSB       0x01E
#define SIM_LTC6811_STSCTRL     0x019
#define SIM_LTC6811_CLRSCTRL    0x018
#define SIM_LTC6811_ADCV        0x260       // 0 1 MD[1] MD[0] 1 1 DCP 0 CH[2] CH[1] CH[0]
#define SIM_LTC6811_ADOWPU      0x268       // 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
#define SIM_LTC6811_ADOWPD      0x228       // 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
#define SIM_LTC6811_CVST        0x207       // 0 1 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
#define SIM_LTC6811_ADOL        0x201       // 0 1 MD[1] MD[0] 0 0 DCP 0 0 0 1
#define SIM_LTC6811_ADAX        0x460       // 1 0 MD[1] MD[0] 1 1 0 0 CHG[2] CHG[1] CHG[0]
#define SIM_LTC6811_ADAXD       0x400       // 1 0 MD[1] MD[0] 0 0 0 0 CHG[2] CHG[1] CHG[0]
#define SIM_LTC6811_AXST        0x407       // 1 0 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
#define SIM_LTC6811_ADSTAT      0x468       // 1 0 MD[1] MD[0] 1 1 0 1 CHST[2] CHST[1] CHST[0]
#define SIM_LTC6811_ADSTATD     0x468       // 1 0 MD[1] MD[0] 0 0 0 1 CHST[2] CHST[1] CHST[0]
#define SIM_LTC6811_STATST      0x40F       // 1 0 MD[1] MD[0] ST[1] ST[0] 0 1 1 1 1
#define SIM_LTC6811_ADCVAX      0x46F       // 1 0 MD[1] MD[0] 1 1 DCP 1 1 1 1
#define SIM_LTC6811_ADCVSC      0x467       // 1 0 MD[1] MD[0] 1 1 DCP 0 1 1 1
#define SIM_LTC6811_CLRCELL     0x711
#define SIM_LTC6811_CLRAUX      0x712
#define SIM_LTC6811_CLRSTAT     0x713
#define SIM_LTC6811_PLADC       0x714
#define SIM_LTC6811_DIAGN       0x715
#define SIM_LTC6811_WRCOMM      0x721
#define SIM_LTC6811_RDCOMM      0x722
#define SIM_LTC6811_STCOMM      0x723

#define SIM_LTC1380_MUX1        0x90        // MUX addresses on Minion boards
#define SIM_LTC1380_MUX2        0x92

typedef struct {
    uint8_t config[6];              // Configuration data of the LTC6811
    uint16_t voltage_data[12];      // Each board can support 12 battery modules
    uint8_t temperature_mux;        // Holds the MUX addr that corresponds to the temperature_sel
    uint16_t temperature_sel;       // Holds which temperature sensors the LTC6811 driver wants to read from.
                                    //      The process of getting temperature data requires knowing what the MUX is set to.
                                    //      Only one temperature sensor is sent from the LTC6811 at a time.
    int32_t temperature_data[16];   // Each board can support 16 temperature sensors
    uint16_t open_wire;             // Each bit indicates a battery node wire
} ltc6811_sim_t;

typedef enum {
    GroupA=0, GroupB, GroupC, GroupD, GroupE, GroupF
} Group;

static const char* file_w = GET_CSV_PATH(SPIW_CSV_FILE);
static const char* file_r = GET_CSV_PATH(SPIR_CSV_FILE);

// Path relative to the executable
static const char* file = GET_CSV_PATH(SPI_CSV_FILE);

static uint8_t chipSelectState = 1;     // During idle, the cs pin should be high.
                                        // Knowing the cs pin's state is not needed for the simulator,
                                        // but checking if the pin is low before any file read/writes
                                        // will make sure the developer follows the correct SPI protocol.

static uint16_t currCmd = 0;            // Before every BSP_SPI_Read, BSP_SPI_Write needs to be called to
                                        // specify the command to handle.

static bool openWireOpFlag = false;     // The reading of voltages for both regular and openwire modes are
                                        // the same. The commands are different though. To measure the
                                        // regular voltage values, the ADCV command code is used. To measure
                                        // the open wire voltage values, the ADOW command code is used.
                                        // Look in the LTC6811 datasheet for more information on the difference.
                                        // The command to read the voltage values for both commands is RDCV.
                                        // When the ADOW command is called in BSP_SPI_Write, this flag should
                                        // turn true so the next time the BSP_SPI_Read function is called with
                                        // the RDCV command set, the RDCommandHandler will know to return the
                                        // appropratie open wire voltages. Any other command that's not ADOW but
                                        // still triggers and ADC conversion (e.g. ADCV), then this flag should
                                        // be set to false.

static bool openWirePUFlag = false;     // Indicates if the pull-up voltage values should be copied or not.
                                        // This flag is set if the ADOWPU command was issued in
                                        // BSP_SPI_Write. The CopyOpenWireVoltageToByteArray function uses
                                        // this flag to determine which voltage values should be written.

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
static void CopyOpenWireVoltageToByteArray(uint8_t *data, Group group, bool pullup);
static void CopyTemperatureToByteArray(uint8_t *data, Group group);
static uint16_t ConvertTemperatureToMilliVolts(int32_t celcius);
static Group DetermineGroupLetter(uint16_t cmd);

/**
 * @brief   File access functions
 */
static bool UpdateSimulationData(void);
static void dischargeModules(void);

/**
 * @brief   Initializes the SPI port connected to the LTC6820.
 *          This port communicates with the LTC6811 voltage and temperature
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          Look at analog devices website and LTC6811's or LTC6820's datasheets.
 * @param   None
 * @return  None
 */
void BSP_SPI_Init(void) {

    // Reset values
    memset(simulationData, 0, sizeof(simulationData));

    PEC15_Table_Init();
    FILE *fp = fopen(file_r, "w+"); //create SPI_read file
    fclose(fp); //close file
    // Check if simulator is running i.e. were the csv files created?
    if(access(file, F_OK) != 0) {
        // File doesn't exit if true
        perror(SPI_CSV_FILE);
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
 * @param   data memory address to data array that contains the data to be sent.
 * @return  None
 */
void BSP_SPI_Write(uint8_t *txBuf, uint32_t txLen) {
    FILE *fp = fopen(file_w, "a"); //open to append
    int fno = fileno(fp); //lock
    flock(fno, LOCK_EX);
    //write all data to file
    for (int i = 0; i < txLen; i++) fprintf(fp, "%o", *(txBuf + i));
    fprintf(fp, "%c", '\n'); //write newline at end of data
    flock(fno, LOCK_UN); //unlock
    fclose(fp); //close file
}

/**
 * @brief   Gets the data from SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not be expecting anything from the uC.
 *          The SPI protocol requires the uC to transmit data in order to receive
 *          anything so the uC will send junk data.
 * @note    Blocking statement
 * @param   data address to store data in
 * @return  None
 */
void BSP_SPI_Read(uint8_t *rxBuf, uint32_t rxLen) {
    //Data will be read from the top. Then it will be deleted
    int counter, i;
    uint8_t tempArr[100];
    FILE *fp = fopen(file_w, "r"); //open to read
    int fno = fileno(fp); //lock
    flock(fno, LOCK_EX);
    //read counter value
    fscanf(fp, "%d", &counter);
    counter++; //increment counter to read from that line
    //read row into rxbuffer
    fgets(rxBuf, rxLen + 1, fp);
    flock(fno, LOCK_UN); //unlock
    fclose(fp); //close file
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
        case SIM_LTC6811_WRCFGA: {
            ExtractDataFromBuff(data, buf, len);
            int dataIdx = 0;
            for(int i = 0; i < NUM_MINIONS; i++) {
                // Copy data to config register
                memcpy(simulationData[i].config, &data[dataIdx*BYTES_PER_REG], BYTES_PER_REG);
                dataIdx++;
            }
            //if any DCC bits are set, reflect that in SPI.csv
            dischargeModules();
            break;
        }

        // Start ADC Conversion
        case SIM_LTC6811_ADCV:
        case SIM_LTC6811_ADAX:
            UpdateSimulationData();
            openWireOpFlag = false;
            break;

        case SIM_LTC6811_ADOWPU:
        case SIM_LTC6811_ADOWPD:
            openWirePUFlag = false;
            if(currCmd == SIM_LTC6811_ADOWPU) {
                openWirePUFlag = true;
            }
            UpdateSimulationData();
            openWireOpFlag = true;
            break;

        case SIM_LTC6811_WRCOMM:
            ExtractMUXAddrFromBuff(buf);
            ExtractMUXSelFromBuff(buf);
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
        case SIM_LTC6811_RDCFGA: {
            // store config registers of all LTC6811s into one continuous array
            int dataIdx = 0;
            for(int i = 0; i < NUM_MINIONS; i++) {
                memcpy(&data[dataIdx * BYTES_PER_REG], simulationData[i].config, BYTES_PER_REG);
                dataIdx++;
            }
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;
        }

        // Read Cell Voltages
        case SIM_LTC6811_RDCVA:
        case SIM_LTC6811_RDCVB:
        case SIM_LTC6811_RDCVC:
        case SIM_LTC6811_RDCVD:
        case SIM_LTC6811_RDCVE:
        case SIM_LTC6811_RDCVF: {
            Group grp = DetermineGroupLetter(currCmd);
            if(openWireOpFlag) {
                CopyOpenWireVoltageToByteArray(data, grp, openWirePUFlag);
            } else {
                CopyVoltageToByteArray(data, grp);
            }
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;
        }

        case SIM_LTC6811_RDAUXA: {
            Group grp = DetermineGroupLetter(currCmd);
            CopyTemperatureToByteArray(data, grp);
            CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG);
            break;
        }

        default:
            break;
    }
}

/**
 * @brief   Returns the group letter depenging on the cmd and if grouping is required.
 * @param   cmd     specifies which group must be selected
 * @return  Group   letter of the cmd group
 */
static Group DetermineGroupLetter(uint16_t cmd) {
    Group grp = GroupA;

    switch(cmd) {
        case SIM_LTC6811_RDCVA:
        case SIM_LTC6811_RDAUXA:
            grp = GroupA;
            break;

        case SIM_LTC6811_RDCVB:
        case SIM_LTC6811_RDAUXB:
            grp = GroupB;
            break;

        case SIM_LTC6811_RDCVC:
        case SIM_LTC6811_RDAUXC:
            grp = GroupC;
            break;

        case SIM_LTC6811_RDCVD:
        case SIM_LTC6811_RDAUXD:
            grp = GroupD;
            break;

        case SIM_LTC6811_RDCVE:
            grp = GroupE;

        case SIM_LTC6811_RDCVF:
            grp = GroupF;
            break;

        default:
            grp = GroupA;
            break;
    }

    return grp;
}

/**
 * @brief FILE ACCESSING FUNCTIONS
 */

/**
 * @brief   Updates the simulationData struct from the CSV file
 * @return  true if data was read successfully, false if failed
 */
static bool UpdateSimulationData(void) {
    int lineIdx = 0;
    int temperatureIdx = 0;
    int voltageIdx = 0;
    int openWireIdx = 0;

    // Open File and get file descriptor
    FILE* fp = fopen(file, "r");
    if(!fp) {
        // Exit, error!
        perror(SPI_CSV_FILE);
        exit(EXIT_FAILURE);
    }

    // Lock the file so simulator.py/SPI.py can not write it during a read op
    // This is a blocking statement
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    // Read data
    while(fgets(csvBuffer, CSV_SPI_BUFFER_SIZE, fp) != 0) {
        char *saveDataPtr = NULL;

        // Make sure the csv buffer has the following format:
        //  [wire status, voltage*10000, temperature sensor 1 * 100, temperature sensor 2 * 100]
        // If you wish to add more SPI data, then add the data extraction here
        char *openWire = __strtok_r(csvBuffer, ",", &saveDataPtr);
        char *voltage = __strtok_r(NULL, ",", &saveDataPtr);
        char *temperature1 = __strtok_r(NULL, ",", &saveDataPtr);
        char *temperature2 = __strtok_r(NULL, ",", &saveDataPtr);

        // Place into ltc6811_sim_t data struct
        // Voltage Data
        simulationData[lineIdx / MAX_VOLT_SENSORS_PER_MINION_BOARD].voltage_data[voltageIdx] = atoi(voltage);
        
        voltageIdx = (voltageIdx + 1) % MAX_VOLT_SENSORS_PER_MINION_BOARD;

        // Temperature Data
        simulationData[lineIdx / MAX_VOLT_SENSORS_PER_MINION_BOARD].temperature_data[temperatureIdx] = atoi(temperature1);

        simulationData[lineIdx / MAX_VOLT_SENSORS_PER_MINION_BOARD].temperature_data[temperatureIdx + MAX_TEMP_SENSORS_PER_MINION_BOARD / NUM_TEMP_SENSORS_PER_MOD] = atoi(temperature2);
        
        temperatureIdx = (temperatureIdx + 1) % (MAX_TEMP_SENSORS_PER_MINION_BOARD / NUM_TEMP_SENSORS_PER_MOD);

        // Open Wire Data
        // Open wires are indicated as a bitmap instead of an array
        // 1 is open wire, 0 means closed wired
        simulationData[lineIdx / MAX_VOLT_SENSORS_PER_MINION_BOARD].open_wire |= (~atoi(openWire) & 0x01) << openWireIdx;
        
        openWireIdx = (openWireIdx + 1) % MAX_VOLT_SENSORS_PER_MINION_BOARD;

        lineIdx++;
    }

    // Unlock the lock so the simulator can write to SPI.csv again
    flock(fno, LOCK_UN);

    // Close file.
    fclose(fp);

    return true;
}

static void writeDCC(int moduleNum, int newStatus){
    FILE* fp = fopen(file, "r+");
    if(!fp) {
        perror(SPI_CSV_FILE);
        exit(EXIT_FAILURE);
    }
    int fno = fileno(fp);
    flock(fno, LOCK_EX);

    fseek(fp, moduleNum * 20, SEEK_SET);
    if(newStatus == 1){
        fprintf(fp, "%d", 1);   //on
    }
    else if(newStatus == 0){
        fprintf(fp, "%d", 0);   //off
    }

    flock(fno, LOCK_UN);
    fclose(fp);
}

static void dischargeModules(void){
    int moduleCount = 1;
    int bound = 8;
    //loop through each IC for all modules
    for(int i = 0; i < NUM_MINIONS; i++) {
        //check if any DCC bits in the 4th config register are set
        if(i == 3){
            bound = 7;
        }
        for(int j = 0; j < bound; j++){
            if(simulationData[i].config[4] & (1<<(j))){
                writeDCC(moduleCount, 1);
            }else{
                writeDCC(moduleCount, 0);
            }
            moduleCount++;
        }
                
                
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
    const uint8_t BYTES_PER_REG = 8;

    buf = buf + 4;

    int minionIdx = 0;
    for(int i = 0; i < NUM_MINIONS; i++) {
        simulationData[minionIdx].temperature_mux = ((buf[i*BYTES_PER_REG] << 4) & 0xF0)
                                                    | ((buf[i*BYTES_PER_REG+1] >> 4) & 0x0F);
        minionIdx++;
    }
}

/**
 * @brief   Extracts the select bits for the MUXs from the 6B COMM register.
 * @note    
 * @param   buf     raw data the LTC6811 drivers sent into BSP_SPI_Write.
 */
static void ExtractMUXSelFromBuff(uint8_t *buf) {
    const uint8_t BYTES_PER_REG = 8;

    buf = buf + 4;

    int minionIdx = 0;
    for(int i = 0; i < NUM_MINIONS; i++) {
        uint8_t sel = ((buf[i*BYTES_PER_REG+3] >> 4) & 0x000F) - 8;     // Check LTC1380 as to why there is an 8
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
 * @brief   Copies the Pull-up voltage data in each separate array into one continuous array
 * @param   data        array that will be filled
 * @param   group       enum of [A,F]
 * @param   pullup      true if pullup voltage data, false if pulldown voltage data to be copied
 */
static void CopyOpenWireVoltageToByteArray(uint8_t *data, Group group, bool pullup) {
    const uint8_t BYTES_PER_REG = 6;
    const int MAX_PINS_PER_LTC6811 = 12;         // LTC6811 can only support 12 voltage modules
    uint16_t pullupVoltages[NUM_MINIONS][MAX_PINS_PER_LTC6811];
    uint16_t pulldownVoltages[NUM_MINIONS][MAX_PINS_PER_LTC6811];

    // The LTC6811 returns only 3 voltage values at a time depending on the group,
    // i.e. group A will only send the voltage values [0,2], group B will send [3,5], and so on
    int voltageStartIdx = group * 3;

    // LTC6811 datasheet may be wrong***
    // For pins 1-11, the pullupVolt-pulldownVolt > -400mV must be true in order for the LTC6811
    // driver to recognize that the pin is an open wire.
    // If pullupVolt[1] = 0, then pin C0 is open.
    // If pulldownVolt[12] = 0, then pin C12 is open.
    // TODO: Simulator currently does not support open wire indication of pin C0 (ground pin)
    for(int i = 0; i < NUM_MINIONS; i++) {
        for(int j = 0; j < MAX_PINS_PER_LTC6811; j++) {
            if(simulationData[i].open_wire & (1 << j)) {
                pullupVoltages[i][j] = 40000;       // These are just dummy values. As long as
                                                    // pullup-pulldown > 4000 (-400mV)
                pulldownVoltages[i][j] = 30000;
                
            } else {
                pullupVoltages[i][j] = 30000;
                pulldownVoltages[i][j] = 40000;
            }
        }
    }

    int dataIdx = 0;
    for(int i = NUM_MINIONS-1; i >= 0; i--) {

        if(pullup) {
            memcpy(&data[dataIdx * BYTES_PER_REG], (uint8_t *)&(pullupVoltages[i][voltageStartIdx]), BYTES_PER_REG);
        } else {
            memcpy(&data[dataIdx * BYTES_PER_REG], (uint8_t *)&(pulldownVoltages[i][voltageStartIdx]), BYTES_PER_REG);
        }

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
    if(group == GroupA) {
        int dataIdx = 0;
        for(int i = NUM_MINIONS-1; i >= 0; i--) {

            uint8_t temperatureIdx = simulationData[i].temperature_sel;

            if(simulationData[i].temperature_mux == SIM_LTC1380_MUX2) {
                temperatureIdx += 8;
            }

            uint16_t mVData = ConvertTemperatureToMilliVolts(simulationData[i].temperature_data[temperatureIdx]) * 10;   // multiply by 10 because the Temperature library is expecting 0.0001 resolution

            memcpy(&data[dataIdx * BYTES_PER_REG], (uint8_t *)&(mVData), 2);
            dataIdx++;
        }
    }
}

/**
 * @brief   Converts the temperature data in celcius to millivolts that the
 *          Temperature.c library will be expecting.
 * @note    The simulator writes the actualy temperature value in Celcius into the
 *          csv file but the LTC6811 really returns the analog mV reading of the sensor.
 * @note    EQUATION: mV = 2230.8 - [13.582 (T - 30)] - [0.00433 (T - 30)^2]
 * @param   data    fixed point value of temperature data in celcius. resolution=0.001
 * @return  mV value in 0.001 resolution
 */
static uint16_t ConvertTemperatureToMilliVolts(int32_t celcius) {

    float celcius_float = (float)celcius / 1000;
    uint16_t mV = 2230.8 - (13.582 * (celcius_float - 30)) - (0.00433 * powf(celcius_float - 30, 2));

    return mV;
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



/**
 * @brief   PRIVATE FUNCTIONS
 */
