// I3G4250D.c
/**
 * Driver for I3G4250D chip related functions. The IC uses SPI to communicate to
 * to MCU.
 *
 * @authors Chase Block
 * @lastRevised 10/13/2018
 */

#include "I3G4250D.h"

/** I3G4250D_Init
 * Initializes SPI to communicate with the gyro slave board (I3G4250D chip)
 * Initializes and configures I3G4250D chip 
 */
void I3G4250D_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO port A clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = CLOCK_PIN | MOSI_PIN | MISO_PIN;		// 2) Initialize which pins to use
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;							// 3) Set PA8 and PA9 as alternate function
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;							// 4) Set the resistor to pull-up
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;				// 5) Initialize the speed of communication as 25 MHz
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;						// 6) Set to open drain
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, CLOCK_PIN_SRC, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, MOSI_PIN_SRC, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, MISO_PIN_SRC, GPIO_AF_SPI2);
	
	GPIO_InitStruct.GPIO_Pin = CHIP_SEL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CRCPolynomial = 0x1;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_Rx | SPI_Direction_Tx;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	
	SPI_Init(SPI2, &SPI_InitStruct);
	SPI_Cmd(SPI2, ENABLE);
}

/** I3G4250D_SendCmd
 * Sends command data to I3G4250D
 * @param unsigned int 16-bit data
 */
void I3G4250D_SendCmd(uint16_t *data){
	while(data != NULL){
		//Send
		while((SPI1->SR & 0x80) != 0);
		SPI1->DR = *data;
		data++;
		//Recieve
		while((SPI1-SR & 0x01) != 0){
			//TODO fill out
		}
	}
}

/** I3G4250D_Measure
 * Sends command to I3G4250D to gather and save the current heading
 * @return unsigned int 16-bit measurements from all ADCs
 */
uint16_t *I3G4250D_Measure(void){

}
