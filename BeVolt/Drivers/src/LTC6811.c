/*!
LTC6811-1: Multicell Battery Monitor
@verbatim
  The LTC6811 is a 3rd generation multicell battery stack
  monitor that measures up to 12 series connected battery
  cells with a total measurement error of less than 1.2mV. The
  cell measurement range of 0V to 5V makes the LTC6811
  suitable for most battery chemistries. All 12 cell voltages
  can be captured in 290uS, and lower data acquisition rates
  can be selected for high noise reduction.

  Using the LTC6811-1, multiple devices are connected in
  a daisy-chain with one host processor connection for all
  devices.
@endverbatim

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

//! @ingroup BMS
//! @{
//! @defgroup LTC6811-1 LTC6811-1: Multicell Battery Monitor
//! @}

/*! @file
    @ingroup LTC6811-1
    Library for LTC6811-1 Multicell Battery Monitor
*/

#include <stdint.h>
#include "LTC681x.h"
#include "LTC6811.h"
#include "config.h"
#include "stm32f4xx.h"
#include "SPI.h"

/*********************************************************/
/*** Code that was added by UTSVT. ***/
/*********************************************************/

static void cs_set(uint8_t state){
	if(state == 0){
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	}else{
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
	}
}
void delay_u(uint16_t micro){
  uint32_t delay = SystemCoreClock / 1000000;
	for(int i = 0; i < micro; i++){
		for(int j = 0; j < delay; j++);
	}
}

void delay_m(uint16_t milli){
  uint32_t delay = SystemCoreClock / 1000;
	for(int i = 0; i < milli; i++){
		for(int j = 0; j < delay; j++);
	}
}

/* wakeup_idle
 * Wake isoSPI up from idle state
 * @param number of ICs in the daisy chain
 */
void wakeup_idle(uint8_t total_ic){
	volatile uint8_t tempReg;		// Temporary buffer
  for (int i = 0; i<total_ic; i++){
    cs_set(0);
    //delayMicroseconds(2); //Guarantees the isoSPI will be in ready mode
    tempReg = SPI1_Read();
    cs_set(1);
  }
}

/* wakeup_sleep
 * Generic wakeup commannd to wake the LTC6811 from sleep
 * @param number of ICs in the daisy chain
 */
void wakeup_sleep(uint8_t total_ic){
  for (int i =0; i<total_ic; i++){
    cs_set(0);
    delay_u(300); // Guarantees the LTC6811 will be in standby
    cs_set(1);
    delay_u(10);
  }
}

/* cmd_68
 * Sense a command to the bms IC. This code will calculate the PEC code for the transmitted command
 * @param 2 Byte array containing the BMS command to be sent
 */ 
void cmd_68(uint8_t tx_cmd[2]){
  uint8_t cmd[4];
  uint16_t cmd_pec;
  //uint8_t md_bits;

  cmd[0] = tx_cmd[0];
  cmd[1] =  tx_cmd[1];
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);
	//cmd[3] = (uint8_t)(cmd_pec >> 8);
  cs_set(0);
  SPI1_WriteMulti(cmd, 4);
  cs_set(1);
}

/* write_68
 * Writes an array of data to the daisy chain. Function calculated PEC for tx_cmd data
 * @param unsigned int 8-bit total number of ICs
 * @param command to transfer data
 * @param array of the data
 */
void write_68(uint8_t total_ic , uint8_t tx_cmd[2], uint8_t data[]){
  const uint8_t BYTES_IN_REG = 6;
  const uint8_t CMD_LEN = 4+(8*total_ic);
  uint8_t cmd[CMD_LEN];
  uint16_t data_pec;
  uint16_t cmd_pec;
  uint8_t cmd_index;

  cmd[0] = tx_cmd[0];
  cmd[1] = tx_cmd[1];
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);
  cmd_index = 4;
	
  for (uint8_t current_ic = total_ic; current_ic > 0; current_ic--){      // executes for each LTC6811 in daisy chain, this loops starts with
    // the last IC on the stack. The first configuration written is
    // received by the last IC in the daisy chain

    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++){
      cmd[cmd_index] = data[((current_ic-1)*6)+current_byte];
      cmd_index = cmd_index + 1;
    }

    data_pec = (uint16_t)pec15_calc(BYTES_IN_REG, &data[(current_ic-1)*6]);    // calculating the PEC for each Iss configuration register data
    cmd[cmd_index] = (uint8_t)(data_pec >> 8);
    cmd[cmd_index + 1] = (uint8_t)data_pec;
    cmd_index = cmd_index + 2;
  }
	
  cs_set(0);
  SPI1_WriteMulti(cmd, CMD_LEN);
  cs_set(1);
}

/* read_68
 * Generic function to write 6811 commands and read data. Function calculated PEC for tx_cmd data
 * @param unsigned int 8-bit number of ICs in the daisy chain
 * @param 2 Byte array containing the BMS command to be sent
 * @param Array that the read back data will be stored
 * @return 8-bit int PEC Status
 */
int8_t read_68(uint8_t total_ic, uint8_t tx_cmd[2], uint8_t *rx_data){
  const uint8_t BYTES_IN_REG = 8;
  uint8_t cmd[4];
  uint8_t data[256];
  int8_t pec_error = 0;
  uint16_t cmd_pec;
  uint16_t data_pec;
  uint16_t received_pec;

  // data = (uint8_t *) malloc((8*total_ic)*sizeof(uint8_t)); // This is a problem because it can fail

  cmd[0] = tx_cmd[0];
  cmd[1] = tx_cmd[1];
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);

  cs_set(0);
  SPI1_WriteReadMulti(cmd, 4, data, (BYTES_IN_REG*total_ic), true);         //Read the configuration data of all ICs on the daisy chain into
  cs_set(1);                          //rx_data[] array

  for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++){       //executes for each LTC6811 in the daisy chain and packs the data
    //into the r_comm array as well as check the received Config data
    //for any bit errors
    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++){
      rx_data[(current_ic*8)+current_byte] = data[current_byte + (current_ic*BYTES_IN_REG)];
    }
    received_pec = (rx_data[(current_ic*8)+6]<<8) + rx_data[(current_ic*8)+7];
    data_pec = pec15_calc(6, &rx_data[current_ic*8]);
    if (received_pec != data_pec){
      pec_error = -1;
    }
  }

  return(pec_error);
}


/* pec15_calc
 * Calculates  and returns the CRC15
 * @param int 32-bit number of bytes that will be used to calculate a PEC
 * @param array of data that will be used to calculate a PEC
 * @returns the calculated pec15 as an unsigned int
 */
uint16_t pec15_calc(int32_t len, uint8_t *data){
  uint16_t remainder,addr;

  remainder = 16;//initialize the PEC
  for (uint8_t i = 0; i<len; i++){ // loops for each byte in data array
    addr = ((remainder>>7)^data[i])&0xff;//calculate PEC table address

    remainder = (remainder<<8)^crc15Table[addr];
  }
  return(remainder*2);//The CRC15 has a 0 in the LSB so the remainder must be multiplied by 2
}

