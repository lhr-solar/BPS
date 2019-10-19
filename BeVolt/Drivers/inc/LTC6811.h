/*!
LTC6811-1: Multicell Battery Monitor

http://www.linear.com/product/LTC6811-1

http://www.linear.com/product/LTC6811-1#demoboards


Copyright 2018(c) Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.
 - Neither the name of Analog Devices, Inc. nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.
 - The use of this software may or may not infringe the patent rights
   of one or more patent holders.  This license does not release you
   from the requirement that you obtain separate licenses from these
   patent holders to use this software.
 - Use of the software either in source or binary form, must be run
   on or directly connected to an Analog Devices Inc. component.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Copyright 2017 Linear Technology Corp. (LTC)
***********************************************************/

/*! @file
    @ingroup LTC6811-1
    Header for LTC6811-1 Multicell Battery Monitor
*/

#ifndef LTC6811_H
#define LTC6811_H

#include <stdint.h>
#include "LTC681x.h"

/*********************************************************/
/*** Code that was added by UTSVT. ***/
/*********************************************************/
// ADC Command Configurations (binary)
// Conversion Mode: MD[1:0] bits
//	00 : 422Hz
//	01 : 27kHz (Fast)
//	10 : 7kHz (Normal)
//	11 : 26Hz (Filtered)
#define ADC_CONVERSION_MODE			0b10

// Discharge Permitted?
//	0 : Disabled
//	1 : Enabled
#define ADC_DCP									0

// Cell channels to measure (binary)
// 	000 : All cells
//	001 : Cell 1 and 7
//	010 : Cell 2 and 8
//	011 : Cell 3 and 9
//	100 : Cell 4 and 10
//	101 : Cell 5 and 11
//	110 : Cell 6 and 12
#define CELL_CH_TO_CONVERT				0b000

#define MD_422HZ_1KHZ 0
#define MD_27KHZ_14KHZ 1
#define MD_7KHZ_3KHZ 2
#define MD_26HZ_2KHZ 3

#define ADC_OPT_ENABLED 1
#define ADC_OPT_DISABLED 0

#define CELL_CH_ALL 0
#define CELL_CH_1and7 1
#define CELL_CH_2and8 2
#define CELL_CH_3and9 3
#define CELL_CH_4and10 4
#define CELL_CH_5and11 5
#define CELL_CH_6and12 6

#define SELFTEST_1 1
#define SELFTEST_2 2

#define AUX_CH_ALL 0
#define AUX_CH_GPIO1 1
#define AUX_CH_GPIO2 2
#define AUX_CH_GPIO3 3
#define AUX_CH_GPIO4 4
#define AUX_CH_GPIO5 5
#define AUX_CH_VREF2 6

#define STAT_CH_ALL 0
#define STAT_CH_SOC 1
#define STAT_CH_ITEMP 2
#define STAT_CH_VREGA 3
#define STAT_CH_VREGD 4

#define DCP_DISABLED 0
#define DCP_ENABLED 1

#define PULL_UP_CURRENT 1
#define PULL_DOWN_CURRENT 0

#define LTC6811_NUM_RX_BYT 8
#define LTC6811_CELL 1
#define LTC6811_AUX 2
#define LTC6811_STAT 3
#define LTC6811_CFGR 0
#define LTC6811_CFGRB 4

/* pec15_calc
 * Calculates  and returns the CRC15
 * @param int 32-bit number of bytes that will be used to calculate a PEC
 * @param array of data that will be used to calculate a PEC
 * @returns the calculated pec15 as an unsigned int
 */
uint16_t pec15_calc(int32_t len, uint8_t *data);

/* wakeup_idle
 * Wake isoSPI up from idle state
 * @param number of ICs in the daisy chain
 */
void wakeup_idle(uint8_t total_ic);

/* wakeup_sleep
 * Generic wakeup commannd to wake the LTC6811 from sleep
 * @param number of ICs in the daisy chain
 */
void wakeup_sleep(uint8_t total_ic);

/* cmd_68
 * Sense a command to the bms IC. This code will calculate the PEC code for the transmitted command
 * @param 2 Byte array containing the BMS command to be sent
 */ 
void cmd_68(uint8_t tx_cmd[2]);

/* write_68
 * Writes an array of data to the daisy chain. Function calculated PEC for tx_cmd data
 * @param unsigned int 8-bit number of ICs in the daisy chain
 * @param 2 Byte array containing the BMS command to be sent
 * @param Array containing the data to be written to the BMS ICs
 */ 
void write_68(uint8_t total_ic, uint8_t tx_cmd[2], uint8_t data[]);

