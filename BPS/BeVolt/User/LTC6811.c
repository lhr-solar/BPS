// LTC6811.c
/**
 * Driver for LTC6813 chip related functions. The IC uses the robust Isolated
 * SPI (isoSPI) to interface with the STM32F4. The LTC6820 is required to convert
 * regular SPI to isoSPI.
 * MCU --SPI--> LTC6820 --isoSPI--> LTC6811
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "LTC6811.h"
#include "Definition.h"
#include "SPI.h"

// Global variables

// Private Functions
static void init_PEC15_Table();
uint16_t pec15 (char *data, int len);
void adcv(uint8_t MD, uint8_t DCP, uint8_t CH);

// Public Definitions
/** LTC6811_Init
 * Initializes SPI for LTC6820 to convert to isoSPI
 * Initializes and configures LTC6811
 */
void LTC6811_Init(void){	
	SPI_Init8();
	init_PEC15_Table();		// Code received from datasheet
	
	// Initialize Chip Select/Enable pin (PB6)
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO portB clock
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;								// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;						// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIOB->ODR |= GPIO_Pin_6;		// CS high
}

/** LTC6811_SendCmd
 * Sends command to LTC6811
 * Packet format:
 * [16 bits : cmd][16 bits : pec]
 * @param unsigned int 16-bit cmd
 */
void LTC6811_SendCmd(uint16_t txCmd){
	uint8_t cmd[4];		// packet that will be sent
  uint16_t pec;		// packet error check		

	// Format command
  cmd[0] = (txCmd >> 8) & 0xFF;		// Commands are 16 bits but SPI only sends them 8 bits at a time
  cmd[1] = (txCmd & 0xFF);
  pec = pec15((char *)cmd, 2);
  cmd[2] = (uint8_t)(pec >> 8);
  cmd[3] = (uint8_t)(pec);
	
	// Send to LTC6811
	GPIOB->ODR &= ~GPIO_Pin_6;		// CS low
	SPI_Write8(cmd, 4);
	GPIOB->ODR |= GPIO_Pin_6;		// CS high
}

/** LTC6811_Write
 * Sends command then sends data to all LTC6811's
 * Packet format:
 * [16 bits : cmd][16 bits : pec][CMD_LEN - 32 bits : data]
 * @param unsigned 16-bit command
 * @param unsigned 8 bit data packet to be sent
 */
void LTC6811_Write(uint16_t txCmd, uint8_t *data){
	const uint8_t BYTES_IN_REG = 6;
  const uint8_t CMD_LEN = 4+(8*NUM_VOLTAGE_BOARDS);
  uint8_t cmd[CMD_LEN];
  uint16_t dataPec;			// packet error check for data
  uint16_t cmdPec;				// packet error check for cmd
  uint8_t cmdIdx;

	// Format command
  cmd[0] = (txCmd >> 8) & 0xFF;
  cmd[1] = (txCmd & 0xFF);;
  cmdPec = pec15((char *)cmd, 2);
  cmd[2] = (uint8_t)(cmdPec >> 8);
  cmd[3] = (uint8_t)(cmdPec);
  cmdIdx = 4;
	
	// Format data
	// Executes for each LTC681x in daisy chain, this loops starts with
  for (uint8_t currentIC = NUM_VOLTAGE_BOARDS; currentIC > 0; currentIC--){
    // the last IC on the stack. The first configuration written is
    // received by the last IC in the daisy chain
    for (uint8_t currentByte = 0; currentByte < BYTES_IN_REG; currentByte++)    {
      cmd[cmdIdx] = data[((currentIC - 1) * 6) + currentByte];
      cmdIdx = cmdIdx + 1;
    }

		// calculating the PEC for each Iss configuration register data
    dataPec = (uint16_t)pec15((char *)&data[(currentIC - 1) * 6], BYTES_IN_REG);
    cmd[cmdIdx] = (uint8_t)(dataPec >> 8);
    cmd[cmdIdx + 1] = (uint8_t)dataPec;
    cmdIdx = cmdIdx + 2;
  }

	// Send to LTC6811
	GPIOB->ODR &= ~GPIO_Pin_6;		// CS low
	SPI_Write8(cmd, CMD_LEN);
	GPIOB->ODR |= GPIO_Pin_6;		// CS high
}