/** LTC6811_Init 
 * Initializes the LTC6811 and the battery Module (cell_asic) struct.
 * This also initializes SPI1 and the chip select pin for the specific IC.
 * @param cell_asic struct
 */
 void LTC6811_Init(cell_asic *battMod){	
	SPI1_Init();				// Initialize SPI1 for voltage board
	
	// Initialize GPIO PB6 pin for LTC6820 CS pin
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	
	LTC6811_init_cfg(NUM_VOLTAGE_BOARDS, battMod);
	LTC6811_reset_crc_count(NUM_VOLTAGE_BOARDS, battMod);
	LTC6811_init_reg_limits(NUM_VOLTAGE_BOARDS, battMod);
}

/* LTC6811_init_reg_limits
 * Initializes variables for each voltage board
 * @param unsigned int 8-bit Total number of IC
 * @param cell_asic struct
 */
void LTC6811_init_reg_limits(uint8_t total_ic, cell_asic ic[]){
  for (uint8_t cic=0; cic<total_ic; cic++){
    ic[cic].ic_reg.cell_channels=12;
    ic[cic].ic_reg.stat_channels=4;
    ic[cic].ic_reg.aux_channels=6;
    ic[cic].ic_reg.num_cv_reg=4;
    ic[cic].ic_reg.num_gpio_reg=2;
    ic[cic].ic_reg.num_stat_reg=3;
  }
}

/* LTC6811_adcv 
 * Starts cell voltage conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permit
 * @param unsigned int 8-bit Cell Channels to be measured
 */
void LTC6811_adcv(uint8_t MD, uint8_t DCP, uint8_t CH){
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x60 + (DCP<<4) + CH;
  cmd_68(cmd);
}

/* LTC6811_adcvsc
 * Starts cell voltage and SOC conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permit
 */
void LTC6811_adcvsc(uint8_t MD, uint8_t DCP){
  uint8_t cmd[4];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits | 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits | 0x60 | (DCP<<4) | 0x07;
  cmd_68(cmd);
}

/* LTC6811_adcvax
 * Starts cell voltage and GPIO 1&2 conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permit
 */
void LTC6811_adcvax(uint8_t MD, uint8_t DCP){
	uint8_t cmd[4];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits | 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits | ((DCP&0x01)<<4) + 0x6F;
  cmd_68(cmd);
}

/* LTC6811_adol
 * Starts cell voltage overlap conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permitted
 */
void LTC6811_adol(uint8_t MD, uint8_t DCP){
  uint8_t cmd[4];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + (DCP<<4) +0x01;
  cmd_68(cmd);
}

/* LTC6811_cvst
 * Starts cell voltage self test conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 */
void LTC6811_cvst(uint8_t MD, uint8_t ST){
  uint8_t cmd[2];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((ST)<<5) +0x07;
  cmd_68(cmd);
}

/* LTC6811_axst
 * Start an Auxiliary Register Self Test Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 */
void LTC6811_axst(uint8_t MD, uint8_t ST){
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((ST&0x03)<<5) +0x07;
  cmd_68(cmd);
}

/* LTC6811_statst
 * Start a Status Register Self Test Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 */
void LTC6811_statst(uint8_t MD, uint8_t ST){
  uint8_t cmd[2];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + ((ST&0x03)<<5) +0x0F;
  cmd_68(cmd);
}

/* LTC6811_pladc
 * Sends the poll adc command
 * @returns 1 byte read back after a pladc command. If the byte is not 0xFF ADC conversion has completed
 */
uint8_t LTC6811_pladc(){
  uint8_t cmd[4];
  uint8_t adc_state = 0xFF;
  uint16_t cmd_pec;

  cmd[0] = 0x07;
  cmd[1] = 0x14;
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);


  cs_set(0);
  SPI1_WriteMulti(cmd,4);
  cs_set(1);
  return(adc_state);
}

/* LTC6811_pollAdc 
 * Blocks operation until the ADC has finished its conversion
 * @returns the approximate time it took for the ADC function to complete
 */
uint32_t LTC6811_pollAdc(){
  uint32_t counter = 0;
  uint8_t finished = 0;
  uint8_t current_time = 0;
  uint8_t cmd[4];
  uint16_t cmd_pec;


  cmd[0] = 0x07;
  cmd[1] = 0x14;
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);

  cs_set(0);
  SPI1_WriteMulti(cmd, 4);

  while ((counter<200000)&&(finished == 0)){
    current_time = SPI1_Read();
    if (current_time>0){
      finished = 1;
    }
    else{
      counter = counter + 10;
    }
  }
	cs_set(1);

  return(counter);
}

/* LTC6811_adax
 * Start a GPIO and Vref2 Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit GPIO Channels to be converted
 */
void LTC6811_adax(uint8_t MD, uint8_t CHG){
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + 0x60 + CHG ;
  cmd_68(cmd);
}

/* LTC6811_adaxd
 * Start an GPIO Redundancy test
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit GPIO Channels to be converted
 */
void LTC6811_adaxd(uint8_t MD, uint8_t CHG){
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + CHG ;
  cmd_68(cmd);
}

/* LTC6811_adstat
 * Start a Status ADC Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Status Channels to be converted
 */
void LTC6811_adstat(uint8_t MD, uint8_t CHST){
  uint8_t cmd[4];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + 0x68 + CHST ;
  cmd_68(cmd);
}

/* LTC6811_adstatd
 * Start a Status register redundancy test Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Status Channels to be converted
 */
void LTC6811_adstatd(uint8_t MD, uint8_t CHST){
  uint8_t cmd[2];
  uint8_t md_bits;

  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x04;
  md_bits = (MD & 0x01) << 7;
  cmd[1] = md_bits + 0x08 + CHST ;
  cmd_68(cmd);
}

/* LTC6811_adow
 * Start an open wire Conversion
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Discharge Permitted
 */
void LTC6811_adow(uint8_t MD, uint8_t PUP){
  uint8_t cmd[2];
  uint8_t md_bits;
  md_bits = (MD & 0x02) >> 1;
  cmd[0] = md_bits + 0x02;
  md_bits = (MD & 0x01) << 7;
  cmd[1] =  md_bits + 0x28 + (PUP<<6) ;//+ CH;
  cmd_68(cmd);
}

