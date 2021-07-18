#include "AS8510.h"
#include "BSP_SPI.h"
#include "BSP_Lights.h"
#include "os.h"
#include "Tasks.h"

// Addresses of the registers in the AS8510
#define AS8510_DREG_I1       0x00   // Current data registers
#define AS8510_DREG_I2       0x01
#define AS8510_DREG_V1       0x02   // Voltage data registers
#define AS8510_DREG_V2       0x03
#define AS8510_STATUS_REG    0x04   // Read-only status register
#define AS8510_DEC_REG_R1_I  0x05   // Control register 1
#define AS8510_DEC_REG_R2_I  0x06   // Control register 2
#define AS8510_FIR_CTL_REG_I 0x07
#define AS8510_CLK_REG       0x08   // Clock control register
#define AS8510_RESET_REG     0x09   // Soft-reset control
#define AS8510_MOD_CTL_REG   0x0A   // Mode control register
#define AS8510_MOD_TA_REG1   0x0B   // Timing control for SBM1/SBM2
#define AS8510_MOD_TA_REG2   0x0C
#define AS8510_MOD_ITH_REG1  0x0D   // Threshold control for SBM1/SBM2
#define AS8510_MOD_ITH_REG2  0x0E
#define AS8510_MOD_TMC_REG1  0x0F   // Configures number of ADC samples to drop
#define AS8510_MOD_TMC_REG2  0x10
#define AS8510_NOM_ITH_REG1  0x11   // Threshold control for NOM2
#define AS8510_NOM_ITH_REG2  0x12
#define AS8510_PGA_CTL_REG   0x13   // Gain control
#define AS8510_PD_CTL_REG_1  0x14   // Power control registers
#define AS8510_PD_CTL_REG_2  0x15
#define AS8510_PD_CTL_REG_3  0x16
#define AS8510_ACH_CTL_REG   0x17   // Analog channel selection
#define AS8510_ISC_CTL_REG   0x18   // Current source setting register (internal source)
#define AS8510_OTP_EN_REG    0x19   // Reserved
#define AS8510_STATUS_REG_2  0x44   // Data saturation flags
#define AS8510_DEC_R1_V      0x45   // Voltage control registers
#define AS8510_DEC_R2_V      0x46
#define AS8510_FIR_CTL_REG_V 0x47

/* Initialize communication with the AS8510
 * to begin measurement of electrical current.
 */
void AS8510_Init(bsp_os_t spi_os) 
{
    BSP_SPI_Init(spi_as8510, &spi_os);

    
    

    // TODO: verify that this initialization is correct once
    //       we have the hardware that we need

    // Configure the registers on the AS8510.
    // Even the registers that are not changed from their default
    // values are shown here, just for good measure.
    
    uint8_t cfg;

    /**** Signal path control registers ****/

    cfg = 0x45;
    AS8510_WriteToAddr(AS8510_DEC_REG_R1_I, &cfg, 1);

    

    // Enable I-channel, disable V-channel, mask interrupt
    cfg = 0x95;
    AS8510_WriteToAddr(AS8510_DEC_REG_R2_I, &cfg, 1);

    cfg = 0x04;
    AS8510_WriteToAddr(AS8510_FIR_CTL_REG_I, &cfg, 1);

    /**** Digital control registers ****/

    cfg = 0x20;
    AS8510_WriteToAddr(AS8510_CLK_REG, &cfg, 1);

    // We are not writing to the reset register

    // Skipping AS8510_MOD_CTL_REG for now... see the end

    cfg = 0x80;
    AS8510_WriteToAddr(AS8510_MOD_TA_REG1, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_MOD_TA_REG2, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_MOD_ITH_REG1, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_MOD_ITH_REG2, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_MOD_TMC_REG1, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_MOD_TMC_REG2, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_NOM_ITH_REG1, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_NOM_ITH_REG2, &cfg, 1);

    /**** Analog control registers ****/

    // Configure gain of 40 (recommended for 200A max)
    cfg = 0x90;
    AS8510_WriteToAddr(AS8510_PGA_CTL_REG, &cfg, 1);

    cfg = 0x6F;
    AS8510_WriteToAddr(AS8510_PD_CTL_REG_1, &cfg, 1);

    cfg = 0xF3;
    AS8510_WriteToAddr(AS8510_PD_CTL_REG_2, &cfg, 1);

    // This should be configured to supply data in 2's complement
    cfg = 0xFC;
    AS8510_WriteToAddr(AS8510_PD_CTL_REG_3, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_ACH_CTL_REG, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_ISC_CTL_REG, &cfg, 1);

    cfg = 0x00;
    AS8510_WriteToAddr(AS8510_OTP_EN_REG, &cfg, 1);

    /**** Voltage control registers ****/

    // Not being used

    /**** Start ****/
    // Start the measurement in Normal mode
    cfg = 0x01;
    AS8510_WriteToAddr(AS8510_MOD_CTL_REG, &cfg, 1);
}

