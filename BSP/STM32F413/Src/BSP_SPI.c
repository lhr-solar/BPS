/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_SPI.h"
#include "stm32f4xx.h"
#include "os.h"
#include "BSP_OS.h"

// These are the sizes of the fifos.
// You can write/read more than this at once,
// but performance will degrade slightly.
#define TX_SIZE 128
#define RX_SIZE 64

#define FIFO_TYPE uint8_t
#define FIFO_SIZE TX_SIZE
#define FIFO_NAME txfifo
#include "fifo.h"

#define FIFO_TYPE uint8_t
#define FIFO_SIZE RX_SIZE
#define FIFO_NAME rxfifo
#include "fifo.h"

/*************************************************
 *                 ==Important==				 *
 * 												 *
 * If adding new SPI busses, all you should have *
 * to modify are the spi_port_t enum (declared   *
 * in the header file), these arrays, and the    *
 * initialization code.                          *
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
	GPIOD,	// LTC6811
	GPIOA	// AS8510
};

static const uint16_t SPI_SELECT_PINS[NUM_SPI_BUSSES] = {
    GPIO_Pin_2,
    GPIO_Pin_15
};

static txfifo_t spiTxFifos[NUM_SPI_BUSSES];
static rxfifo_t spiRxFifos[NUM_SPI_BUSSES];

static bsp_os_t *SPI_os[NUM_SPI_BUSSES];

// Use this inline function to wait until SPI communication is complete
static inline void SPI_Wait(SPI_TypeDef *SPIx){
	while(((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPIx)->SR & SPI_SR_BSY));
}

// Use this inline function to wait until SPI communication is complete
static inline void SPI_WaitTx(SPI_TypeDef *SPIx){
#ifdef BAREMETAL
	SPI_Wait(SPIx);
#endif

#ifdef RTOS
	SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
	if(SPIx == SPI1){
		SPI_os[spi_ltc6811]->pend();
	}
	else if(SPIx == SPI3){
		SPI_os[spi_as8510]->pend();
	}
#endif
}

/** SPI_WriteRead
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
 * @param   spi_os pointer to struct that holds the SPI specific pend()/post() functions 
 * @return  None
 */