/* LTC6811_rdcv 
 * Reads and parses the LTC6811 cell voltage registers
 * @param unsigned int 8-bit cell voltage register
 * @param unsigned int 8-bit number of ICs in the daisy chain (-1 only)
 * @param array of the parsed cell codes from lowest to highest
 * @return unsigned 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
uint8_t LTC6811_rdcv(uint8_t reg, uint8_t total_ic, cell_asic ic[]){
  int8_t pec_error = 0;
  uint8_t cell_data[LTC6811_NUM_RX_BYT*total_ic];;
  uint8_t c_ic = 0;

  if (reg == 0){
    for (uint8_t cell_reg = 1; cell_reg<ic[0].ic_reg.num_cv_reg+1; cell_reg++){                   //executes once for each of the LTC6811 cell voltage registers
      LTC6811_rdcv_reg(cell_reg, total_ic,cell_data );
      for (int current_ic = 0; current_ic<total_ic; current_ic++){
        if (ic->isospi_reverse == false){
          c_ic = current_ic;
        }
        else{
          c_ic = total_ic - current_ic - 1;
        }
        pec_error = pec_error + parse_cells(current_ic,cell_reg, cell_data,
                                            &ic[c_ic].cells.c_codes[0],
                                            &ic[c_ic].cells.pec_match[0]);
      }
    }
  }
  else{
    LTC6811_rdcv_reg(reg, total_ic,cell_data);
    for (int current_ic = 0; current_ic<total_ic; current_ic++){
      if (ic->isospi_reverse == false){
        c_ic = current_ic;
      }
      else{
        c_ic = total_ic - current_ic - 1;
      }
      pec_error = pec_error + parse_cells(current_ic,reg, &cell_data[8*c_ic], &ic[c_ic].cells.c_codes[0], &ic[c_ic].cells.pec_match[0]);
    }
  }
  LTC6811_check_pec(total_ic,LTC6811_CELL,ic);
  return(pec_error);
}

/* LTC6811_rdaux
 * Reads and parses the LTC6811 auxiliary registers
 * @param unsigned int 8-bit GPIO voltage register
 * @param unsigned int 8-bit number of ICs in the daisy chain
 * @param two dimensional array of the parsed gpio voltage codes
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdaux(uint8_t reg, uint8_t total_ic, cell_asic ic[]){
  int8_t pec_error = 0;
  LTC6811_rdaux(reg,total_ic,ic);
  return (pec_error);
}

/* LTC6811_rdcv_reg
 * Reads the raw cell voltage register data
 * @param unsigned int 8-bit cell voltage register that is read back
 * @param unsigned int 8-bit number of ICs in the system
 * @param array of the unparsed cell codes
 */
void LTC6811_rdcv_reg(uint8_t reg, uint8_t total_ic, uint8_t *data){
  const uint8_t REG_LEN = 8; //number of bytes in each ICs register + 2 bytes for the PEC
  uint8_t cmd[4];
  uint16_t cmd_pec;

  if (reg == 1){     //1: RDCVA
    cmd[1] = 0x04;
    cmd[0] = 0x00;
  }
  else if (reg == 2){ //2: RDCVB
    cmd[1] = 0x06;
    cmd[0] = 0x00;
  }
  else if (reg == 3){ //3: RDCVC
    cmd[1] = 0x08;
    cmd[0] = 0x00;
  }
  else if (reg == 4){ //4: RDCVD
    cmd[1] = 0x0A;
    cmd[0] = 0x00;
  }
  else if (reg == 5){ //4: RDCVE
    cmd[1] = 0x09;
    cmd[0] = 0x00;
  }
  else if (reg == 6){ //4: RDCVF
    cmd[1] = 0x0B;
    cmd[0] = 0x00;
  }

  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);

  cs_set(0);
  SPI1_WriteReadMulti(cmd, 4, data, (REG_LEN*total_ic), true);
  cs_set(1);
}

/* parse_cells 
 * helper function that parses voltage measurement registers
 * @param unsigned int 8-bit current 
 * @param unsigned int 8-bit current IC
 * @param array of the cell data 
 * @param array of the cell codes 
 * @param array of the IC PEC
 * @return signed int 8-bit PEC status
 */
int8_t parse_cells(uint8_t current_ic, uint8_t cell_reg, uint8_t cell_data[], uint16_t *cell_codes, uint8_t *ic_pec){
  const uint8_t BYT_IN_REG = 6;
  const uint8_t CELL_IN_REG = 3;
  int8_t pec_error = 0;
  uint16_t parsed_cell;
  uint16_t received_pec;
  uint16_t data_pec;
  uint8_t data_counter = current_ic*LTC6811_NUM_RX_BYT; //data counter

  for (uint8_t current_cell = 0; current_cell<CELL_IN_REG; current_cell++){  // This loop parses the read back data into cell voltages, it
    // loops once for each of the 3 cell voltage codes in the register

    parsed_cell = cell_data[data_counter] + (cell_data[data_counter + 1] << 8);//Each cell code is received as two bytes and is combined to
    // create the parsed cell voltage code
    cell_codes[current_cell  + ((cell_reg - 1) * CELL_IN_REG)] = parsed_cell;
    data_counter = data_counter + 2;                       //Because cell voltage codes are two bytes the data counter
    //must increment by two for each parsed cell code
  }
  received_pec = (cell_data[data_counter] << 8) | cell_data[data_counter+1]; //The received PEC for the current_ic is transmitted as the 7th and 8th
  //after the 6 cell voltage data bytes
  data_pec = pec15_calc(BYT_IN_REG, &cell_data[(current_ic) * LTC6811_NUM_RX_BYT]);

  if (received_pec != data_pec){
    pec_error = 1;                             //The pec_error variable is simply set negative if any PEC errors
    ic_pec[cell_reg-1]=1;
  }
  else{
    ic_pec[cell_reg-1]=0;
  }
  data_counter=data_counter+2;
  return(pec_error);
}

/* LTC6811_rdstat_reg
 * Reads a single stat register and stores the read data in the *data point as a byte array
 * @param unsigned int 8-bit stat register
 * @param unsigned int 8-bit number of ICs
 * @param unsigned int 8-bit pointer to the array of the unparsed state codes
 */
void LTC6811_rdstat_reg(uint8_t reg, uint8_t total_ic, uint8_t *data){
  const uint8_t REG_LEN = 8; // number of bytes in the register + 2 bytes for the PEC
  uint8_t cmd[4];
  uint16_t cmd_pec;
	
  if (reg == 1){     //Read back statiliary group A
    cmd[1] = 0x10;
    cmd[0] = 0x00;
  }
  else if (reg == 2){  //Read back statiliary group B
    cmd[1] = 0x12;
    cmd[0] = 0x00;
  }
  else{          //Read back statiliary group A
    cmd[1] = 0x10;
    cmd[0] = 0x00;
  }

  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);

  cs_set(0);
  SPI1_WriteReadMulti(cmd, 4, data, (REG_LEN*total_ic), true);
  cs_set(1);
}