/** LTC6811_Read
 * Sends command to LTC6811 then reads all the data that the LTC6811 sends back
 * @param unsigned 16-bit command
 * @param pointer to unsigned 8-bit array that will hold data that is received
 * @param max size of rx array
 * @return pecError (just to check if any noise affected communication)
 */
int8_t LTC6811_Read(uint16_t txCmd, uint8_t *rxData, uint32_t rxSize){
	const uint8_t BYTES_IN_REG = 8;
  uint8_t cmd[4];
  uint8_t data[256];
  int8_t pecError = 0;
  uint16_t cmdPec;
  uint16_t dataPec;
  uint16_t receivedPec;

	// Format command
  cmd[0] = (txCmd >> 8) & 0xFF;
  cmd[1] = txCmd & 0xFF;
  cmdPec = pec15((char *)cmd, 2);
  cmd[2] = (uint8_t)(cmdPec >> 8);
  cmd[3] = (uint8_t)(cmdPec);

	// Send to LTC6811
	GPIOB->ODR &= ~GPIO_Pin_6;		// CS low
	SPI_WriteRead8(cmd, 4, data, (BYTES_IN_REG * NUM_VOLTAGE_BOARDS));	//Read the configuration data of all ICs on the daisy chain into
	GPIOB->ODR |= GPIO_Pin_6;		// CS high

	//executes for each LTC681x in the daisy chain and packs the data
  for (uint8_t currentIC = 0; currentIC < NUM_VOLTAGE_BOARDS; currentIC++){
    //into the r_comm array as well as check the received Config data
    //for any bit errors
    for (uint8_t currentByte = 0; currentByte < BYTES_IN_REG; currentByte++){
      rxData[(currentIC * 8) + currentByte] = data[currentByte + (currentIC*BYTES_IN_REG)];
    }
    receivedPec = (rxData[(currentIC*8)+6]<<8) + rxData[(currentIC * 8) + 7];
    dataPec = pec15((char *)&rxData[currentIC * 8], 6);
    if (receivedPec != dataPec){
      pecError = -1;
    }
  }

  return pecError;
}

/** LTC6811_Measure
 * Sends command to LTC6811 to gather and save all ADC values
 * @param ptr to unsigned int 16-bit buffer that will hold new measurements from all ADC's 
 * @param number of modules
 */
void LTC6811_Measure(uint16_t modules, uint32_t size){
	adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
}


/********************************************************************************************************
The follow functions were taken from the LTC6811.cpp library. Only those that are required are copied to
our BPS. To keep our system simple, this driver as simple as possible, we're not including erroneous
functions. If you want to know more of what these functions do, look up the LTC6811 datasheet and search
the commands (for example search adcv for more information about cell measuring)
*********************************************************************************************************/
/*
Starts cell voltage conversion
*/
void adcv(
  uint8_t MD, //ADC Mode
  uint8_t DCP, //Discharge Permit
  uint8_t CH //Cell Channels to be measured
)
{
  uint16_t cmd;
  uint16_t mdBits;

  mdBits = (MD & 0x02) >> 1;
  cmd = ((mdBits + 0x02) << 8) & 0xFF00;
  mdBits = (MD & 0x01) << 7;
  cmd |= (mdBits + 0x60 + (DCP<<4) + CH) & 0xFF;
  LTC6811_SendCmd(cmd);
}

