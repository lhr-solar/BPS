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
#include "Definition.h"
#include "SPI.h"

// Global variables
uint16_t static modules[NUM_MODULES];		// Holds voltage values of modules

// Private Functions
static void init_PEC15_Table();
uint16_t pec15 (char *data, int len);

// Public Definitions
/** LTC6811_Init
 * Initializes SPI for LTC6820 to convert to isoSPI
 * Initializes and configures LTC6811
 */
void LTC6811_Init(void){
	SPI1_Init();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIOA->MODER
}

/** LTC6811_SendCmd
 * Sends command data to LTC6811
 * @param unsigned int 16-bit data
 */
uint16_t *LTC6811_SendCmd(uint16_t *data){
	
}

/** LTC6811_Measure
 * Sends command to LTC6811 to gather and save all ADC values
 * @return unsigned int 16-bit measurements from all ADC's
 */
uint16_t *LTC6811_Measure(void){
	
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