/* read_68
 * Generic function to write 6811 commands and read data. Function calculated PEC for tx_cmd data
 * @param unsigned int 8-bit number of ICs in the daisy chain
 * @param 2 Byte array containing the BMS command to be sent
 * @param Array that the read back data will be stored
 * @return 8-bit int PEC Status
 */
int8_t read_68(uint8_t total_ic, uint8_t tx_cmd[2], uint8_t *rx_data);

/** LTC6811_Init
 * Initializes the LTC6811 and the battery Module (cell_asic) struct.
 * This also initializes SPI1 and the chip select pin for the specific IC.
 * @param cell_asic struct
 */
void LTC6811_Init(cell_asic *battMod);

/* LTC6811_init_reg_limits
 * Initializes variables for each voltage board
 * @param unsigned int 8-bit Total number of IC
 * @param cell_asic struct
 */
void LTC6811_init_reg_limits(uint8_t total_ic, cell_asic ic[]);


/* LTC6811_diagn
 * Starts the Mux Decoder diagnostic self test
 * MUXFAIL bit will be set to 1 for a failure and 0 if the test has been passed
 */
void LTC6811_diagn(void);


/* LTC6811_pladc
 * Sends the poll adc command
 * @returns 1 byte read back after a pladc command. If the byte is not 0xFF ADC conversion has completed
 */
uint8_t LTC6811_pladc(void);


/* LTC6811_pollAdc !
 * Blocks operation until the ADC has finished its conversion
 * @returns the approximate time it took for the ADC function to complete
 */
uint32_t LTC6811_pollAdc(void);

/* LTC6811_adcv !
 * Starts cell voltage conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permit
 * @param unsigned int 8-bit Cell Channels to be measured
 */
void LTC6811_adcv(uint8_t MD, uint8_t DCP, uint8_t CH);

/* LTC6811_adcvax
 * Starts cell voltage and GPIO 1&2 conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permit
 */
void LTC6811_adcvax(uint8_t MD, uint8_t DCP);

/* LTC6811_cvst
 * Starts cell voltage self test conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 */
void LTC6811_cvst(uint8_t MD, uint8_t ST);

/* LTC6811_adcvsc
 * Starts cell voltage and SOC conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permit
 */
void LTC6811_adcvsc(uint8_t MD, uint8_t DCP);

/* LTC6811_adol
 * Starts cell voltage overlap conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permitted
 */
void LTC6811_adol(uint8_t MD, uint8_t DCP);

/* LTC6811_adow
 * Start an open wire Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permitted
 */
void LTC6811_adow(uint8_t MD, uint8_t PUP);

/* LTC6811_adax
 * Start a GPIO and Vref2 Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit GPIO Channels to be converted
 */
void LTC6811_adax(uint8_t MD, uint8_t CHG);

/* LTC6811_adaxd
 * Start an GPIO Redundancy test
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit GPIO Channels to be converted
 */
void LTC6811_adaxd(uint8_t MD, uint8_t CHG);

/* LTC6811_axst
 * Start an Auxiliary Register Self Test Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 */
void LTC6811_axst(uint8_t MD, uint8_t ST);

/* LTC6811_adstat
 * Start a Status ADC Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Status Channels to be converted
 */
void LTC6811_adstat(uint8_t MD, uint8_t CHST);

/* LTC6811_adstatd
 * Start a Status register redundancy test Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Status Channels to be converted
 */
void LTC6811_adstatd(uint8_t MD, uint8_t CHST);

/* LTC6811_statst
 * Start a Status Register Self Test Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 */
void LTC6811_statst(uint8_t MD, uint8_t ST);


/* LTC6811_rdcv
 * Reads and parses the LTC6811 cell voltage registers
 * @param unsigned int 8-bit cell voltage register
 * @param unsigned int 8-bit number of ICs in the daisy chain (-1 only)
 * @param array of the parsed cell codes from lowest to highest
 * @return unsigned 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read ? 
 */
uint8_t LTC6811_rdcv(uint8_t reg, uint8_t total_ic, cell_asic ic[]);

/* LTC6811_rdaux
 * Reads and parses the LTC6811 auxiliary registers
 * @param unsigned int 8-bit GPIO voltage register
 * @param unsigned int 8-bit number of ICs in the daisy chain
 * @param two dimensional array of the parsed gpio voltage codes
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdaux(uint8_t reg, uint8_t total_ic, cell_asic ic[]);

/* LTC6811_rdcv_reg
 * Reads the raw cell voltage register data
 * @param unsigned int 8-bit cell voltage register that is read back
 * @param unsigned int 8-bit number of ICs in the system
 * @param array of the unparsed cell codes
 */