/* LTC6811_rdstat
 * Reads and parses the LTC6811 stat registers.
 * @param unsigned int 8-bit Stat register
 * @param unsigned int 8-bit number of ICs in the system
 * @param two dimensional array of the stat codes
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdstat(uint8_t reg, uint8_t total_ic, cell_asic ic[]){
  const uint8_t BYT_IN_REG = 6;
  const uint8_t GPIO_IN_REG = 3;

  uint8_t data[LTC6811_NUM_RX_BYT*total_ic];
  uint8_t data_counter = 0;
  int8_t pec_error = 0;
  uint16_t parsed_stat;
  uint16_t received_pec;
  uint16_t data_pec;
  uint8_t c_ic = 0;

  if (reg == 0){
    for (uint8_t stat_reg = 1; stat_reg< 3; stat_reg++){                      //executes once for each of the LTC6811 stat voltage registers
      data_counter = 0;
      LTC6811_rdstat_reg(stat_reg, total_ic,data);                            //Reads the raw statiliary register data into the data[] array
			for (uint8_t current_ic = 0 ; current_ic < total_ic; current_ic++){      // executes for every LTC6811 in the daisy chain
        if (ic->isospi_reverse == false){
          c_ic = current_ic;
        }
        else{
          c_ic = total_ic - current_ic - 1;
        }
        // current_ic is used as the IC counter
        if (stat_reg ==1){
          for (uint8_t current_gpio = 0; current_gpio< GPIO_IN_REG; current_gpio++){ // This loop parses the read back data into GPIO voltages, it
            // loops once for each of the 3 gpio voltage codes in the register
            parsed_stat = data[data_counter] + (data[data_counter+1]<<8);              //Each gpio codes is received as two bytes and is combined to
            ic[c_ic].stat.stat_codes[current_gpio] = parsed_stat;
            data_counter=data_counter+2;                                               //Because gpio voltage codes are two bytes the data counter
          }
        }
        else if (stat_reg == 2){
          parsed_stat = data[data_counter] + (data[data_counter+1]<<8);              //Each gpio codes is received as two bytes and is combined to
          data_counter = data_counter +2;
          ic[c_ic].stat.stat_codes[3] = parsed_stat;
          ic[c_ic].stat.flags[0] = data[data_counter++];
          ic[c_ic].stat.flags[1] = data[data_counter++];
          ic[c_ic].stat.flags[2] = data[data_counter++];
          ic[c_ic].stat.mux_fail[0] = (data[data_counter] & 0x02)>>1;
          ic[c_ic].stat.thsd[0] = data[data_counter++] & 0x01;
        }
        received_pec = (data[data_counter]<<8)+ data[data_counter+1];          //The received PEC for the current_ic is transmitted as the 7th and 8th
        //after the 6 gpio voltage data bytes
        data_pec = pec15_calc(BYT_IN_REG, &data[current_ic*LTC6811_NUM_RX_BYT]);

        if (received_pec != data_pec){
          pec_error = -1; //The pec_error variable is simply set negative if any PEC errors
          ic[c_ic].stat.pec_match[stat_reg-1]=1;
          //are detected in the received serial data
        }
        else{
          ic[c_ic].stat.pec_match[stat_reg-1]=0;
        }
        data_counter=data_counter+2;                        //Because the transmitted PEC code is 2 bytes long the data_counter
        //must be incremented by 2 bytes to point to the next ICs gpio voltage data
      }
    }
  }
  else{
    LTC6811_rdstat_reg(reg, total_ic, data);
    for (int current_ic = 0 ; current_ic < total_ic; current_ic++){            // executes for every LTC6811 in the daisy chain
      // current_ic is used as an IC counter
      if (ic->isospi_reverse == false){
        c_ic = current_ic;
      }
      else{
        c_ic = total_ic - current_ic - 1;
      }
      if (reg == 1){
        for (uint8_t current_gpio = 0; current_gpio< GPIO_IN_REG; current_gpio++){ // This loop parses the read back data into GPIO voltages, it
          // loops once for each of the 3 gpio voltage codes in the register
          parsed_stat = data[data_counter] + (data[data_counter+1]<<8);              //Each gpio codes is received as two bytes and is combined to
          // create the parsed gpio voltage code
          ic[c_ic].stat.stat_codes[current_gpio] = parsed_stat;
          data_counter=data_counter+2;                        //Because gpio voltage codes are two bytes the data counter
          //must increment by two for each parsed gpio voltage code
        }
      }
      else if (reg == 2){
        parsed_stat = data[data_counter++] + (data[data_counter++]<<8);              //Each gpio codes is received as two bytes and is combined to
        ic[c_ic].stat.stat_codes[3] = parsed_stat;
        ic[c_ic].stat.flags[0] = data[data_counter++];
        ic[c_ic].stat.flags[1] = data[data_counter++];
        ic[c_ic].stat.flags[2] = data[data_counter++];
        ic[c_ic].stat.mux_fail[0] = (data[data_counter] & 0x02)>>1;
        ic[c_ic].stat.thsd[0] = data[data_counter++] & 0x01;
      }

      received_pec = (data[data_counter]<<8)+ data[data_counter+1];          //The received PEC for the current_ic is transmitted as the 7th and 8th
      //after the 6 gpio voltage data bytes
      data_pec = pec15_calc(BYT_IN_REG, &data[current_ic*LTC6811_NUM_RX_BYT]);
      if (received_pec != data_pec){
        pec_error = -1;                             //The pec_error variable is simply set negative if any PEC errors
        ic[c_ic].stat.pec_match[reg-1]=1;
      }
      data_counter=data_counter+2;
    }
  }
  LTC6811_check_pec(total_ic,LTC6811_STAT,ic);
  return (pec_error);
}

/* LTC6811_clrcell
 * Clears the cell voltage registers and intiallizes all values to 1
 * Reads back hexadecimal 0xFF after the command is sent
 */
void LTC6811_clrcell(){
  uint8_t cmd[2]= {0x07 , 0x11};
  cmd_68(cmd);
}

/* LTC6811_clraux
 * Clears the Auxiliary registers and initializes all values to 1
 * Reads back hexadecimal 0xFF after the command is sent
 */
void LTC6811_clraux(){
  uint8_t cmd[2]= {0x07 , 0x12};
  cmd_68(cmd);
}

/* LTC6811_clrstat
 * Clears the Stat registers and intiallizes all values to 1
 * Reads back hexadecimal 0xFF after the command is sent
 */
void LTC6811_clrstat(){
  uint8_t cmd[2]= {0x07 , 0x13};
  cmd_68(cmd);
}

/* LTC6811_clrsctrl
 * Clears the Sctrl registers and initializes all values to 0
 * Reads back hexadecimal 0x00 after the command is sent
 */
void LTC6811_clrsctrl(){
  uint8_t cmd[2]= {0x00 , 0x18};
  cmd_68(cmd);
}

/* LTC6811_diagn
 * Starts the Mux Decoder diagnostic self test
 * MUXFAIL bit will be set to 1 for a failure and 0 if the test has been passed
 */
void LTC6811_diagn(){
  uint8_t cmd[2] = {0x07 , 0x15};
  cmd_68(cmd);
}

