#include "BSP_SPI.h"
#include "stm32f4xx.h"

// Use this macro function to wait until SPI communication is complete
#define SPI_Wait(SPIx)		while(((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY))

/*************************************************
 *                 ==Important==				 *
 * 												 *
 * If adding new SPI busses, all you should have *
 * to modify are the spi_port_t enum (declared   *
 * in the header file), these three arrays, and  *
 * the initialization code.                      *
 *************************************************/

// Lookup for the proper SPI ports
static SPI_TypeDef * const SPI_BUSSES[NUM_SPI_BUSSES] = {
	// These need to be in the same order as the enum 
	// spi_port_t as declared in BSP_SPI.h.
	SPI1,	// LTC6811
	SPI3 	// AS8510
};

static GPIO_TypeDef * const SPI_SELECT_PORTS[NUM_SPI_BUSSES] = {
	// Need to be in the same order as the enum spi_port_t
	GPIOB,	// LTC6811
	GPIOA	// AS8510
};

static const uint16_t SPI_SELECT_PINS[NUM_SPI_BUSSES] = {
    GPIO_Pin_6,
    GPIO_Pin_15
};

/** SPI1_WriteRead
 * @brief   Sends and receives a byte of data on the SPI line.
 * @param   txData single byte that will be sent to the device.
 * @return  rxData single byte that was read from the device.
 */
static uint8_t SPI_WriteRead(spi_port_t port, uint8_t txData){
    if(port >= NUM_SPI_BUSSES) return -1;

	SPI_TypeDef *bus = SPI_BUSSES[port];
	
	SPI_Wait(bus);
	bus->DR = txData & 0x00FF;
	SPI_Wait(bus);
	return bus->DR & 0x00FF;
}

/**
 * @brief   Initializes the SPI port.
 * @param   port The SPI port to initialize.
 * @return  None
 */
void BSP_SPI_Init(spi_port_t port) {
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// I don't think there's any way around hardcoding this one

    if(port == spi_ltc6811) {
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
	
	} else if(port == spi_as8510) {
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
}

/**
 * @brief   Transmits data to through SPI.
 * @note    Blocking statement
 * @param   port    the SPI port to write to
 * @param   txBuf   data array that contains the data to be sent.
 * @param   txLen   length of data array.
 * @return  None
 */
void BSP_SPI_Write(spi_port_t port, uint8_t *txBuf, uint32_t txLen) {
    for(uint32_t i = 0; i < txLen; i++){
		SPI_WriteRead(port, txBuf[i]);
	}
}

/**
 * @brief   Gets the data from SPI. With the way the LTC6811 communication works,
 *          the LTC6811 will not be expecting anything from the uC.
 *          The SPI protocol requires the uC to transmit data in order to receive
 *          anything so the uC will send junk data.
 * @note    Blocking statement
 * @param   port    the SPI port to read from
 * @param   rxBuf   data array to store the data that is received.
 * @param   rxLen   length of data array.
 * @return  None
 */
void BSP_SPI_Read(spi_port_t port, uint8_t *rxBuf, uint32_t rxLen) {
    for(uint32_t i = 0; i < rxLen; i++){
		rxBuf[i] = SPI_WriteRead(port, 0x00);
	}
}

/**
 * @brief   Sets the state of the chip select output pin.
 *          Set the state to low/0 to notify the LTC6811 that the data sent on the
 *          SPI lines are for it. Set the state to high/1 to make the LTC6811
 *          go to standby.
 * @param   port    the SPI port to select/deselct on
 * @param   state   0 for select, 1 to deselect
 * @return  None
 */
void BSP_SPI_SetStateCS(spi_port_t port, uint8_t state) {
    if(port >= NUM_SPI_BUSSES) return;

	if(state) {
		GPIO_SetBits(SPI_SELECT_PORTS[port], SPI_SELECT_PINS[port]);
	} else {
		GPIO_ResetBits(SPI_SELECT_PORTS[port], SPI_SELECT_PINS[port]);
	}
}
