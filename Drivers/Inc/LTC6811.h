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

#define CELL 1
#define AUX 2
#define STAT 3

/** LTC6811_Init
 * Initializes the LTC6811 and the battery Module (cell_asic) struct.
 * This also initializes SPI and the chip select pin for the specific IC.
 * @param cell_asic struct
 */
void LTC6811_Init(cell_asic *battMod);

//wrappers for LTC6811 commands that return a PEC error value. These functions will trip the BPS if packets fail too many times in a row.

void LTC6811_rdcv_wrap(uint8_t reg, uint8_t total_ic, cell_asic ic[]);

void LTC6811_rdaux_wrap(uint8_t reg, uint8_t total_ic, cell_asic ic[]);

void LTC6811_rdstat_wrap(uint8_t reg, uint8_t total_ic, cell_asic ic[]);

void LTC6811_rdcfg_wrap(uint8_t total_ic, cell_asic ic[]);

void LTC6811_rdpwm_wrap(uint8_t total_ic, uint8_t pwmReg, cell_asic ic[]);

void LTC6811_rdcomm_wrap(uint8_t total_ic, cell_asic ic[]);


/********************************************************
*********************************************************/


void LTC6811_init_reg_limits(uint8_t total_ic, cell_asic ic[]);


/*! Starts the Mux Decoder diagnostic self test

 Running this command will start the Mux Decoder Diagnostic Self Test
 This test takes roughly 1mS to complete. The MUXFAIL bit will be updated,
 the bit will be set to 1 for a failure and 0 if the test has been passed.

*/
void LTC6811_diagn(void);


//! Sends the poll adc command
//! @returns 1 byte read back after a pladc command. If the byte is not 0xFF ADC conversion has completed
uint8_t LTC6811_pladc(void);


//! This function will block operation until the ADC has finished it's conversion
//! @returns the approximate time it took for the ADC function to complete.
uint32_t LTC6811_pollAdc(void);

/*! Starts cell voltage conversion
*/
void LTC6811_adcv(uint8_t MD, //!< ADC Conversion Mode
                  uint8_t DCP, //!< Controls if Discharge is permitted during conversion
                  uint8_t CH //!< Sets which Cell channels are converted
                 );

/*!  Starts cell voltage  and GPIO 1&2 conversion
*/
void LTC6811_adcvax(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Controls if Discharge is permitted during conversion
);


/*!  Starts cell voltage self test conversion
*/
void LTC6811_cvst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Self Test Mode
);

/*!  Starts cell voltage and SOC conversion
*/
void LTC6811_adcvsc(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Controls if Discharge is permitted during conversion
);
/*!  Starts cell voltage overlap conversion
*/
void LTC6811_adol(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t DCP //!< Discharge permitted during conversion
);

/*!  Start an open wire Conversion
*/
void LTC6811_adow(
	uint8_t MD, //ADC Mode
  uint8_t PUP, // Pull-up/pull-down current for open wire conversions
	uint8_t CH, // Channels
	uint8_t DCP // Controls if Discharge is permitted during conversion
);


/*!  Start a GPIO and Vref2 Conversion
*/
void LTC6811_adax(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHG //!< Sets which GPIO channels are converted
);

/*!  Start an GPIO Redundancy test
*/
void LTC6811_adaxd(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHG //!< Sets which GPIO channels are converted
);

/*!  Start an Auxiliary Register Self Test Conversion
*/
void LTC6811_axst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Sets if self test 1 or 2 is run
);

/*!  Start a Status ADC Conversion
*/
void LTC6811_adstat(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t CHST //!< Sets which Stat channels are converted
);

/*!   Start a Status register redundancy test Conversion
*/
void LTC6811_adstatd(
  uint8_t MD, //!< ADC Mode
  uint8_t CHST //!< Sets which Status channels are converted
);


/*!  Start a Status Register Self Test Conversion
*/
void LTC6811_statst(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Sets if self test 1 or 2 is run
);

