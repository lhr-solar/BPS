/** LTC2983.c
 * Driver for LTC2983 chip related functions. The IC uses SPI to communicate to
 * to MCU. IsoSPI is not required since temperature sensors are galvanically isolated.
 * regular SPI to isoSPI.
 * If using SPI: MCU --SPI--> LTC2983
 * If using isoSPI: MCU --SPI--> LTC6820 --isoSPI--> LTC2983
 */

#include "LTC2983.h"

const uint8_t READ_CMD = 0x03;
const uint8_t WRITE_CMD = 0x02;
const uint16_t CHANNEL_RESULTS_OFFSET = 0x0010;

/** LTC2983_Init
 * Initializes and configures LTC2983 chip 
 * @preconditions		SPI_Init8 and GPIO Pins: PC6-8 & PB13-15 are initialized
 */
void LTC2983_Init(void){
	// CMD(0), Channel Address(1-2), Data or Command(3-6)
	uint8_t setADCMode[7] = {WRITE_CMD, 0x02, 0x00, 0xF4, 0x00, 0x00, 0x00};
	uint8_t maskConfig[7] = {WRITE_CMD, 0x00, 0xF4, 0x00, 0x0F, 0xFF, 0xFF};	

	GPIO_InitTypeDef GPIO_InitStruct;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
//	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// Wait until LTC2983 is ready starting up (might not be necessary due to timing)
	while(!LTC2983_Ready()) {
		//printf("Delay Init\n\r");
	};
	
	// Set channels for board 1 to Direct ADC mode
	for(board tempBoard = TEMP_CS1; tempBoard <= TEMP_CS1; tempBoard++) {
		for(uint32_t i = 0; i < NUM_SENSORS_ON_TEMP_BOARD_1; i++) {
			Board_Select(tempBoard, 0);
			//SPI_WriteMulti8(message, 7);
			SPI_Write8(setADCMode[0]);
			SPI_Write8(setADCMode[1]);
			SPI_Write8(setADCMode[2]);
			SPI_Write8(setADCMode[3]);
			SPI_Write8(setADCMode[4]);
			SPI_Write8(setADCMode[5]);
			SPI_Write8(setADCMode[6]);			
			Board_Select(tempBoard, 1);
			for(uint32_t i = 0; i < 80000; i++) {}
			setADCMode[2] += 4;					// Increment to next channel

		}
		setADCMode[2] = 0;			// reset channel selection
		
		// Set multichannel conversion setting
		Board_Select(tempBoard, 0);
		//SPI_WriteMulti8(maskConfig, 7);
		SPI_Write8(maskConfig[0]);
		SPI_Write8(maskConfig[1]);
		SPI_Write8(maskConfig[2]);
		SPI_Write8(maskConfig[3]);
		SPI_Write8(maskConfig[4]);
		SPI_Write8(maskConfig[5]);
		SPI_Write8(maskConfig[6]);			
		Board_Select(tempBoard, 1);
	}
 
	while(!LTC2983_Ready()) {
		// Spin
	};
}

/** Board_Select
 * Uses specified CS pins for selected board.
 * @param enumerated type Board. List in "Settings.h". If correct board isn't selected, don't do anything
 * @param state of the CS line (1 or 0). Note that SPI CS lines are HIGH or 1 during idle
 */
void Board_Select(board tempBoard, bool state) {
	switch(tempBoard) {
		case TEMP_CS1: 
			if(state) GPIOB->ODR |= GPIO_Pin_13;
			else GPIOB->ODR &= ~GPIO_Pin_13;
			break;
		case TEMP_CS2:
			if(state) GPIOB->ODR |= GPIO_Pin_14;
			else GPIOB->ODR &= ~GPIO_Pin_14;
			break;
			
		case TEMP_CS3:
			if(state) GPIOB->ODR |= GPIO_Pin_15;
			else GPIOB->ODR &= ~GPIO_Pin_15;
			break;
		
		case TEMP_CS4:
			if(state) GPIOC->ODR |= GPIO_Pin_6;
			else GPIOC->ODR &= ~GPIO_Pin_6;
			break;
		
		case TEMP_CS5:
			if(state) GPIOC->ODR |= GPIO_Pin_7;
			else GPIOC->ODR &= ~GPIO_Pin_7;
			break;
			
		case TEMP_CS6:	
			if(state) GPIOC->ODR |= GPIO_Pin_8;
			else GPIOC->ODR &= ~GPIO_Pin_8;
			break;
			
		default:	return;
	}

}


