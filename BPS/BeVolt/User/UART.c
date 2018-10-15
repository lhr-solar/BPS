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
 
#include <stdint.h>
#include "stm32f4xx.h"
#include "FIFO.h"
#include "Definition.h"

// Global variables
static uint32_t TxErrorCnt = 0;
static uint32_t RxErrorCnt = 0;

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
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART3);
	
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
	USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	
	// Set NVIC
	NVIC_InitStruct.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = UART1_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

/** UART1_Write
 * Sends data through tx pin for UART1
 * @param ptr to char buffer, size of buffer
 */
void UART1_Write(char *txBuf, uint32_t txSize){
	for(uint32_t i = 0; i < txSize; i++){
		if(FIFO_IsFull_Tx() != 0){
			TxErrorCnt++;
		}else{
			FIFO_Put_Tx(txBuf[i]);
		}			
	}
	
	// Enable interrupt since Fifo now has contents
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
}

/** UART1_Read
 * Sends data through tx pin for UART1
 * The data received will be stored in rxBuf
 * @param ptr to char buffer, size of buffer
 */
void UART1_Read(char *rxBuf, uint32_t rxSize){
	for(uint32_t i = 0; i < rxSize; i++){
		
	}
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
	
	FIFO_Init();
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
	if(FIFO_IsEmpty_Tx()){
		USART_ClearITPendingBit(USART1, USART_IT_TC);
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		TxErrorCnt++;
	}else{
		USART1->DR = FIFO_Get_Tx();
	}
}

/** copyHardwareToSoftware
 * Places contents of rx hardware FIFO to software buffer
 */
void copyHardwareToSoftware(void){
	uint32_t data = USART1->DR;
	if(FIFO_IsFull_Rx()){
		volatile uint32_t junk = data;
		RxErrorCnt++;
	}else{
		FIFO_Put_Rx(data & 0xFF);
	}
}

/***************************** Interrupt Service Routines ******************************/
void USART1_IRQHandler(void){
	// Transmission Complete ISR
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET){
		copySoftwareToHardware();
	}
	// Rx Fifo Not Empty ISR
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		copyHardwareToSoftware();
	}
	// Overflow error ISR
	if(USART_GetITStatus(USART1, USART_FLAG_ORE) != RESET){
		//LED_GreenOn();
	}
}
