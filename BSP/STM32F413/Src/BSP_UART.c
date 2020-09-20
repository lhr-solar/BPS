#include "BSP_UART.h"
#include "stm32f4xx.h"

#define TX_SIZE     128
#define RX_SIZE     64

static char txBuffer2[TX_SIZE];
static uint32_t txPut2 = 0;
static uint32_t txGet2 = 0;

static char rxBuffer2[RX_SIZE];
static uint32_t rxPut2 = 0;
static uint32_t rxGet2 = 0;
static bool lineReceived2 = false;

static char txBuffer3[TX_SIZE];
static uint32_t txPut3 = 0;
static uint32_t txGet3 = 0;

static char rxBuffer3[RX_SIZE];
static uint32_t rxPut3 = 0;
static uint32_t rxGet3 = 0;
static bool lineReceived3 = false;

static bool TxFifo_Get(uint8_t *data, uint8_t usart);
static bool TxFifo_Put(uint8_t data, uint8_t usart);
static bool TxFifo_IsFull(uint8_t usart);
static bool TxFifo_IsEmpty(uint8_t usart);

static bool RxFifo_Get(uint8_t *data, uint8_t usart);
static bool RxFifo_Put(uint8_t data, uint8_t usart);
static bool RxFifo_RemoveLast(uint8_t *data, uint8_t usart);
static bool RxFifo_Peek(uint8_t *data, uint8_t usart);
static bool RxFifo_IsFull(uint8_t usart);
static bool RxFifo_IsEmpty(uint8_t usart);

/**
 * @brief   Initializes the UART peripheral
 */
void BSP_UART_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    USART_InitTypeDef UART_InitStruct = {0};
    //Enable USART3, PC5, PB10, USART2, PA2, PA3 clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //Initialize pins for USART
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    //Enable alternative function on all pins
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
    //Initialize UART2 and 3
    UART_InitStruct.USART_BaudRate = 115200;
    UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    UART_InitStruct.USART_Parity = USART_Parity_No;
    UART_InitStruct.USART_StopBits = USART_StopBits_1;
    UART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART3, &UART_InitStruct);
    USART_Init(USART2, &UART_InitStruct);
    //Enable interrupts on receiving and transmitting
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    //Enable UART2 and 3
    USART_Cmd(USART3, ENABLE);
    USART_Cmd(USART2, ENABLE);
    //Enable NVIC interrupts for USART2 and 3
    NVIC_InitTypeDef NVIC_InitStructure;
  	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 * @brief   Gets one line of ASCII text that was received.
 * @pre     str should be at least 128bytes long.
 * @param   str : pointer to buffer to store the string. This buffer should be initialized
 *                  before hand.
 * @param   usart : which usart to read from (2 or 3)
 * @return  number of bytes that was read
 */
uint32_t BSP_UART_ReadLine(char *str, uint8_t usart) {
    uint8_t data = 0;
    uint32_t recvd = 0;
    if(lineReceived3 && (usart == 3)) { //read from 3rd usart
        USART_ITConfig(USART3, USART_IT_RXNE, RESET);
        RxFifo_Peek(&data, 3);
        while(!RxFifo_IsEmpty(3) && data != '\r') {
            recvd += RxFifo_Get((uint8_t *)str++, 3);
            RxFifo_Peek(&data, 3);
        }
        RxFifo_Get(&data, 3);
        *str = 0;
        lineReceived3 = false;
        USART_ITConfig(USART3, USART_IT_RXNE, SET);
        return recvd;
    }
    if(lineReceived2 && (usart == 2)) { //read from usart 2
        USART_ITConfig(USART2, USART_IT_RXNE, RESET);
        RxFifo_Peek(&data, 2);
        while(!RxFifo_IsEmpty(2) && data != '\r') {
            recvd += RxFifo_Get((uint8_t *)str++, 2);
            RxFifo_Peek(&data, 2);
        }
        RxFifo_Get(&data, 2);
        *str = 0;
        lineReceived2 = false;
        USART_ITConfig(USART2, USART_IT_RXNE, SET);
        return recvd;
    }
    return 0;
}

/**
 * @brief   Transmits data to through UART line
 * @param   str : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @param   usart : which usart to read from (2 or 3)
 * @return  number of bytes that were sent
 */
uint32_t BSP_UART_Write(char *str, uint32_t len, uint8_t usart) {
    uint32_t sent = 0;
    if (usart == 3){
        USART_ITConfig(USART3, USART_IT_TC, RESET);
        while (*str != '\0' && len > 0){
            sent += TxFifo_Put(*str, 3);
            str++;
            len--;
        }
        USART_ITConfig(USART3, USART_IT_TC, SET);
        return sent;
    }
    if (usart == 2){
        USART_ITConfig(USART2, USART_IT_TC, RESET);
        while (*str != '\0' && len > 0){
            sent += TxFifo_Put(*str, 2);
            str++;
            len--;
        }
        USART_ITConfig(USART2, USART_IT_TC, SET);
        return sent;
    }
    return 0;
}

void USART2_IRQHandler(void) {
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        uint8_t data = USART2->DR;
        bool removeSuccess = 1;
        if(data == '\r') lineReceived2 = true;
        // Check if it was a backspace.
        // '\b' for minicmom
        // '\177' for putty
        if(data != '\b' && data != '\177') RxFifo_Put(data, 2);
        // Sweet, just a "regular" key. Put it into the fifo
        // Doesn't matter if it fails. If it fails, then the data gets thrown away
        // and the easiest solution for this is to increase RX_SIZE
        else {
            uint8_t junk = 0;
            // Delete the last entry!
            removeSuccess = RxFifo_RemoveLast(&junk, 2);
        }
        if(removeSuccess) USART2->DR = data;
    }
    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET) {
        // If getting data from fifo fails i.e. the tx fifo is empty, then turn off the TX interrupt
        if(!TxFifo_Get((uint8_t *)&(USART2->DR), 2)) USART_ITConfig(USART2, USART_IT_TC, RESET);
    }
    if(USART_GetITStatus(USART2, USART_IT_ORE) != RESET);
}

