/** UART.h
 * UART driver. Mainly for communication with PuTTY.
 */

/***** Unlike SPI, UART cannot have multiple modules connected to the same tx/rx line *****/

/** Pins:
 * UART1:
 *		tx : PA9
 *		rx : PA10
 * UART3:
 *		tx : PB10
 *		rx : PC5
 */

#include "UART.h"

Fifo RxFifo;//fifo to store data from uart interrupt handler

// This is required to use printf
struct __FILE{
	int file;
};

// Global variables
//static uint32_t TxErrorCnt = 0;
//static uint32_t RxErrorCnt = 0;
FILE __stdout;
FILE __stdin;

// Private Function Prototypes
void copySoftwareToHardware(void);
void copyHardwareToSoftware(void);

// Public Function Definitions
/** UART1_Init
 * Initializes UART1 Module
 * Pins:
 *		tx : PA9
 *		rx : PA10
 * @param baud rate: 9600, 115200, 500000
 */

void UART1_Init(uint32_t baud){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	//NVIC_InitTypeDef NVIC_InitStruct;

	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// Initialize PA9 and PA10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	// Initialize USART1
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
}

/** UART1_Write
 * Sends data through tx pin for UART1
 * @param ptr to char buffer, size of buffer
 */
void UART1_Write(char *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		while((USART1->SR&USART_SR_TC) == 0);	// Wait until transmission is done
		USART1->DR = txBuf[i] & 0xFF;
	}
}

/** UART1_Read
 * Sends data through tx pin for UART1
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART1_Read(char *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		while((USART1->SR&USART_SR_RXNE) == 0);
		rxBuf[i] = USART1->DR & 0xFF;
	}
}

/** UART3_Init
 * Initializes UART1 Module
 * Pins:
 *		tx : PB10
 *		rx : PC5
 * @param baud rate: 9600, 115200, 500000
 */
void UART3_Init(uint32_t baud){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;

	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	// Initialize PB10 and PC5
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_USART3);

	// Initialize USART3
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStruct);
	USART_Cmd(USART3, ENABLE);
}

/** UART3_Write
 * Sends data through tx pin for UART3
 * @param ptr to char buffer, size of buffer
 */
void UART3_Write(char *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		while((USART3->SR&USART_SR_TC) == 0);	// Wait until transmission is done
		USART3->DR = txBuf[i] & 0xFF;
	}
}

/** UART3_Read
 * Reads data from rx pin for UART3
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART3_Read(char *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		while((USART3->SR&USART_SR_RXNE) == 0);
		rxBuf[i] = USART3->DR & 0xFF;
	}
}

void UART1_OutChar(char data){
	while((USART1->SR&USART_SR_TC) == 0);	// Wait until transmission is done
	USART1->DR = data & 0xFF;
}

void UART3_OutChar(char data){
	while((USART3->SR&USART_SR_TC) == 0);	// Wait until transmission is done
	USART3->DR = data & 0xFF;
}

// this is used for printf to output to the usb uart
int fputc(int ch, FILE *f){
	//UART3_Write((char *)&ch, 1);
  UART1_Write((char *)&ch, 1);
	return 1;
}

int fgetc(FILE *f){
	char letter;
	//UART1_Read(&letter, 1);
	UART1_Read(&letter, 1);
	return (int)letter;
}

/**
* @brief Configures the USART1 Peripheral for interrupts.
* @param None
* @retval None
*/
void USART1_Config(void){
 GPIO_InitTypeDef GPIO_InitStructure;
 USART_InitTypeDef USART_InitStructure;
 /* USART IOs configuration ***********************************/
 /* Enable GPIOA clock */
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 /* Connect PA9 to USART1_Tx */
 GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
 /* Connect PA10 to USART1_Rx*/
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
     /* Configure USART1_Tx and USART1_Rx as alternate function */
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_Init(GPIOA, &GPIO_InitStructure);
     /* USART configuration ***************************************/
     /* USART1 configured as follow:
     - BaudRate = 115200 baud
     - Word Length = 8 Bits
     - One Stop Bit
     - No parity
     - Hardware flow control disabled (RTS and CTS signals)
     - Receive and transmit enabled
     */
     /* Enable USART1 clock */
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

     USART_InitStructure.USART_BaudRate = 115200;
     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
     USART_InitStructure.USART_StopBits = USART_StopBits_1;
     USART_InitStructure.USART_Parity = USART_Parity_No;
     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
     USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
     USART_Init(USART1, &USART_InitStructure);
     /* Enable USART1 */
     USART_Cmd(USART1, ENABLE);

     /* Enable USART1 Receive interrupt */
     USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		 
		 NVIC_InitTypeDef NVIC_InitStructure;
		 /* Enable USART1 IRQ channel in the NVIC controller.
      When the USART1 interrupt is generated (in this example when
      data is received) the USART1_IRQHandler will be served */
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
			
			//initialize RxFifo
			RxFifo.head = 0;
			RxFifo.tail = 0;
}

/**
* @brief This function handles USART1 global interrupt request.
* @param None
* @retval None
*/
void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
	/* Read one byte from the receive data register */
		fifoPut(&RxFifo, USART_ReceiveData(USART1));
  }
}

//use when fifo is storing commands received over uart
//returns if fifo has a command in it by checking for carriage return
bool hasCommand(Fifo *fifo){
	for (int i = fifo->tail; i != fifo->head; i = (i + 1) % fifo_size){
		if (fifo->queue[i] == 0x0d)
			return true;
	}
	return false;
}
//for using fifo to store commands received from UART
//fails if multiple commands are stored in queue
//preconditions: fifo has a command stored in it, buffer is large enough to hold command
//parameters: fifo with command stored in it, buffer to receive stored command
//fills buffer with received command
void getCommand(Fifo *fifo, char buffer[]){
	if (hasCommand(fifo)){
		int i = 0;
		while(fifo->queue[fifo->tail] != '\r'){
			buffer[i] = fifoGet(fifo);
			i++;
		}
		fifoGet(fifo);//get rid of carriage return
		buffer[i] = 0;
	}
}

//checks if new data has been received from UART interrupts
//if so, it echoes the new character to Putty and adds the character to the passed fifo
//backspace is supported
void checkUARTandEcho(Fifo *uartFifo) {
		if (!fifoIsFull(*uartFifo) && !fifoIsEmpty(RxFifo)){//put new characters into fifo
			//throw out bad characters
			if ((RxFifo.queue[RxFifo.tail] != '\r') && ((RxFifo.queue[RxFifo.tail]  < 0x20) || (RxFifo.queue[RxFifo.tail]  > 0x7f))){
				fifoGet(&RxFifo);
				return;
			}
			//only let enter or backspace get into last valid spot in fifo so it does not freeze when it fills up
			if (((uartFifo->head + 2) % fifo_size == uartFifo->tail) && ((RxFifo.queue[RxFifo.tail] != '\r') && (RxFifo.queue[RxFifo.tail] != 0x7f))){
				fifoGet(&RxFifo);
				return;
			}
			if (RxFifo.queue[RxFifo.tail] != 0x7f)
				fifoPut(uartFifo, (char) RxFifo.queue[RxFifo.tail]);
			printf("%c", RxFifo.queue[RxFifo.tail]);
			if (RxFifo.queue[RxFifo.tail] == 0x7f){
				removeLastElement(uartFifo);
			}
			fifoGet(&RxFifo);
		}
	}