/*!  Reads and parses the LTC6811 cell voltage registers.

  @return int8_t, PEC Status.
    0: No PEC error detected
    -1: PEC error detected, retry read
*/
uint8_t LTC6811_rdcv(uint8_t reg, //!< controls which cell voltage register is read back.
                     uint8_t total_ic, //!< the number of ICs in the daisy chain(-1 only)
                     cell_asic ic[] //!< array of the parsed cell codes from lowest to highest.
                    );



/*!  Reads and parses the LTC6811 auxiliary registers.
@return  int8_t, PEC Status
  0: No PEC error detected
 -1: PEC error detected, retry read
*/
int8_t LTC6811_rdaux(uint8_t reg,        //!< controls which GPIO voltage register is read back
                     uint8_t nIC,        //!< the number of ICs in the daisy chain
                     cell_asic ic[] //!< A two dimensional array of the parsed gpio voltage codes
                    );

/*!  Reads and parses the LTC6811 stat registers.

@return  int8_t, PEC Status
  0: No PEC error detected
 -1: PEC error detected, retry read
*/
int8_t LTC6811_rdstat(uint8_t reg, //Determines which Stat  register is read back.
                      uint8_t total_ic,//the number of ICs in the system
                      cell_asic ic[]
                     );

/*!  Clears the LTC6811 cell voltage registers
*/
void LTC6811_clrcell(void);

/*! Clears the LTC6811 Auxiliary registers
*/
void LTC6811_clraux(void);

/*!  Clears the LTC6811 Stat registers
*/
void LTC6811_clrstat(void);

/*!  Clears the LTC6811 Sctrl registers
*/
void LTC6811_clrsctrl(void);

/*!  Write the LTC6811 configuration register
*/
void LTC6811_wrcfg(uint8_t nIC, //!< The number of ICs being written
                   cell_asic ic[] //!< a two dimensional array of the configuration data that will be written
                  );

void LTC6811_wrcfgb(uint8_t nIC, //!< The number of ICs being written
                    cell_asic ic[] //!< a two dimensional array of the configuration data that will be written
                   );
/*!  Reads configuration registers of a LTC6811 daisy chain
@return int8_t, PEC Status.
  0: Data read back has matching PEC
   -1: Data read back has incorrect PEC
*/
int8_t LTC6811_rdcfg(uint8_t nIC, //!< number of ICs in the daisy chain
                     cell_asic ic[] //!< a two dimensional array that the function stores the read configuration data
                    );

int8_t LTC6811_rdcfgb(uint8_t nIC, //!< number of ICs in the daisy chain
                      cell_asic ic[] //!< a two dimensional array that the function stores the read configuration data
                     );
/*!  Write the LTC6811 PWM register
*/
void LTC6811_wrpwm(uint8_t nIC, //!< number of ICs in the daisy chain
                   uint8_t pwmReg,
                   cell_asic ic[]
                  );

/*!  Reads pwm registers of a LTC6811 daisy chain

*/
int8_t LTC6811_rdpwm(uint8_t nIC, //!< number of ICs in the daisy chain
                     uint8_t pwmReg,
                     cell_asic ic[] //!< a two dimensional array that the function stores the read pwm data
                    );

/*!  Write the LTC6811 Sctrl register
*/
void LTC6811_wrsctrl(uint8_t nIC, //!< number of ICs in the daisy chain
                     uint8_t sctrl_reg,
                     cell_asic ic[]
                    );


/*!  Reads sctrl registers of a LTC6811 daisy chain
@return int8_t, PEC Status.
  0: Data read back has matching PEC
  -1: Data read back has incorrect PEC
*/
int8_t LTC6811_rdsctrl(uint8_t nIC, //!< number of ICs in the daisy chain
                       uint8_t sctrl_reg,
                       cell_asic ic[] //!< a two dimensional array that the function stores the read pwm data
                      );


/*!  Start Sctrl data communication
This command will start the sctrl pulse communication over the spins
*/
void LTC6811_stsctrl(void);


/*!  Write the LTC6811 COMM register
*/
void LTC6811_wrcomm(uint8_t total_ic, //!< Number of ICs in the daisy chain
                    cell_asic ic[] //!< A two dimensional array of the comm data that will be written
                   );