/* LTC6811_wrcfg 
 * Writes the configuration registers of the LTC6811-1s connected in a daisy chain stack
 * Written in descending order so the last device's configuration is written first
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_wrcfg(uint8_t total_ic, cell_asic ic[]){
  uint8_t cmd[2] = {0x00 , 0x01} ;
  uint8_t write_buffer[256];
  uint8_t write_count = 0;
  uint8_t c_ic = 0;
  for (uint8_t current_ic = 0; current_ic<total_ic; current_ic++){
		if (ic->isospi_reverse == true){
      c_ic = current_ic;
    }
    else{
      c_ic = total_ic - current_ic - 1;
    }

    for (uint8_t data = 0; data<6; data++){
      write_buffer[write_count] = ic[c_ic].config.tx_data[data];
      write_count++;
    }
  }
  write_68(total_ic, cmd, write_buffer);
}

/* LTC6811_rdcfg 
 * Reads configuration registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdcfg(uint8_t total_ic, cell_asic ic[]){
  int8_t pec_error = 0;
  uint8_t cmd[2]= {0x00 , 0x02};
  uint8_t read_buffer[256];
  uint16_t data_pec;
  uint16_t calc_pec;
  uint8_t c_ic = 0;
  pec_error = read_68(total_ic, cmd, read_buffer);
	
  for (uint8_t current_ic = 0; current_ic<total_ic; current_ic++){
    if (ic->isospi_reverse == false){
      c_ic = current_ic;
    }
    else{
      c_ic = total_ic - current_ic - 1;
    }

    for (int byte=0; byte<8; byte++){
      ic[c_ic].config.rx_data[byte] = read_buffer[byte+(8*current_ic)];
    }
    calc_pec = pec15_calc(6,&read_buffer[8*current_ic]);
    data_pec = read_buffer[7+(8*current_ic)] | (read_buffer[6+(8*current_ic)]<<8);
    if (calc_pec != data_pec ){
      ic[c_ic].config.rx_pec_match = 1;
    }
    else ic[c_ic].config.rx_pec_match = 0;
  }
  LTC6811_check_pec(total_ic, LTC6811_CFGR, ic);
  return(pec_error);
}

/* LTC6811_wrpwm
 * Writes the pwm registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width modulation register
 * @param two dimensional array of the configuration data
 */
void LTC6811_wrpwm(uint8_t total_ic, uint8_t pwmReg, cell_asic ic[]){
  uint8_t cmd[2];
  uint8_t write_buffer[256];
  uint8_t write_count = 0;
  uint8_t c_ic = 0;
  if (pwmReg == 0){
    cmd[0] = 0x00;
    cmd[1] = 0x20;
  }
  else{
    cmd[0] = 0x00;
    cmd[1] = 0x1C;
  }

  for (uint8_t current_ic = 0; current_ic<total_ic; current_ic++){
    if (ic->isospi_reverse == true){
      c_ic = current_ic;
    }
    else{
      c_ic = total_ic - current_ic - 1;
    }
    for (uint8_t data = 0; data<6; data++){
      write_buffer[write_count] = ic[c_ic].pwm.tx_data[data];
      write_count++;
    }
  }
  write_68(total_ic, cmd, write_buffer);
}


/* LTC6811_rdpwm
 * Reads pwm registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width modulation register
 * @param two dimensional array of the configuration data
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdpwm(uint8_t total_ic, uint8_t pwmReg, cell_asic ic[]){
  uint8_t cmd[4];
  uint8_t read_buffer[256];
  int8_t pec_error = 0;
  uint16_t data_pec;
  uint16_t calc_pec;
  uint8_t c_ic = 0;

  if (pwmReg == 0){
    cmd[0] = 0x00;
    cmd[1] = 0x22;
  }
  else{
    cmd[0] = 0x00;
    cmd[1] = 0x1E;
  }

  pec_error = read_68(total_ic, cmd, read_buffer);
  for (uint8_t current_ic =0; current_ic<total_ic; current_ic++){
    if (ic->isospi_reverse == false){
      c_ic = current_ic;
    }
    else{
      c_ic = total_ic - current_ic - 1;
    }
    for (int byte=0; byte<8; byte++){
      ic[c_ic].pwm.rx_data[byte] = read_buffer[byte+(8*current_ic)];
    }
    calc_pec = pec15_calc(6,&read_buffer[8*current_ic]);
    data_pec = read_buffer[7+(8*current_ic)] | (read_buffer[6+(8*current_ic)]<<8);
    if (calc_pec != data_pec ){
      ic[c_ic].pwm.rx_pec_match = 1;
    }
    else ic[c_ic].pwm.rx_pec_match = 0;
  }
  return(pec_error);
}

/* LTC6811_wrcomm
 * Writes the COMM registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit pulse width modulation register
 * @param two dimensional array of the comm data
 */
void LTC6811_wrcomm(uint8_t total_ic, cell_asic ic[]){
  uint8_t cmd[2]= {0x07 , 0x21};
  uint8_t write_buffer[256];
  uint8_t write_count = 0;
  uint8_t c_ic = 0;
  for (uint8_t current_ic = 0; current_ic<total_ic; current_ic++){
    if (ic->isospi_reverse == true){
      c_ic = current_ic;
    }
    else{
      c_ic = total_ic - current_ic - 1;
    }

    for (uint8_t data = 0; data<6; data++){
      write_buffer[write_count] = ic[c_ic].com.tx_data[data];
      write_count++;
    }
  }
  write_68(total_ic, cmd, write_buffer);
}

/* LTC6811_rdcomm
 * Reads COMM registers of a LTC6811 daisy chain
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 8-bit PEC Status
 * 		0: No PEC error detected
 *		-1: PEC error detected, retry read
 */
int8_t LTC6811_rdcomm(uint8_t total_ic, cell_asic ic[]){
	uint8_t cmd[2]= {0x07 , 0x22};
  uint8_t read_buffer[256];
  int8_t pec_error = 0;
  uint16_t data_pec;
  uint16_t calc_pec;
  uint8_t c_ic=0;
  pec_error = read_68(total_ic, cmd, read_buffer);
  for (uint8_t current_ic = 0; current_ic<total_ic; current_ic++){
    if (ic->isospi_reverse == false){
      c_ic = current_ic;
    }
    else{
      c_ic = total_ic - current_ic - 1;
    }

    for (int byte=0; byte<8; byte++){
      ic[c_ic].com.rx_data[byte] = read_buffer[byte+(8*current_ic)];
    }
    calc_pec = pec15_calc(6,&read_buffer[8*current_ic]);
    data_pec = read_buffer[7+(8*current_ic)] | (read_buffer[6+(8*current_ic)]<<8);
    if (calc_pec != data_pec ){
      ic[c_ic].com.rx_pec_match = 1;
    }
    else ic[c_ic].com.rx_pec_match = 0;
  }
  return(pec_error);
}

