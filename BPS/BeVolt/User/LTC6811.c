// LTC6811.c
/**
 * Driver for LTC6811 chip related functions. The IC uses the robust Isolated
 * SPI (isoSPI) to interface with the STM32F4. The LTC6820 is required to convert
 * regular SPI to isoSPI.
 * MCU --SPI--> LTC6820 --isoSPI--> LTC6813
 * @authors Sijin Woo, Chase Block
 * @lastRevised 9/3/2018
 */

#include <stdint.h>
#include "LTC6811.h"
#include "stm32f4xx.h"
#include <string.h>

#define SEND_BUFFER_SIZE 31

#define ADC_MODE_NORMAL 0x080 // ADC Mode Normal
#define ADC_STD 0x260 // Basic required bits for ADC init
#define DCP_YES 0x10 // Discharge permitted
#define ADCV (ADC_MODE_NORMAL | ADC_STD)

#define ADCVSC_STD 0x4C7
#define ADCVSC (ADCVSC_STD | ADC_MODE_NORMAL)

#define WRCFGA 0x001 // Write configuration register group A
#define WRCFGB 0x024 
#define RDCFGA 0x002 // Read configuration register group A
#define RDCFGB 0x026 
#define RDCVA  0x004 // Read cell voltage register group A
#define RDCVB  0x006 // etc.
#define RDCVC  0x008
#define RDCVD  0x00A
#define RDCVE  0x009
#define RDCVF  0x00B
#define RDAUXA 0x00C // Read auxilliary register group A
#define RDAUXB 0x00E // etc.
#define RDAUXC 0x00D
#define RDAUXD 0x00F
#define RDSTATA 0x010 // Read status register group A
#define RDSTATB 0x012 // Read status register group B

// Global variables
uint16_t *RecievedData;

// Private Functions
static void init_PEC15_Table();
uint16_t pec15 (char *data, int len);

// Public Definitions

// TODO: implement
/** LTC6811_Init
 * Initializes SPI for LTC6820 to convert to isoSPI
 * Initializes and configures LTC6811
 */
void LTC6811_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO port A clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;		// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;							// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial = 0x1;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_Rx | SPI_Direction_Tx;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);
}

/** LTC6811_SendCmd
 * Sends command data to LTC6811
 * @param unsigned int 16-bit data
 */
uint16_t *LTC6811_SendCmd(uint16_t *data){
	while(data != NULL){
		//Send
		while((SPI1->SR & 0x080) != 0);
		SPI1->DR = *data;
		data++;
		//Recieve
		while((SPI1->SR & 0x01) != 0){
			
		}
	}
}
	
/** LTC6811_Measure
 * Sends command to LTC6811 to gather and save all ADC values
 * @return unsigned int 16-bit measurements from all ADC's
 */
uint16_t *LTC6811_Measure(void){
	
}

/************************************
Copyright 2012 Analog Devices, Inc. (ADI)
Permission to freely use, copy, modify, and distribute this software for any purpose with or
without fee is hereby granted, provided that the above copyright notice and this permission
notice appear in all copies: THIS SOFTWARE IS PROVIDED “AS IS” AND ADI DISCLAIMS ALL WARRANTIES INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL ADI BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM ANY USE
OF SAME, INCLUDING ANY LOSS OF USE OR DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
***************************************/
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