void LTC6811_rdcv_reg(uint8_t reg, uint8_t total_ic, uint8_t *data);

/* parse_cells 
 * helper function that parses voltage measurement registers
 * @param unsigned int 8-bit current 
 * @param unsigned int 8-bit current IC
 * @param array of the cell data 
 * @param array of the cell codes 
 * @param array of the IC PEC
 * @return signed int 8-bit PEC status
 */
int8_t parse_cells(uint8_t current_ic, uint8_t cell_reg, uint8_t cell_data[], uint16_t *cell_codes, uint8_t *ic_pec);

/* LTC6811_rdstat
 * Reads and parses the LTC6811 stat registers.
 * @param unsigned int 8-bit Stat register
 * @param unsigned int 8-bit number of ICs in the system
 * @param two dimensional array of the stat codes
 * @return signed 8-bit Packet Error Code
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdstat(uint8_t reg, uint8_t total_ic, cell_asic ic[]);

/* LTC6811_clrcell
 * Clears the cell voltage registers and intiallizes all values to 1
 * Reads back hexadecimal 0xFF after the command is sent
 */
void LTC6811_clrcell(void);

/* LTC6811_clraux
 * Clears the Auxiliary registers and initializes all values to 1
 * Reads back hexadecimal 0xFF after the command is sent
 */
void LTC6811_clraux(void);

/* LTC6811_clrstat
 * Clears the Stat registers and intiallizes all values to 1
 * Reads back hexadecimal 0xFF after the command is sent
 */
void LTC6811_clrstat(void);

/* LTC6811_clrsctrl
 * Clears the Sctrl registers and initializes all values to 0
 * Reads back hexadecimal 0x00 after the command is sent
 */
void LTC6811_clrsctrl(void);

/* LTC6811_wrcfg 
 * Writes the configuration registers of the LTC6811-1s connected in a daisy chain stack
 * Written in descending order so the last device's configuration is written first
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_wrcfg(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_wrcfgb ?
 * 
 * @param unsigned int 8-bit number of ICs
 * @param two dimensional array of the configuration data
 */
void LTC6811_wrcfgb(uint8_t nIC, cell_asic ic[]);

/* LTC6811_rdcfg 
 * Reads configuration registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 8-bit Packet Error Code
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdcfg(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_rdcfgb ?
 * 
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdcfgb(uint8_t nIC, cell_asic ic[]);

/* LTC6811_wrpwm
 * Writes the pwm registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width modulation register
 * @param two dimensional array of the configuration data
 */
void LTC6811_wrpwm(uint8_t total_ic, uint8_t pwmReg, cell_asic ic[]);

/* LTC6811_rdpwm
 * Reads pwm registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width modulation register
 * @param two dimensional array of the configuration data
 * @return signed 8-bit Packet Error Code
 */
int8_t LTC6811_rdpwm(uint8_t total_ic, uint8_t pwmReg, cell_asic ic[]);


/* LTC6811_wrsctrl
 * Write the LTC6811 Sctrl register
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width sctrl register
 * @param two dimensional array of the configuration data
 */
void LTC6811_wrsctrl(uint8_t nIC, uint8_t sctrl_reg, cell_asic ic[]);

/* LTC6811_wrsctrl
 * Reads sctrl registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width sctrl register
 * @param two dimensional array of the configuration data
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */ 
int8_t LTC6811_rdsctrl(uint8_t nIC, uint8_t sctrl_reg, cell_asic ic[]);

/* LTC6811_stsctrl
 * Starts Sctrl data communication over the spins
 */
void LTC6811_stsctrl(void);

/* LTC6811_wrcomm
 * Writes the COMM registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width modulation register
 * @param two dimensional array of the comm data
 */