/* LTC6811_stcomm
 * Shifts data in COMM register out over LTC6811 SPI/I2C port
 */
void LTC6811_stcomm(){
	uint8_t cmd[4];
  uint16_t cmd_pec;
	volatile uint8_t temp;

  cmd[0] = 0x07;
  cmd[1] = 0x23;
  cmd_pec = pec15_calc(2, cmd);
  cmd[2] = (uint8_t)(cmd_pec >> 8);
  cmd[3] = (uint8_t)(cmd_pec & 0x00FF);

  cs_set(0);
  SPI1_WriteMulti(cmd,4);
  for (int i = 0; i<9; i++){
    temp = SPI1_Read();
  }
  cs_set(1);
}

/* LTC6811_set_discharge
 * Helper function to set discharge bit in CFG register
 * @param int Cell 
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_set_discharge(int Cell, uint8_t total_ic, cell_asic ic[]){
  for (int i=0; i<total_ic; i++){
    if (Cell<9){
      ic[i].config.tx_data[4] = ic[i].config.tx_data[4] | (1<<(Cell-1));
    }
    else if (Cell < 13){
      ic[i].config.tx_data[5] = ic[i].config.tx_data[5] | (1<<(Cell-9));
    }
  }
}

/* LTC6811_st_lookup
 * Looks up the result pattern for digital filter self test
 * @param unsigned int 8-bit ADC Mode
 * @param unsigned int 8-bit Self Test
 * @return unsigned 16-bit int test pattern
 */
uint16_t LTC6811_st_lookup(uint8_t MD, uint8_t ST){
  uint16_t test_pattern = 0;
  if (MD == 1){
    if (ST == 1){
      test_pattern = 0x9565;
    }
    else{
      test_pattern = 0x6A9A;
    }
  }
  else{
    if (ST == 1)
    {
      test_pattern = 0x9555;
    }
    else{
      test_pattern = 0x6AAA;
    }
  }
  return(test_pattern);
}

/* LTC6811_run_cell_adc_st
 * Runs the Digital Filter/ADC Self Test
 * @param unsigned int 8-bit ADC register
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 16-bit error
 */
int16_t LTC6811_run_cell_adc_st(uint8_t adc_reg,uint8_t total_ic, cell_asic ic[]){
  int16_t error = 0;
  uint16_t expected_result = 0;
  for (int self_test = 1; self_test<3; self_test++){    
		expected_result = LTC6811_st_lookup(2,self_test);
    wakeup_idle(total_ic);
    switch (adc_reg){
      case LTC6811_CELL:
        wakeup_idle(total_ic);
        LTC6811_clrcell();
        LTC6811_cvst(2,self_test);
        LTC6811_pollAdc();//this isn't working
        wakeup_idle(total_ic);
        error = LTC6811_rdcv(0, total_ic,ic);
        for (int cic = 0; cic < total_ic; cic++){
          for (int channel=0; channel< ic[cic].ic_reg.cell_channels; channel++){
            if (ic[cic].cells.c_codes[channel] != expected_result){
              error = error+1;
            }
          }
        }
        break;
      case LTC6811_AUX:
        error = 0;
        wakeup_idle(total_ic);
        LTC6811_clraux();
        LTC6811_axst(2,self_test);
        LTC6811_pollAdc();
        delay_m(10);
        wakeup_idle(total_ic);
        LTC6811_rdaux(0, total_ic,ic);
        for (int cic = 0; cic < total_ic; cic++){
          for (int channel=0; channel< ic[cic].ic_reg.aux_channels; channel++){
            if (ic[cic].aux.a_codes[channel] != expected_result){
              error = error+1;
            }
          }
        }
        break;
      case LTC6811_STAT:
        wakeup_idle(total_ic);
        LTC6811_clrstat();
        LTC6811_statst(2,self_test);
        LTC6811_pollAdc();
        wakeup_idle(total_ic);
        error = LTC6811_rdstat(0,total_ic,ic);
        for (int cic = 0; cic < total_ic; cic++){
          for (int channel=0; channel< ic[cic].ic_reg.stat_channels; channel++){
            if (ic[cic].stat.stat_codes[channel] != expected_result){
              error = error+1;
            }
          }
        }
        break;

      default:
        error = -1;
        break;
    }
  }
  return(error);
}

/* LTC6811_run_adc_redundancy_st
 * Runs the ADC Digital Redudancy commands and checks output for errors
 * @param unsigned int 8-bit ADC mode
 * @param unsigned int 8-bit ADC register
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return signed 16-bit error
 */
int16_t LTC6811_run_adc_redundancy_st(uint8_t adc_mode, uint8_t adc_reg, uint8_t total_ic, cell_asic ic[]){
  int16_t error = 0;
  for (int self_test = 1; self_test<3; self_test++){
    wakeup_idle(total_ic);
    switch (adc_reg){
      case LTC6811_AUX:
        LTC6811_clraux();
        LTC6811_adaxd(adc_mode,AUX_CH_ALL);
        LTC6811_pollAdc();
        wakeup_idle(total_ic);
        error = LTC6811_rdaux(0, total_ic,ic);
        for (int cic = 0; cic < total_ic; cic++){
          for (int channel=0; channel< ic[cic].ic_reg.aux_channels; channel++){
            if (ic[cic].aux.a_codes[channel] >= 65280){
              error = error+1;
            }
          }
        }
        break;
      case LTC6811_STAT:
        LTC6811_clrstat();
        LTC6811_adstatd(adc_mode,STAT_CH_ALL);
        LTC6811_pollAdc();
        wakeup_idle(total_ic);
        error = LTC6811_rdstat(0,total_ic,ic);
        for (int cic = 0; cic < total_ic; cic++){
          for (int channel=0; channel< ic[cic].ic_reg.stat_channels; channel++){
            if (ic[cic].stat.stat_codes[channel] >= 65280){
              error = error+1;
            }
          }
        }
        break;

      default:
        error = -1;
        break;
    }
  }
  return(error);
}

