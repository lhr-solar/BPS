// I3G4250D.c
/**
 * Driver for I3G4250D chip related functions. The IC uses SPI to communicate to
 * to MCU.
 *
 * @authors Chase Block
 * @lastRevised 10/13/2018
 */

#include "I3G4250D.h"
#include <string.h>

/** I3G4250D_Init
 * Initializes SPI to communicate with the gyro slave board (I3G4250D chip)
 * Initializes and configures I3G4250D chip 
 */
void I3G4250D_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO port A clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = CHIP_SEL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIOB->ODR |= GPIO_Pin_12;	// Bring pin 12 high
}

/** I3G4250D_SendCmd
 * Sends command data to I3G4250D
 * @param unsigned int 16-bit data
 */
uint8_t I3G4250D_SendCmd(uint8_t address, uint16_t *data, uint8_t RW){	
	
	while(data != NULL){
		while((SPI1->SR & 0x080) != 0);
		
	}
}

/** I3G4250D_Measure
 * Sends command to I3G4250D to gather and save the current heading
 * @return unsigned int 16-bit measurements from all ADCs
 */
uint16_t *I3G4250D_Measure(void){

}