void LTC6811_wrcomm(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_rdcomm
 * Reads COMM registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdcomm(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_stcomm
 * Shifts data in COMM register out over LTC6811 SPI/I2C port
 * Issues a stcomm command and clocks data out of the COMM register ?
 */
void LTC6811_stcomm(void);

/*LTC6811_st_lookup
 * @param ADC Conversion Mode
 * @param Self test number
 * @returns returns the register data pattern for a given ADC MD and Self test 
 */
uint16_t LTC6811_st_lookup(uint8_t MD, uint8_t ST);

/* LTC6811_set_discharge
 * Helper function to set discharge bit in CFG register
 * @param int Cell 
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_set_discharge(int Cell, uint8_t total_ic, cell_asic ic[]);

/* LTC6811_st_lookup
 * Looks up the result pattern for digital filter self test
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 * @return unsigned 16-bit int test pattern
 */
uint16_t LTC6811_st_lookup(uint8_t MD, uint8_t ST);

/* LTC6811_run_cell_adc_st
 * Runs the Digital Filter/ADC Self Test
 * @param unsigned int 8-bit ADC register
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 16-bit error
 */
 
int16_t LTC6811_run_cell_adc_st(uint8_t adc_reg,uint8_t total_ic, cell_asic ic[]);

/* LTC6811_run_adc_redundancy_st
 * Runs the ADC Digital Redudancy commands and checks output for errors
 * @param unsigned int 8-bit ADC mode
 * @param unsigned int 8-bit ADC register
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 16-bit error
 */
int16_t LTC6811_run_adc_redundancy_st(uint8_t adc_mode, uint8_t adc_reg, uint8_t total_ic, cell_asic ic[]);

/* LTC6811_run_openwire
 * Runs the datasheet algorithm for open wire
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_run_openwire(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_set_discharge
 * Helper function to set discharge bit in CFG register
 * @param int Cell 
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_set_discharge(int Cell, uint8_t total_ic, cell_asic ic[]);

/* LTC6811_run_adc_overlap
 * Runs the ADC overlap test for the IC
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return unsigned 16-bit error
 */
uint16_t LTC6811_run_adc_overlap(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_check_pec
 * Helper function that counts overall PEC errors and register/IC PEC errors
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit register
 * @param two dimensional array of the IC cells
 */
void LTC6811_check_pec(uint8_t total_ic, uint8_t reg, cell_asic ic[]);

/* LTC6811_reset_crc_count
 * Helper Function to reset PEC counters
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 */
void LTC6811_reset_crc_count(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_init_cfg
 * Helper function to initialize the CFGR data structures
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 */
void LTC6811_init_cfg(uint8_t total_ic, cell_asic ic[]);

/* LTC6811_set_cfgr
 * Helper function to set appropriate bits in CFGR register based on bit function
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param boolean reference powered up
 * @param boolean ADC mode option bit
 * @param GPIO Pin 5
 * @param Discharge Cell 12
 */
void LTC6811_set_cfgr(uint8_t nIC, cell_asic ic[], bool refon, bool adcopt, bool gpio[5],bool dcc[12]);

/* LTC6811_set_cfgr_refon
 * Helper function to set the REFON bit
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param boolean reference powered up
 */
void LTC6811_set_cfgr_refon(uint8_t nIC, cell_asic ic[], bool refon);

/* LTC6811_set_cfgr
 * Helper function to set the adcopt bit
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param boolean ADC mode option bit
 */
void LTC6811_set_cfgr_adcopt(uint8_t nIC, cell_asic ic[], bool adcopt);

/* LTC6811_set_cfgr
 * Helper function to set GPIO bits
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param GPIO Pin ?
 */
void LTC6811_set_cfgr_gpio(uint8_t nIC, cell_asic ic[],bool gpio[]);

/* LTC6811_set_cfgr_dis
 * Helper function to turn the DCC bits HIGH or LOW
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param Discharge Cell ?
 */
void LTC6811_set_cfgr_dis(uint8_t nIC, cell_asic ic[],bool dcc[]);

/* LTC6811_set_cfgr_uv
 * Helper Function to set uv value in CFGRA register
 * @param unsigned int 8-bit number of ICs ?
 * @param two dimensional array of the IC cells
 * @param unsigned int 16-bit uv value
 */
void LTC6811_set_cfgr_uv(uint8_t nIC, cell_asic ic[],uint16_t uv);

/* LTC6811_set_cfgr_ov
 * Helper function to set OV value in CFGRA register
 * @param unsigned int 8-bit number of ICs ?
 * @param two dimensional array of the IC cells
 * @param unsigned int 16-bit ov value
 */
void LTC6811_set_cfgr_ov(uint8_t nIC, cell_asic ic[],uint16_t ov);

/* LTC6811_init_max_min
 * Initializes the max and min of the IC cells
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param two dimensional array of the min of the IC cells
 * @param two dimensional array of the max of the IC cells
 */
void LTC6811_init_max_min(uint8_t total_ic, cell_asic ic[],cell_asic ic_max[],cell_asic ic_min[]);

/* LTC6811_max_min
 * Checks the difference in max and min and redefines max and min??
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param two dimensional array of the min of the IC cells
 * @param two dimensional array of the max of the IC cells
 * @param two dimensional array of difference in max and min
 */
void LTC6811_max_min(uint8_t total_ic, cell_asic ic_cells[], cell_asic ic_min[], cell_asic ic_max[],cell_asic ic_delta[]);
#endif
