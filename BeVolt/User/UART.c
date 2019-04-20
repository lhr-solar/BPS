/** UART.h
 * UART driver. Mainly for communication with PuTTY.
 * @authors Sijin Woo
 * @lastRevised 10/2/2018
 */

/***** Unlike SPI, UART cannot have multiple modules connected to the same tx/rx line *****/

/** Pins:
 * UART1:
 *		tx : PA9
 *		rx : PA10
 * UART3:		*** NOTE: Nucleo Board uses these pins for USB ***
 *		tx : PD8
 *		rx : PD9
 */
 
#include "UART.h"

#define FIFO_SIZE		32
#define FIFO_SUCCESS	1
#define FIFO_FAIL		0

// This is required to use printf
struct __FILE{
	int file;
};

// Global variables
static bool NewCommandReceivedFlag = false;
static uint32_t TxErrorCnt = 0;
static uint32_t RxErrorCnt = 0;
FILE __stdout;
FILE __stdin;

AddIndexFifo(Rx, FIFO_SIZE, char, FIFO_SUCCESS, FIFO_FAIL)
AddIndexFifo(Tx, FIFO_SUCCESS, char, FIFO_SUCCESS, FIFO_FAIL)

// Private Function Prototypes
void copySoftwareToHardware(void);
void copyHardwareToSoftware(void);
void UART1_OutChar(char data);
void UART3_OutChar(char data);

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
	NVIC_InitTypeDef NVIC_InitStruct;
	
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
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	
	// Set NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = UART1_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	RxFifo_Init();
	TxFifo_Init();
}

/** UART1_Write
 * Sends data through tx pin for UART1
 * @param ptr to char buffer, size of buffer
 */
void UART1_Write(char *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		UART1_OutChar(*txBuf);
		txBuf++;
	}
	
	// Enable interrupt since Fifo now has contents
	USART_ClearITPendingBit(USART1, USART_IT_TXE);
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/** UART1_Read
 * Reads data from rx pin for UART1
 * The data received will be stored in rxBuf
 * Make sure your buffer size is enough to get whole string.
 * @param ptr to char buffer, size of buffer
 * @param 1 if successfully got new string, 0 if not
 */
bool UART1_Read(char *rxBuf, uint32_t rxSize){
	if(NewCommandReceivedFlag){
		for(uint32_t i = 0; i < rxSize && RxFifo_Size() > 0; i++){
			RxFifo_Get(&rxBuf[i]);
		}
		return true;
	}
	return false;
}

/**************** WARNING ***************/
// ONLY USE THIS FOR DEBUGGING ON NUCLEO!!!!!
/** UART3_Init
 * Initializes UART1 Module
 * Pins: 
 *		tx : PD8
 *		rx : PD9
 * @param baud rate: 9600, 115200, 500000
 */
void UART3_Init(uint32_t baud){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	// Initialize clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	// Initialize PD8 and PD9
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	
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



// Private Function Definitions
/** copySoftwareToHardware
 * Places contents of tx software buffer to hardware FIFO
 */
void copySoftwareToHardware(void){
	char letter;
	if(TxFifo_Size() == 0){
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		TxErrorCnt++;
	}else{
		TxFifo_Get(&letter);
		USART1->DR = letter;
	}
}

/** copyHardwareToSoftware
 * Places contents of rx hardware FIFO to software buffer
 */
void copyHardwareToSoftware(void){
	uint32_t data = USART1->DR;
	if(RxFifo_Size() >= (FIFO_SIZE - 1)){
		volatile uint32_t junk = data;
		RxErrorCnt++;
	}else{
		RxFifo_Put(data & 0xFF);
		if(data == '\n' || data == '\r'){
			NewCommandReceivedFlag = true;
		}
	}
	UART1_OutChar(data);	// echo
}

void UART1_OutChar(char data){
	while(TxFifo_Put(data) == FIFO_FAIL);
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	copySoftwareToHardware();
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

void UART3_OutChar(char data){
	while((USART3->SR&USART_SR_TC) == 0);	// Wait until transmission is done
	USART3->DR = data & 0xFF;
}

// this is used for printf to output to the usb uart
int fputc(int ch, FILE *f){
	if(!NUCLEO){
		UART1_OutChar(ch);
	}else{
		UART3_Write((char *)&ch, 1);
	}
  return 1;
}

int fgetc(FILE *f){
	char letter;
	if(!NUCLEO){
		UART1_Read(&letter, 1);
	}else{
		UART3_Read(&letter, 1);
	}
	return letter;
}

/***************************** Interrupt Service Routines ******************************/
void USART1_IRQHandler(void){
	// Transmission Complete ISR
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_TXE);	// acknowledge
		copySoftwareToHardware();
		if(TxFifo_Size() == 0){
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}
	}
	// Rx Fifo Not Empty ISR
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	// acknowledge
		copyHardwareToSoftware();
	}
	// Overflow error ISR
	if(USART_GetITStatus(USART1, USART_FLAG_ORE) != RESET){
		//LED_GreenOn();
	}
}