/* LTC6811_run_openwire
 * Runs the datasheet algorithm for open wire
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 */
void LTC6811_run_openwire(uint8_t total_ic, cell_asic ic[]){
  uint16_t OPENWIRE_THRESHOLD = 4000;
  const uint8_t  N_CHANNELS = ic[0].ic_reg.cell_channels;

  cell_asic pullUp_cell_codes[total_ic];
  cell_asic pullDwn_cell_codes[total_ic];
  cell_asic openWire_delta[total_ic];
  //int8_t error;

  wakeup_sleep(total_ic);
  LTC6811_adow(MD_7KHZ_3KHZ,PULL_UP_CURRENT);
  LTC6811_pollAdc();
  wakeup_idle(total_ic);
  LTC6811_adow(MD_7KHZ_3KHZ,PULL_UP_CURRENT);
  LTC6811_pollAdc();
  wakeup_idle(total_ic);
  int8_t error = LTC6811_rdcv(0, total_ic,pullUp_cell_codes);

  wakeup_idle(total_ic);
  LTC6811_adow(MD_7KHZ_3KHZ,PULL_DOWN_CURRENT);
  LTC6811_pollAdc();
  wakeup_idle(total_ic);
  LTC6811_adow(MD_7KHZ_3KHZ,PULL_DOWN_CURRENT);
  LTC6811_pollAdc();
  wakeup_idle(total_ic);
  error = LTC6811_rdcv(0, total_ic,pullDwn_cell_codes);

  for (int cic=0; cic<total_ic; cic++){
		ic[cic].system_open_wire =0;
    for (int cell=0; cell<N_CHANNELS; cell++){
      if (pullDwn_cell_codes[cic].cells.c_codes[cell]>pullUp_cell_codes[cic].cells.c_codes[cell]){
        openWire_delta[cic].cells.c_codes[cell] = pullDwn_cell_codes[cic].cells.c_codes[cell] - pullUp_cell_codes[cic].cells.c_codes[cell]  ;
      }
      else{
        openWire_delta[cic].cells.c_codes[cell] = 0;
      }
    }
  }
  for (int cic=0; cic<total_ic; cic++){
    for (int cell=1; cell<N_CHANNELS; cell++){
      if (openWire_delta[cic].cells.c_codes[cell]>OPENWIRE_THRESHOLD){
        ic[cic].system_open_wire += (1<<cell);
      }
    }
    if (pullUp_cell_codes[cic].cells.c_codes[0] == 0){
      ic[cic].system_open_wire += 1;
    }
    if (pullUp_cell_codes[cic].cells.c_codes[N_CHANNELS-1] == 0){
      ic[cic].system_open_wire += (1<<(N_CHANNELS));
    }
  }
}

/* LTC6811_run_adc_overlap
 * Runs the ADC overlap test for the IC
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the configuration data
 * @return unsigned 16-bit error
 */
uint16_t LTC6811_run_adc_overlap(uint8_t total_ic, cell_asic ic[]){
  uint16_t error = 0;
  int32_t measure_delta =0;
  int16_t failure_pos_limit = 20;
  int16_t failure_neg_limit = -20;
  wakeup_idle(total_ic);
  LTC6811_adol(MD_7KHZ_3KHZ,DCP_DISABLED);
  LTC6811_pollAdc();
  wakeup_idle(total_ic);
  error = LTC6811_rdcv(0, total_ic,ic);
  for (int cic = 0; cic<total_ic; cic++){
    measure_delta = (int32_t)ic[cic].cells.c_codes[6]-(int32_t)ic[cic].cells.c_codes[7];
    if ((measure_delta>failure_pos_limit) || (measure_delta<failure_neg_limit)){
      error = error | (1<<(cic-1));
    }
  }
  return(error);
}

/* LTC6811_max_min
 * Checks the difference in max and min and redefines max and min??
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param two dimensional array of the min of the IC cells
 * @param two dimensional array of the max of the IC cells
 * @param two dimensional array of difference in max and min
 */
void LTC6811_max_min(uint8_t total_ic, cell_asic ic_cells[], cell_asic ic_min[], cell_asic ic_max[],cell_asic ic_delta[]) {
  for (int j=0; j < total_ic; j++){
    for (int i = 0; i< 12; i++){
      if (ic_cells[j].cells.c_codes[i]>ic_max[j].cells.c_codes[i]){
				ic_max[j].cells.c_codes[i]=ic_cells[j].cells.c_codes[i];
			}
      else if (ic_cells[j].cells.c_codes[i]<ic_min[j].cells.c_codes[i]){
				ic_min[j].cells.c_codes[i]=ic_cells[j].cells.c_codes[i];
			}
      ic_delta[j].cells.c_codes[i] = ic_max[j].cells.c_codes[i] - ic_min[j].cells.c_codes[i];
    }
  }
}

/* LTC6811_init_max_min
 * Initializes the max and min of the IC cells
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param two dimensional array of the min of the IC cells
 * @param two dimensional array of the max of the IC cells
 */
void LTC6811_init_max_min(uint8_t total_ic, cell_asic ic[],cell_asic ic_max[],cell_asic ic_min[]){
  for (int j=0; j < total_ic; j++){
    for (int i = 0; i< ic[j].ic_reg.cell_channels; i++){
      ic_max[j].cells.c_codes[i]=0;
      ic_min[j].cells.c_codes[i]=0xFFFF;
    }
  }
}

/* LTC6811_check_pec
 * Helper function that counts overall PEC errors and register/IC PEC errors
 * @param unsigned int 8-bit number of ICs 
 * @param unsigned int 8-bit register
 * @param two dimensional array of the IC cells
 */
void LTC6811_check_pec(uint8_t total_ic, uint8_t reg, cell_asic ic[]){
	switch (reg){
    case LTC6811_CFGR:
      for (int current_ic = 0 ; current_ic < total_ic; current_ic++){
        ic[current_ic].crc_count.pec_count = ic[current_ic].crc_count.pec_count + ic[current_ic].config.rx_pec_match;
        ic[current_ic].crc_count.cfgr_pec = ic[current_ic].crc_count.cfgr_pec + ic[current_ic].config.rx_pec_match;
      }
      break;
    case LTC6811_CFGRB:
      for (int current_ic = 0 ; current_ic < total_ic; current_ic++){
        ic[current_ic].crc_count.pec_count = ic[current_ic].crc_count.pec_count + ic[current_ic].configb.rx_pec_match;
        ic[current_ic].crc_count.cfgr_pec = ic[current_ic].crc_count.cfgr_pec + ic[current_ic].configb.rx_pec_match;
      }
      break;
    case LTC6811_CELL:
      for (int current_ic = 0 ; current_ic < total_ic; current_ic++){
        for (int i=0; i<ic[0].ic_reg.num_cv_reg; i++){
          ic[current_ic].crc_count.pec_count = ic[current_ic].crc_count.pec_count + ic[current_ic].cells.pec_match[i];
          ic[current_ic].crc_count.cell_pec[i] = ic[current_ic].crc_count.cell_pec[i] + ic[current_ic].cells.pec_match[i];
        }
      }
      break;
    case LTC6811_AUX:
      for (int current_ic = 0 ; current_ic < total_ic; current_ic++){
        for (int i=0; i<ic[0].ic_reg.num_gpio_reg; i++){
          ic[current_ic].crc_count.pec_count = ic[current_ic].crc_count.pec_count + (ic[current_ic].aux.pec_match[i]);
          ic[current_ic].crc_count.aux_pec[i] = ic[current_ic].crc_count.aux_pec[i] + (ic[current_ic].aux.pec_match[i]);
        }
      }
      break;
    case LTC6811_STAT:
      for (int current_ic = 0 ; current_ic < total_ic; current_ic++){
        for (int i=0; i<ic[0].ic_reg.num_stat_reg-1; i++){
          ic[current_ic].crc_count.pec_count = ic[current_ic].crc_count.pec_count + ic[current_ic].stat.pec_match[i];
          ic[current_ic].crc_count.stat_pec[i] = ic[current_ic].crc_count.stat_pec[i] + ic[current_ic].stat.pec_match[i];
        }
      }
      break;
    default:
      break;
  }
}