//Reads and parses the LTC681x cell voltage registers.
uint8_t rdcv(uint8_t reg, // Controls which cell voltage register is read back.
                     uint8_t total_ic, // the number of ICs in the system
                     cell_asic ic[] // Array of the parsed cell codes
                    )
{
  int8_t pecError = 0;
  uint8_t cell_data[8 * NUM_VOLTAGE_BOARDS];
  uint8_t c_ic = 0;

  if (reg == 0)
  {
    for (uint8_t cell_reg = 1; cell_reg<ic[0].ic_reg.num_cv_reg+1; cell_reg++)                   //executes once for each of the LTC6811 cell voltage registers
    {
      LTC681x_rdcv_reg(cell_reg, total_ic,cell_data );
      for (int current_ic = 0; current_ic<total_ic; current_ic++)
      {
        if (ic->isospi_reverse == false)
        {
          c_ic = current_ic;
        }
        else
        {
          c_ic = total_ic - current_ic - 1;
        }
        pecError = pecError + parse_cells(current_ic,cell_reg, cell_data,
                                            &ic[c_ic].cells.c_codes[0],
                                            &ic[c_ic].cells.pec_match[0]);
      }
    }
  }

  else
  {
    LTC681x_rdcv_reg(reg, total_ic,cell_data);

    for (int current_ic = 0; current_ic<total_ic; current_ic++)
    {
      if (ic->isospi_reverse == false)
      {
        c_ic = current_ic;
      }
      else
      {
        c_ic = total_ic - current_ic - 1;
      }
      pecError = pecError + parse_cells(current_ic,reg, &cell_data[8*c_ic],
                                          &ic[c_ic].cells.c_codes[0],
                                          &ic[c_ic].cells.pec_match[0]);
    }
  }
  LTC681x_check_pec(total_ic,CELL,ic);

  return(pecError);
}

// Reads raw data of voltages from LTC6811
void rdcvReg(uint8_t reg, uint8_t total_ic, uint8_t *data){
	uint8_t cmd[4];
  uint16_t cmdPec;

  if (reg == 1)     //1: RDCVA
  {
    cmd[1] = 0x04;
    cmd[0] = 0x00;
  }
  else if (reg == 2) //2: RDCVB
  {
    cmd[1] = 0x06;
    cmd[0] = 0x00;
  }
  else if (reg == 3) //3: RDCVC
  {
    cmd[1] = 0x08;
    cmd[0] = 0x00;
  }
  else if (reg == 4) //4: RDCVD
  {
    cmd[1] = 0x0A;
    cmd[0] = 0x00;
  }
  else if (reg == 5) //4: RDCVE
  {
    cmd[1] = 0x09;
    cmd[0] = 0x00;
  }
  else if (reg == 6) //4: RDCVF
  {
    cmd[1] = 0x0B;
    cmd[0] = 0x00;
  }


  cmdPec = pec15((char *)cmd, 2);
  cmd[2] = (uint8_t)(cmdPec >> 8);
  cmd[3] = (uint8_t)(cmdPec);

  
  SPI_WriteRead8(cmd,4,data,(8 * NUM_VOLTAGE_BOARDS));
  
}

/*********************************************************************************************************
Copyright 2012 Analog Devices, Inc. (ADI)
Permission to freely use, copy, modify, and distribute this software for any purpose with or
without fee is hereby granted, provided that the above copyright notice and this permission
notice appear in all copies: THIS SOFTWARE IS PROVIDED “AS IS” AND ADI DISCLAIMS ALL WARRANTIES INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL ADI BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM ANY USE
OF SAME, INCLUDING ANY LOSS OF USE OR DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*********************************************************************************************************/
int16_t pec15Table[256];
int16_t CRC15_POLY = 0x4599;
void init_PEC15_Table(){
	uint16_t remainder;
	for (int i = 0; i < 256; i++){
		remainder = i << 7;
		for (int bit = 8; bit > 0; --bit){
			if (remainder & 0x4000){
				remainder = ((remainder << 1));
				remainder = (remainder ^ CRC15_POLY);
			}else{
				remainder = ((remainder << 1));
			}
		}
		pec15Table[i] = remainder&0xFFFF;
	}
}

uint16_t pec15 (char *data , int len){
	uint16_t remainder,address;
	remainder = 16;//PEC seed
	for (int i = 0; i < len; i++){
	address = ((remainder >> 7) ^ data[i]) & 0xff;//calculate PEC table address
	remainder = (remainder << 8 ) ^ pec15Table[address];
	}
	return (remainder*2);//The CRC15 has a 0 in the LSB so the final value must be multiplied by 2
}