void USART3_IRQHandler(void) {
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        uint8_t data = USART3->DR;
        bool removeSuccess = 1;
        if(data == '\r') lineReceived3 = true;
        // Check if it was a backspace.
        // '\b' for minicmom
        // '\177' for putty
        if(data != '\b' && data != '\177') RxFifo_Put(data, 3);
        // Sweet, just a "regular" key. Put it into the fifo
        // Doesn't matter if it fails. If it fails, then the data gets thrown away
        // and the easiest solution for this is to increase RX_SIZE
        else {
            uint8_t junk = 0;
            // Delete the last entry!
            removeSuccess = RxFifo_RemoveLast(&junk, 3);
        }
        if(removeSuccess) USART3->DR = data;
    }
    if(USART_GetITStatus(USART3, USART_IT_TC) != RESET) {
        // If getting data from fifo fails i.e. the tx fifo is empty, then turn off the TX interrupt
        if(!TxFifo_Get((uint8_t *)&(USART3->DR), 3)) USART_ITConfig(USART3, USART_IT_TC, RESET);
    }
    if(USART_GetITStatus(USART3, USART_IT_ORE) != RESET);
}


static bool TxFifo_Get(uint8_t *data, uint8_t usart) {
    if(!TxFifo_IsEmpty(3) && (usart == 3)) {
        *data = txBuffer3[txGet3];
        txGet3 = (txGet3 + 1) % TX_SIZE;
        return true;
    }
    if(!TxFifo_IsEmpty(2) && usart == 2) {
        *data = txBuffer2[txGet2];
        txGet2 = (txGet2 + 1) % TX_SIZE;
        return true;
    }
    return false;
}

static bool TxFifo_Put(uint8_t data, uint8_t usart) {
    if(!TxFifo_IsFull(3) && (usart  == 3)) {
        txBuffer3[txPut3] = data;
        txPut3 = (txPut3 + 1) % TX_SIZE;
        return true;
    }
    if(!TxFifo_IsFull(2) && (usart == 2)) {
        txBuffer2[txPut2] = data;
        txPut2 = (txPut2 + 1) % TX_SIZE;
        return true;
    }
    return false;
}

static bool TxFifo_IsFull(uint8_t usart) {
    if (usart == 3) return (txPut3 + 1) % TX_SIZE == txGet3;
    if (usart == 2) return (txPut2 + 1) % TX_SIZE == txGet2;
    return 0;
}

static bool TxFifo_IsEmpty(uint8_t usart) {
    if (usart == 3) return txGet3 == txPut3;
    if (usart == 2) return txGet2 == txPut2;
    return 0;
}

static bool RxFifo_Get(uint8_t *data, uint8_t usart) {
    if(!RxFifo_IsEmpty(3) && (usart == 3)) {
        *data = rxBuffer3[rxGet3];
        rxGet3 = (rxGet3 + 1) % RX_SIZE;
        return true;
    }
    if(!RxFifo_IsEmpty(2) && (usart == 2)) {
        *data = rxBuffer2[rxGet2];
        rxGet2 = (rxGet2 + 1) % RX_SIZE;
        return true;
    }
    return false;
}

static bool RxFifo_Put(uint8_t data, uint8_t usart) {
    if(!RxFifo_IsFull(3) && (usart == 3)) {
        rxBuffer3[rxPut3] = data;
        rxPut3 = (rxPut3 + 1) % RX_SIZE;
        return true;
    }
    if(!RxFifo_IsFull(2) && (usart == 2)) {
        rxBuffer2[rxPut2] = data;
        rxPut2 = (rxPut2 + 1) % RX_SIZE;
        return true;
    }
    return false;
}

static bool RxFifo_RemoveLast(uint8_t *data, uint8_t usart) {
    if(!RxFifo_IsEmpty(3) && (usart == 3)) {
        *data = rxBuffer3[rxPut3 - 1];
        if(rxPut3 > 0) rxPut3--;
        else rxPut3 = RX_SIZE - 1;
        return true;
    }
    if(!RxFifo_IsEmpty(2) && (usart == 2)) {
        *data = rxBuffer2[rxPut2 - 1];
        if(rxPut2 > 0) rxPut2--;
        else rxPut2 = RX_SIZE - 1;
        return true;
    }
    return false;
}

static bool RxFifo_Peek(uint8_t *data, uint8_t usart) {
    if(!RxFifo_IsEmpty(3) && (usart == 3)) {
        *data = rxBuffer3[rxGet3];
        return true;
    }
    if(!RxFifo_IsEmpty(2) && (usart == 2)) {
        *data = rxBuffer2[rxGet2];
        return true;
    }
    return false;
}

static bool RxFifo_IsFull(uint8_t usart) {
    if (usart == 3) return (rxPut3 + 1) % RX_SIZE == rxGet3;
    if (usart == 2) return (rxPut2 + 1) % RX_SIZE == rxGet2;
    return 0;
}

static bool RxFifo_IsEmpty(uint8_t usart) {
    if (usart == 3) return rxGet3 == rxPut3;
    if (usart == 2) return rxGet2 == rxPut2;
    return 0;
}
