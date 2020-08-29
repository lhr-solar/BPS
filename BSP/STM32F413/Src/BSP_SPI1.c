#include "BSP_SPI1.h"
#include "stm32f4xx.h"

// Use this macro function to wait until SPI communication is complete
#define SPI_Wait(SPIx)		while(((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/** SPI1_WriteRead
 * @brief   Sends and receives a byte of data on the SPI line.
 * @param   txData single byte that will be sent to the device.
 * @return  rxData single byte that was read from the device.
 */
static uint8_t SPI1_WriteRead(uint8_t txData){
	SPI_Wait(SPI1);
	SPI1->DR = txData & 0x00FF;
	SPI_Wait(SPI1);
	return SPI1->DR & 0x00FF;
}

/**
 * @brief   Initializes the SPI port connected to the LTC6820.
 *          This port communicates with the LTC6811 voltage and temperature
 *          monitoring IC. The LTC6820 converts the SPI pins to 2-wire isolated SPI.
 *          Look at analog devices website and LTC6811's or LTC6820's datasheets.
 * @param   None
 * @return  None
 */
void BSP_SPI1_Init(void) {
    //      SPI configuration:
    //          speed : 125kbps
    //          CPOL : 1 (polarity of clock during idle is high)
    //          CPHA : 1 (tx recorded during 2nd edge)
    // Pins:
    //      SPI1:
    //          PB3 : SCK
    //          PB4 : MISO
    //          PB5 : MOSI 
    //          PB6 : CS

    GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// Initialize pins
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
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
	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);

    // Initialize CS pin
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief   Transmits data to through SPI.
 *          With the way the LTC6811 communication works, the LTC6811 will not send
 *          anything during a transmit for uC to LTC6811. This is unlike what
 *          the SPI protocol expects where a transmit and receive happen
 *          simultaneously.
 * @note    Blocking statement
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI1_Write(uint8_t *txBuf, uint32_t txLen) {
    for(uint32_t i = 0; i < txLen; i++){
		SPI1_WriteRead(txBuf[i]);
	}
}

/**
 * @brief   Gets the data from SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not be expecting anything from the uC.
 *          The SPI protocol requires the uC to transmit data in order to receive
 *          anything so the uC will send junk data.
 * @note    Blocking statement
 * @param   rxBuf   data array to store the data that is received.
 * @param   rxLen   length of data array.
 * @return  None
 */
void BSP_SPI1_Read(uint8_t *rxBuf, uint32_t rxLen) {
    for(uint32_t i = 0; i < rxLen; i++){
		rxBuf[i] = SPI1_WriteRead(0x00);
	}
}

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the LTC6811 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the LTC6811
 *          go to standby.
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI1_SetStateCS(uint8_t state) {
    // PB6 is the Chip Select pin for the LTC6811
    if(state) {
        GPIO_SetBits(GPIOB, GPIO_Pin_6);
    } else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    }
}