void BSP_SPI_Init(spi_port_t port, bsp_os_t *spi_os){
	//      SPI configuration:
    //          speed : 125kbps
    //          CPOL : 1 (polarity of clock during idle is high)
    //          CPHA : 1 (tx recorded during 2nd edge)
    // Pins:
    //      SPI1:
    //          PB3 : SCK
    //          PB4 : MISO
    //          PB5 : MOSI 
    //          PD2 : CS

    GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	for(int i = 0; i < NUM_SPI_BUSSES; i++) {
		spiTxFifos[i] = txfifo_new();
		spiRxFifos[i] = rxfifo_new();
	}

	// I don't think there's any way around hardcoding this one

    if(port == spi_ltc6811) {
		//      SPI configuration:
		//          speed : 
		//          CPOL : 1 (polarity of clock during idle is high)
		//          CPHA : 1 (tx recorded during 2nd edge)
		// Pins:
		//      SPI1:
		//          PB3 : SCK
		//          PB4 : MISO
		//          PB5 : MOSI 
		//          PD2 : CS
		
		// Initialize clocks
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
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
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
		SPI_InitStruct.SPI_CRCPolynomial = 0;	
		SPI_Init(SPI1, &SPI_InitStruct);
		SPI_Cmd(SPI1, ENABLE);

		// Initialize CS pin
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_Init(GPIOD, &GPIO_InitStruct);
		SPI_os[spi_ltc6811] = spi_os;

		#ifdef RTOS
		//Configure SPI1 interrupt priority
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel = SPI1_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		#endif

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
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
		
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
		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
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
		SPI_os[spi_as8510] = spi_os;

		#ifdef RTOS
		//Configure SPI3 interrupt priority
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel = SPI3_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStruct);
		#endif
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
	// If we're below an experimentally-determined value, just use polling
	if(txLen < 8) { 
		for(int i = 0; i < txLen; i++) {
			SPI_WriteRead(port, txBuf[i]);
		}
	} else {
		// If we have a lot of data, we use interrupts to mitigate it
		// Fill as much of the fifo as possible
		size_t i = 0;
		while(i < txLen) {
			// Put as much data into the fifo as can fit
			while(i < txLen && txfifo_put(&spiTxFifos[port], txBuf[i])) {
				i++;
			}

			// Wait for the transmission to complete
			SPI_WaitTx(SPI_BUSSES[port]);
		}
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
	// bool first = true;

	// If we have only a little amount of data, just use polling
	if(rxLen < 8) {
		for(int i = 0; i < rxLen; i++) {
			rxBuf[i] = SPI_WriteRead(port, 0x00);
		}
	} else {
		SPI_I2S_ITConfig(SPI_BUSSES[port], SPI_I2S_IT_RXNE, ENABLE);
		// Fill the fifo with zeros to read
		size_t i = 0, r = 0;
		// Empty the fifo
		rxfifo_renew(&spiRxFifos[port]);
		// Read the data
		while(i < rxLen) {
			// Keep filling the fifo with data until we have read everything
			while(i < rxLen && txfifo_put(&spiTxFifos[port], 0)) {
				i++;
			}

			// Wait for the transmission to complete
			SPI_WaitTx(SPI_BUSSES[port]);
			// Busy wait the last bit, just to ensure all bytes have been received

			SPI_Wait(SPI_BUSSES[spi_ltc6811]);

			// Copy the data out of the fifo
			while(r < i && rxfifo_get(&spiRxFifos[port], &rxBuf[r])) {
				r++;
			}
		}
		SPI_I2S_ITConfig(SPI_BUSSES[port], SPI_I2S_IT_RXNE, DISABLE);
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

/***************THE FOLLWING CODE IS FOR THE RTOS VERSION OF THE BPS*********/
#ifdef RTOS

void SPI1_IRQHandler(void){
	// Save the CPU registers
	CPU_SR_ALLOC();

	// Protect a critical section
	CPU_CRITICAL_ENTER();

	// make the kernel aware that the interrupt has started
	OSIntEnter();
	CPU_CRITICAL_EXIT();

	// Handle the interrupts
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET){
		// Check to see if there is any data awaiting transmission
		if(!txfifo_get(&spiTxFifos[spi_ltc6811], (uint8_t*)&SPI1->DR)) {
			// We are out of data, so turn off the interrupt and post the semaphore
			SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, DISABLE);
			SPI_os[spi_ltc6811]->post();
		}
	}
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET){
		// Get the incoming data, put it in the fifo
		// If this overflows, it's the user's fault.
		rxfifo_put(&spiRxFifos[spi_ltc6811], SPI1->DR);
	}

	//make the kernel aware that the interrupt has ended
	OSIntExit();
}

void SPI3_IRQHandler(){
	// Save the CPU registers
	CPU_SR_ALLOC();

	// Protect a critical section
	CPU_CRITICAL_ENTER();

	// make the kernel aware that the interrupt has started
	OSIntEnter();
	CPU_CRITICAL_EXIT();

	
	// Handle the interrupts
	if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_TXE) == SET){
		// Check to see if there is any data awaiting transmission
		if(!txfifo_get(&spiTxFifos[spi_as8510], (uint8_t*)&SPI3->DR)) {
			// We are out of data, so turn off the interrupt and post the semaphore
			SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, DISABLE);
			SPI_os[spi_as8510]->post();
		}
	}
	if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET){
		// Get the incoming data, put it in the fifo
		// If this overflows, it's the user's fault.
		rxfifo_put(&spiRxFifos[spi_as8510], SPI3->DR);
	}
	
	//make the kernel aware that the interrupt has ended
	OSIntExit();
}




#endif