/* Writes to a register on the AS8510
 * @param addr the address to write to
 * @param data the data to write
 * @param len  the length of the data
 */
ErrorStatus AS8510_WriteToAddr(uint8_t addr, uint8_t *data, uint8_t len) 
{
    uint8_t txdata[256];
    
    if(addr + len > 0x47) {
        return ERROR;
    }

    // Copy the data into the transmit buffer
    txdata[0] = addr;
    memcpy(txdata + 1, data, len);

    BSP_SPI_SetStateCS(spi_as8510, 0);

    // Write the data to the registers
    BSP_SPI_Write(spi_as8510, txdata, len + 1);

    BSP_SPI_SetStateCS(spi_as8510, 1);

    return SUCCESS;
}

/* Reads from a register on the AS8510
 * @param addr the address to write to
 * @param data the data to write
 * @param len  the length of the data
 */
ErrorStatus AS8510_ReadFromAddr(uint8_t addr, uint8_t *data, uint8_t len) 
{
    if(addr + len > 0x47) {
        return ERROR;
    }

    uint8_t txaddr = addr | 0x80;

    BSP_SPI_SetStateCS(spi_as8510, 0);

    BSP_SPI_Write(spi_as8510, &txaddr, 1);
    BSP_SPI_Read(spi_as8510, data, len);

    BSP_SPI_SetStateCS(spi_as8510, 1);

    return SUCCESS;
}

/* Reads from the specified register, but only 1 byte.
 * @param addr the address of the register
 * @return the byte that was read
 */
uint8_t AS8510_ReadRegister(uint8_t addr)
{
    uint8_t data;

    AS8510_ReadFromAddr(addr, &data, 1);

    return data;
}

/* Gets the current from the AS8510
 * TODO: is this conversion correct?
 */
int32_t AS8510_GetCurrent() {
    uint8_t dreg[2];

    AS8510_ReadFromAddr(0, dreg, 2);

    uint16_t reading = ((uint16_t)(dreg[0]) << 8) | (uint16_t)(dreg[1]);

    // We are measuring in I200 mode (Gain = 40). The datasheet suggests the ADC reference voltage will typically be around 1.225 V, 
    // although it will be affected by temperature. For now, I will ignore drift and see how well the BPS can measure temperature.

    // 1.225 V divided by 16 bits => 0.0000186920166015625 V of precision
    // Gain of 40 => 0.0000004673004150390625 V drop across shunt changes the ADC by one bit
    // 100 uOhm shunt => 0.004673004150390625 A of precision in our measurement (4.673004150390625 mA per step)
    // The datasheet states that a chopper gets rid of the offset in the input voltage, so I will assume
    // that the ADC is centered on 0 Amps (a reading of 0x8000 will be 0 Amps)

    // Used for converting ADC reading into milliamps
    const int32_t OFFSET = 0x8000;
    const int32_t NUMERATOR = 4673;
    const int32_t DENOMINATOR = 1000;
    
    int32_t milliamps = (((int32_t) reading) - OFFSET) * NUMERATOR / DENOMINATOR;

    return milliamps; // TODO: how does this need to be modified to be useable?
}

