// I3G4250D.c
/**
 * Driver for I3G4250D chip related functions. The IC uses SPI to communicate to
 * to MCU.
 *
 * @authors Chase Block
 * @lastRevised 10/13/2018
 */

#include "Gyro.h"

double headings [3]; // x,y,z

/** ICM20600_Init
 * Initializes SPI for ICM20600
 * Initializes and configures LTC6811
 */
void ICM20600_Init(void){
	
	// This needs to be made to work with the global SPI lines
	
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	// 1) Initialize GPIO port A clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIOB->ODR |= GPIO_Pin_12;	// Bring pin 12 high

	ICM20600_SendCmd(0x13, 0x1F, 0x00, 0x00);	// Initialize settings: activate the unit and set to 12.5 Hz update

	ICM20600_ResetHeadings();
}

/** ICM20600_SendCmd
 * Sends command data to ICM20600
 * @param unsigned int 8-bit address (actually only 7-bits used)
 * @param unsinged int 8-bit data
 * @param unsigned int 8-bit Read/Write' (1/0)
 */
uint16_t *ICM20600_SendCmd(uint8_t address, uint8_t data, uint8_t rw, uint32_t readSize){
	return 0;	// TODO: Complete this function
}

/** ICM20600_Measure
 * Sends command to ICM20600 to gather and save all heading values
 * @return unsigned int 16-bit measurements [X,Y,Z]
 */
uint16_t *ICM20600_Measure(void){
	uint16_t* readVal = ICM20600_SendCmd(OUT_X_MSB, 0x00, 0x01, 0x03);

	return readVal;
}

/** ICM20600_CurrentHeading
 * Updates the heading before returning
 * This should be called periodically (12.5 Hz)
 * @return the current heading of the car
 */
double *ICM20600_CurrentHeading(void){
	int16_t * measurements = (int16_t *) ICM20600_Measure();
	headings[0] += *(measurements++) / 12.5;
	headings[1] += *(measurements++) / 12.5;
	headings[2] += *(measurements) / 12.5;

	return headings;
}

/** ICM20600_ResetHeadings
 * Resets the heading values
 */
void ICM20600_ResetHeadings(void){
	headings[0] = headings[1] = headings[2] = 0;
}

/**
 * @return 0 if the car is not flipped over, 1 if it is
 */
uint8_t ICM20600_IsFlipped(void){
	double * _headings = ICM20600_CurrentHeading();	// Update and grab the values

	if(abs((int)_headings[0]) > 90 || abs((int)_headings[1]) > 90) return 0x01;

	return 0x00;
}