/** LTC2983_Ready
 * Checks if LTC2984 is ready after startup
 * @return unsigned int 16-bit data
 */
bool LTC2983_Ready(void){
//	while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) {
//		printf("Current Low\n\r");
//	}printf("?\n\r");	
//	for(int i = 0; i < 80000; i++);
	return true;
	uint8_t message[3] = {READ_CMD, 0x00, 0x00};
	uint8_t result;

	GPIOB->ODR &= ~GPIO_Pin_13;
	//SPI_WriteReadMulti8(message, 3, &result, 1, true);
	//for(uint32_t i = 0; i < 80000; i++) {}
	
	SPI_Write8(message[0]);
	SPI_Write8(message[1]);
	SPI_Write8(message[2]);
	result = SPI_Read8();	

	GPIOB->ODR |= GPIO_Pin_13;

	for(int i = 0; i < 80000; i++);
	
  if((result & 0x40) == 0x40) {
		return true;
	} else {
		return false;
	}
	
}



/** LTC2983_MeasureSingleChannel
 * Sends command to LTC2983 to gather and save all ADC values
 * @param Channel number to read (1 - 20)
 * @return signed 32-bit measurements from specified channel.
 *       returns a -1 if invalid ADC measurement
 */
int32_t LTC2983_MeasureSingleChannel(uint8_t channel){
	// Data Messages Declarations
	uint8_t result[4];
	
	//channel -= 1;
	uint8_t message[4] = {WRITE_CMD, 0x00, 0x00, 0x81};	 // Command, Address, Data
				// message for start conversion
	uint8_t receive[3] = {READ_CMD, 0x00, 0x10};
				// Read conversion result from channel 1 
	message[3] += channel;
	receive[2] += (channel * 4);			// 4 bytes per channel

	GPIOB->ODR &= ~GPIO_Pin_13;
	SPI_WriteMulti8(message, 4);
	GPIOB->ODR |= GPIO_Pin_13;
	
	
	while(!LTC2983_Ready()) {
		
		GPIOB->ODR &= ~GPIO_Pin_13;
		//SPI_WriteMulti8(message, 4);			// restart conversion
			SPI_Write8(message[0]);
			SPI_Write8(message[1]);
			SPI_Write8(message[2]);
			SPI_Write8(message[3]);
		GPIOB->ODR |= GPIO_Pin_13;
		
		//printf("Not ready.\n\r");
	}

	GPIOB->ODR &= ~GPIO_Pin_13;
	//SPI_WriteReadMulti8(receive, 3, result, 4, true);
	SPI_Write8(receive[0]);
	SPI_Write8(receive[1]);
	SPI_Write8(receive[2]);
	result[3] = SPI_Read8();	
	result[2] = SPI_Read8();
	result[1] = SPI_Read8();
	result[0] = SPI_Read8();
	
for(int i = 0; i < 80000; i++);
	GPIOB->ODR |= GPIO_Pin_13;	
	printf("%d\n\r", result);
	
	LTC2983_ConversionValidity(result[0]);
	
	if((result[3] & 0x01) == 0x01) {
		return (*(int32_t *)result & 0x007FFFFF);
	} else {
		return -1;
	}
		
}

/** LTC2983_StartMeasuringADC
 * Starts direct ADC conversion for all channels of selected board
 * @note Conversions are initiated consecutively so if you read the Command
 *		Status Register or the INTERRUPT pin, then it won't be ready until All
 *		channels are finished converting.
 * @param Selected Temperature Board CS (enumerated type in "Settings.h")
 */
void LTC2983_StartMeasuringADC(board temperatureBoard) {
	// CMD(0), Channel Address(1-2), Data or Command(3-6)
	uint8_t startConversion[4] = {WRITE_CMD, 0x00, 0x00, 0x81};
	
	// Start multichannel conversion for all channels.
	Board_Select(temperatureBoard, 0);
	//SPI_WriteMulti8(startConversion, 4);
	SPI_Write8(startConversion[0]);
	SPI_Write8(startConversion[1]);
	SPI_Write8(startConversion[2]);
	SPI_Write8(startConversion[3]);	
	
	Board_Select(temperatureBoard, 1);
	
	// Channel conversions are initiated consecutively and CSR won't be ready until all channels are ready.
	while(!LTC2983_Ready()) {
		//printf("Board not ready\n\r");
	}
	
}