/* LTC6811_reset_crc_count
 * Helper Function to reset PEC counters
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 */
void LTC6811_reset_crc_count(uint8_t total_ic, cell_asic ic[]){
  for (int current_ic = 0 ; current_ic < total_ic; current_ic++){
    ic[current_ic].crc_count.pec_count = 0;
    ic[current_ic].crc_count.cfgr_pec = 0;
    for (int i=0; i<6; i++){
      ic[current_ic].crc_count.cell_pec[i]=0;
    }
    for (int i=0; i<4; i++){
      ic[current_ic].crc_count.aux_pec[i]=0;
    }
    for (int i=0; i<2; i++){
      ic[current_ic].crc_count.stat_pec[i]=0;
    }
  }
}

/* LTC6811_init_cfg
 * Helper function to intialize the CFGR data structures
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 */
void LTC6811_init_cfg(uint8_t total_ic, cell_asic ic[]){
  bool REFON = true;
  bool ADCOPT = false;
  bool gpioBits[5] = {true,true,true,true,true};
  bool dccBits[12] = {false,false,false,false,false,false,false,false,false,false,false,false};
  for (uint8_t current_ic = 0; current_ic<total_ic; current_ic++){
    for (int j =0; j<6; j++){
      ic[current_ic].config.tx_data[j] = 0;
      ic[current_ic].configb.tx_data[j] = 0;
    }
    LTC6811_set_cfgr(current_ic ,ic,REFON,ADCOPT,gpioBits,dccBits);
  }
}

/* LTC6811_set_cfgr
 * Helper function to set appropriate bits in CFGR register based on bit function
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param boolean reference powered up
 * @param boolean ADC mode option bit
 * @param GPIO Pin 5
 * @param Discharge Cell 12
 */
void LTC6811_set_cfgr(uint8_t nIC, cell_asic ic[], bool refon, bool adcopt, bool gpio[5],bool dcc[12]){
  LTC6811_set_cfgr_refon(nIC,ic,refon);
  LTC6811_set_cfgr_adcopt(nIC,ic,adcopt);
  LTC6811_set_cfgr_gpio(nIC,ic,gpio);
  LTC6811_set_cfgr_dis(nIC,ic,dcc);
}

/* LTC6811_set_cfgr_refon
 * Helper function to set the REFON bit
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param boolean reference powered up
 */
void LTC6811_set_cfgr_refon(uint8_t nIC, cell_asic ic[], bool refon){
  if (refon) ic[nIC].config.tx_data[0] = ic[nIC].config.tx_data[0]|0x04;
  else ic[nIC].config.tx_data[0] = ic[nIC].config.tx_data[0]&0xFB;
}

/* LTC6811_set_cfgr
 * Helper function to set the adcopt bit
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param boolean ADC mode option bit
 */
void LTC6811_set_cfgr_adcopt(uint8_t nIC, cell_asic ic[], bool adcopt){
  if (adcopt) ic[nIC].config.tx_data[0] = ic[nIC].config.tx_data[0]|0x01;
  else ic[nIC].config.tx_data[0] = ic[nIC].config.tx_data[0]&0xFE;
}

/* LTC6811_set_cfgr
 * Helper function to set GPIO bits
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param GPIO Pin 5
 */
void LTC6811_set_cfgr_gpio(uint8_t nIC, cell_asic ic[],bool gpio[5]){
  for (int i =0; i<5; i++){
    if (gpio[i])ic[nIC].config.tx_data[0] = ic[nIC].config.tx_data[0]|(0x01<<(i+3));
    else ic[nIC].config.tx_data[0] = ic[nIC].config.tx_data[0]&(~(0x01<<(i+3)));
  }
}

/* LTC6811_set_cfgr_dis
 * Helper function to turn the DCC bits HIGH or LOW
 * @param unsigned int 8-bit number of ICs 
 * @param two dimensional array of the IC cells
 * @param Discharge Cell 12
 */
void LTC6811_set_cfgr_dis(uint8_t nIC, cell_asic ic[],bool dcc[12]){
	for (int i =0; i<8; i++){
    if (dcc[i])ic[nIC].config.tx_data[4] = ic[nIC].config.tx_data[4]|(0x01<<i);
    else ic[nIC].config.tx_data[4] = ic[nIC].config.tx_data[4]& (~(0x01<<i));
  }
  for (int i =0; i<4; i++){
    if (dcc[i+8])ic[nIC].config.tx_data[5] = ic[nIC].config.tx_data[5]|(0x01<<i);
    else ic[nIC].config.tx_data[5] = ic[nIC].config.tx_data[5]&(~(0x01<<i));
  }
}

/* LTC6811_set_cfgr_uv
 * Helper Function to set uv value in CFGRA register
 * @param unsigned int 8-bit number of ICs ?
 * @param two dimensional array of the IC cells
 * @param unsigned int 16-bit uv value
 */
void LTC6811_set_cfgr_uv(uint8_t nIC, cell_asic ic[],uint16_t uv){
  uint16_t tmp = (uv/16)-1;
  ic[nIC].config.tx_data[1] = 0x00FF & tmp;
  ic[nIC].config.tx_data[2] = ic[nIC].config.tx_data[2]&0xF0;
  ic[nIC].config.tx_data[2] = ic[nIC].config.tx_data[2]|((0x0F00 & tmp)>>8);
}

/* LTC6811_set_cfgr_ov
 * Helper function to set OV value in CFGRA register
 * @param unsigned int 8-bit number of ICs ?
 * @param two dimensional array of the IC cells
 * @param unsigned int 16-bit ov value
 */
void LTC6811_set_cfgr_ov(uint8_t nIC, cell_asic ic[],uint16_t ov){
  uint16_t tmp = (ov/16);
  ic[nIC].config.tx_data[3] = 0x00FF & (tmp>>4);
  ic[nIC].config.tx_data[2] = ic[nIC].config.tx_data[2]&0x0F;
  ic[nIC].config.tx_data[2] = ic[nIC].config.tx_data[2]|((0x000F & tmp)<<4);
}

const uint16_t crc15Table[256] =  {0x0,0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac,  //!<precomputed CRC15 Table
                               0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
                               0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
                               0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
                               0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
                               0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
                               0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
                               0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
                               0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
                               0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
                               0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
                               0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
                               0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
                               0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
                               0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
                               0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
                               0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
                               0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
                               0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
                               0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
                               0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
                               0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
                               0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095};