/*!  Reads comm registers of a LTC6811 daisy chain
@return int8_t, PEC Status.

  0: Data read back has matching PEC

  -1: Data read back has incorrect PEC

*/
int8_t LTC6811_rdcomm(uint8_t total_ic, //!< number of ICs in the daisy chain
                      cell_asic ic[] //!< Two dimensional array that the function stores the read comm data.
                     );

/*!  issues a stcomm command and clocks data out of the COMM register */
void LTC6811_stcomm(void);


/*! @returns returns the register data pattern for a given ADC MD and Self test */
uint16_t LTC6811_st_lookup(
  uint8_t MD, //!< ADC Conversion Mode
  uint8_t ST //!< Self test number
);

void LTC6811_set_discharge(int Cell,
                           uint8_t total_ic,
                           cell_asic ic[]);

/*! Helper function that runs the ADC Self Tests*/
int16_t LTC6811_run_cell_adc_st(uint8_t adc_reg,
                                uint8_t total_ic,
                                cell_asic ic[]);

/*! Helper function that runs the ADC Digital Redudancy commands and checks output for errors*/
int16_t LTC6811_run_adc_redundancy_st(uint8_t adc_mode,
                                      uint8_t adc_reg,
                                      uint8_t total_ic,
                                      cell_asic ic[]);

/*! Helper function that runs the datasheet open wire algorithm*/
uint64_t LTC6811_run_openwire_multi(uint8_t total_ic,
                          cell_asic ic[],
													bool print);

void LTC6811_set_discharge(int Cell,
                           uint8_t total_ic,
                           cell_asic ic[]);

/*! Helper Function that runs the ADC Overlap test*/
uint16_t LTC6811_run_adc_overlap(uint8_t total_ic,
                                 cell_asic ic[]);

/*! Helper Function that counts overall PEC errors and register/IC PEC errors*/
void LTC6811_check_pec(uint8_t total_ic,
                       uint8_t reg,
                       cell_asic ic[]);

/*! Helper Function that resets the PEC error counters */
void LTC6811_reset_crc_count(uint8_t total_ic,
                             cell_asic ic[]);

/*! Helper Function to initialize the CFGR data structures*/
void LTC6811_init_cfg(uint8_t total_ic,
                      cell_asic ic[]);

/*! Helper function to set appropriate bits in CFGR register based on bit function*/
void LTC6811_set_cfgr(uint8_t nIC,
                      cell_asic ic[],
                      bool refon,
                      bool adcopt,
                      bool gpio[5],
                      bool dcc[12]);

/*! Helper function to turn the refon bit HIGH or LOW*/
void LTC6811_set_cfgr_refon(uint8_t nIC,
                            cell_asic ic[],
                            bool refon);

/*! Helper function to turn the ADCOPT bit HIGH or LOW*/
void LTC6811_set_cfgr_adcopt(uint8_t nIC,
                             cell_asic ic[],
                             bool adcopt);

/*! Helper function to turn the GPIO bits HIGH or LOW*/
void LTC6811_set_cfgr_gpio(uint8_t nIC,
                           cell_asic ic[],
                           bool gpio[]);

/*! Helper function to turn the DCC bits HIGH or LOW*/
void LTC6811_set_cfgr_dis(uint8_t nIC,
                          cell_asic ic[],
                          bool dcc[]);
/*!  Helper function to set uv field in CFGRA register*/
void LTC6811_set_cfgr_uv(uint8_t nIC,
                         cell_asic ic[],
                         uint16_t uv);

/*!  Helper function to set ov field in CFGRA register*/
void LTC6811_set_cfgr_ov(uint8_t nIC,
                         cell_asic ic[],
                         uint16_t ov);

void LTC6811_init_max_min(uint8_t total_ic,
                          cell_asic ic[],
                          cell_asic ic_max[],
                          cell_asic ic_min[]);

void LTC6811_max_min(uint8_t total_ic, cell_asic ic_cells[],
                     cell_asic ic_min[],
                     cell_asic ic_max[],
                     cell_asic ic_delta[]);
#endif
