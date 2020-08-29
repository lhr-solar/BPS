#include "BSP_SPI3.h"
#include "stm32f4xx.h"

// Use this macro function to wait until SPI communication is complete
#define SPI3_Wait(SPIx)		while(((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/** SPI3_WriteRead
 * @brief   Sends and receives a byte of data on the SPI line.
 * @param   txData single byte that will be sent to the device.
 * @return  rxData single byte that was read from the device.
 */
static uint8_t SPI3_WriteRead(uint8_t txData){
	SPI3_Wait(SPI3);
	SPI3->DR = txData & 0x00FF;
	SPI3_Wait(SPI3);
	return SPI3->DR & 0x00FF;
}

/**
 * @brief   Initializes the SPI port connected to the AS8510.
 *          This port communicates with the AS8510 voltage and current
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          We are only using the AS8510 to monitor current.
 * @param   None
 * @return  None
 */
void BSP_SPI3_Init(void) {
    //      SPI configuration:
    //          speed : 125kbps
    //          CPOL : 1 (polarity of clock during idle is high)
    //          CPHA : 1 (tx recorded during 2nd edge)
    // Pins:
    //      SPI3:
    //          PC10 : SCK
    //          PC11 : MISO
    //          PC12 : MOSI 
    //          PA15 : CS

    GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	// Initialize pins
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
	
	// Initialize SPI port
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 0;	
	SPI_Init(SPI3, &SPI_InitStruct);
	SPI_Cmd(SPI3, ENABLE);

    // Initialize CS pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief   Transmits data to through SPI.
 * @note    Blocking statement
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI3_Write(uint8_t *txBuf, uint32_t txLen) {
    for(uint32_t i = 0; i < txLen; i++){
		SPI3_WriteRead(txBuf[i]);
	}
}

/**
 * @brief   Gets the data from SPI.
 * @note    Blocking statement
 * @param   rxBuf   data array to store the data that is received.
 * @param   rxLen   length of data array.
 * @return  None
 */
void BSP_SPI3_Read(uint8_t *rxBuf, uint32_t rxLen) {
    for(uint32_t i = 0; i < rxLen; i++){
		rxBuf[i] = SPI3_WriteRead(0x00);
	}
}

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the AS8510 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the AS8510
 *          ignore the data on the line.
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI3_SetStateCS(uint8_t state) {
    // PB6 is the Chip Select pin for the LTC6811
    if(state) {
        GPIO_SetBits(GPIOA, GPIO_Pin_15);
    } else {
        GPIO_ResetBits(GPIOA, GPIO_Pin_15);
    }
}