/**	LTC2983_ReadConversions
 * Reads all channels from temperature board and stores in buffer
 * @param pointer to a buffer to store conversion results
 * @param temperature board CS (enumerated type in "Settings.h")
 * @param number of Channels, use "Settings.h" definitions
 * @preconditions All channels on the board finished conversion before running this function
 */
void LTC2983_ReadConversions(int32_t *Buf, board temperatureBoard, uint8_t numOfChannels) {
	uint8_t readConversionResult[3] = {READ_CMD, 0x00, 0x10};
	uint8_t result[4];
	
	for(uint32_t i = 0; i < numOfChannels; i++) {
		Board_Select(temperatureBoard, 0);
		
		//SPI_WriteReadMulti8(readConversionResult, 3, result, 4, true);
		//for(uint32_t i = 0; i < 80000; i++){}			// Delay for receiving messages SPI		
		
		SPI_Write8(readConversionResult[0]);
		SPI_Write8(readConversionResult[1]);
		SPI_Write8(readConversionResult[2]);
		result[3] = SPI_Read8();
		result[2] = SPI_Read8();
		result[1] = SPI_Read8();
		result[0] = SPI_Read8();	
		
		Board_Select(temperatureBoard, 1);
		
		readConversionResult[2] += 4;				// Increment to next channel address	
		for(uint32_t i = 0; i < 12*80000; i++){}			
		
		if(i == 0) printf("Result 0 - 0x%x\n\r", result[0]);
		if(i == 0) printf("Result 3 - 0x%x\n\r", result[3]);
		
		//LTC2983_ConversionValidity(result[0]);
//		if(result[3] == 0x01) {
			Buf[i] = *(int32_t *)result;// & 0x007FFFFF;
//		} else {
//			Buf[i] = -1;
//		}
	}
}



/** LTC2983_ConversionValidity
 * Checks the error bits of ADC conversion
 * @param Most significant byte of received data conversion
 * @note Used braces with conditional for future updates
 *			 in case return values or setting flags
 */
void LTC2983_ConversionValidity(uint8_t errorBits) {
	if((errorBits & 0x80) > 0) {
		printf("*** SENSOR HARD FAULT ***\n\r");
	} 
	if((errorBits & 0x40) > 0) {
		printf("*** RANGE HARD FAULT ***\n\r");
	}
	if((errorBits & 0x30) > 0) {
		printf("*** SHOULD NOT BE USED FOR DIRECT ADC ***\n\r");
	}
	if((errorBits & 0x08) > 0) {
		printf("*** MEASUREMENT ABOVE RANGE SOFT FAULT ***\n\r");		
	}
	if((errorBits & 0x04) > 0) {
		printf("*** MEASUREMENT BELOW RANGE SOFT FAULT ***\n\r");
	}
	if((errorBits & 0x02) > 0) {
		printf("*** MEASUREMENT OUT OF RANGE SOFT FAULT ***\n\r");
	}
	if((errorBits & 0x01) != 0x01) {
		printf("*** INVALID CONVERSION ***\n\r");
	}
	
	
}





/** LTC2983_ReadChannel
 * Reads the 24 bit ADC value at a specified channel
 * @preconditions Specified channel to be read finished conversion before running this function
 * @param Selected temperature board CS (enumerated type in "Settings.h")
 * @param unsigned int 16-bit channel number
 * @return signed 32-bit (unconverted) data for channel
 */
int32_t LTC2983_ReadChannel(board temperatureBoard, uint8_t channel) {
	uint8_t convert[2];
	uint16_t channelAddr;
	uint8_t singleChannelResult[4];
	
	while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) {
		printf("Current Low\n\r");
	}
	printf("read\n\r");
	
	channelAddr = (channel * 4) + CHANNEL_RESULTS_OFFSET;		// Get address of the channel (every 4 bytes in memory)
	
	convert[1] = (uint8_t)(channelAddr & 0x00FF);						// Isolate channel address (4 bytes long) into 2 separate bytes
	convert[0] = (uint8_t)((channelAddr & 0xFF00) >> 8);	
	
	uint8_t readSingleChannel[3] = {READ_CMD, convert[0], convert[1]};		// Message for SPI
	
	Board_Select(temperatureBoard, 0);
	SPI_WriteReadMulti8(readSingleChannel, 3, singleChannelResult, 4, true);
	for(int i = 0; i < 80000; i++);
	Board_Select(temperatureBoard, 1);
	
	return *(uint32_t *)singleChannelResult;
}






