/* Copyright (c) 2022 UT Longhorn Racing Solar */

#include "BSP_UART.h"
#include <stdint.h>
#include <pthread.h>
//Written by Sijin and Revised by Manthan Upadhyaya: 10/2020

#if 0
#define RX_SIZE     64

static char rxBuffer3[RX_SIZE];
static uint32_t rxPut3 = 0;
static uint32_t rxGet3 = 0;
static bool lineReceived3 = false;

static char rxBuffer2[RX_SIZE];
static uint32_t rxPut2 = 0;
static uint32_t rxGet2 = 0;
static bool lineReceived2 = false;

static bool RxFifo_Get(uint8_t *data, UART_Port usart);
static bool RxFifo_Put(uint8_t data, UART_Port usart);
static bool RxFifo_Peek(uint8_t *data, UART_Port usart);
static bool RxFifo_IsFull(UART_Port usart);
static bool RxFifo_IsEmpty(UART_Port usart);

static pthread_mutex_t rx_mutex3 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t rx_mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *ScanThread3(void *arg);
void *ScanThread2(void *arg);

#endif

/**
 * @brief   Initializes the UART peripheral
 */
void BSP_UART_Init(__attribute__((unused)) callback_t rxCallback,   // I can't be bother to figure out how to use these  
                   __attribute__((unused)) callback_t txCallback,   // callbacks in simulation, so they're just going to
                   __attribute__((unused)) UART_Port usart) {       // be unused for now.
#if 0
    int err = 0;
    pthread_attr_t attr;
    pthread_t id;
    // Configure stdin to not use a buffer. May not be needed.
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    err = pthread_mutex_init(&rx_mutex2, NULL);
    err = pthread_mutex_init(&rx_mutex3, NULL);
    if(err != 0) {
        perror("pthread_mutex_init");
        exit(EXIT_FAILURE);
    }

    // Get thread attributes to create a thread
    err = pthread_attr_init(&attr);
    if(err != 0) {
        perror("pthread_attr_init");
        exit(EXIT_FAILURE);
    }
    err = pthread_create(&id, &attr, &ScanThread3, NULL);
    if(err != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    err = pthread_attr_init(&attr);
    if(err != 0) {
        perror("pthread_attr_init");
        exit(EXIT_FAILURE);
    }
    err = pthread_create(&id, &attr, &ScanThread2, NULL);
    if(err != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
#endif
}

/**
 * @brief   Gets one line of ASCII text that was received.
 * @pre     str should be at least 128bytes long.
 * @param   str : pointer to buffer to store the string. This buffer should be initialized
 *                  before hand.
 * @param   usart : which usart to read from
 * @return  number of bytes that was read
 */
uint32_t BSP_UART_ReadLine(char *str, UART_Port usart) {
#if 0
    uint8_t data = 0;
    uint32_t recvd = 0;
    if(lineReceived3 && (usart == UART_USB)) {
        pthread_mutex_lock(&rx_mutex3);
        RxFifo_Peek(&data, UART_USB);
        while(!RxFifo_IsEmpty(UART_USB) && data != '\r' && data != '\n') {
            recvd += RxFifo_Get((uint8_t *)str++, UART_USB);
            RxFifo_Peek(&data, UART_USB);
        }
        RxFifo_Get(&data, UART_USB);
        *str = 0;
        lineReceived3 = false;
        pthread_mutex_unlock(&rx_mutex3);
        return recvd;
    }
    if(lineReceived2 && (usart == UART_BLE)) {
        pthread_mutex_lock(&rx_mutex2);
        RxFifo_Peek(&data, UART_BLE);
        while(!RxFifo_IsEmpty(UART_BLE) && data != '\r' && data != '\n') {
            recvd += RxFifo_Get((uint8_t *)str++, UART_BLE);
            RxFifo_Peek(&data, UART_BLE);
        }
        RxFifo_Get(&data, UART_BLE);
        *str = 0;
        lineReceived2 = false;
        pthread_mutex_unlock(&rx_mutex2);
        return recvd;
    }
#endif
    return 0;
}

/**
 * @brief   Transmits data to through UART line
 * @param   str : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @param   usart : which usart to write to
 * @return  number of bytes that were sent
 */
uint32_t BSP_UART_Write(char *str, uint32_t len, UART_Port usart) {
    return printf("%.*s", len, str);
}

#if 0
void *ScanThread3(void *arg) {
    do {
        int data = fgetc(stdin);
        if(data == '\r' || data == '\n') lineReceived3 = true;
        // Check if it was a backspace.
        // '\b' for minicmom
        // '\177' for putty
        if(data != '\b' && data != '\177' && data != 0) {
            pthread_mutex_lock(&rx_mutex3);
            // Sweet, just a "regular" key. Put it into the fifo
            // Doesn't matter if it fails. If it fails, then the data gets thrown away
            // and the easiest solution for this is to increase RX_SIZE
            RxFifo_Put((uint8_t)data, UART_USB);
            pthread_mutex_unlock(&rx_mutex3);
        }
    } while (1);
    // Should not get here
    int retval = 1;
    pthread_exit(&retval);
}

void *ScanThread2(void *arg) {
    do {
        int data = fgetc(stdin);
        if(data == '\r' || data == '\n') lineReceived2 = true;
        // Check if it was a backspace.
        // '\b' for minicmom
        // '\177' for putty
        if(data != '\b' && data != '\177' && data != 0) {
            pthread_mutex_lock(&rx_mutex2);
            // Sweet, just a "regular" key. Put it into the fifo
            // Doesn't matter if it fails. If it fails, then the data gets thrown away
            // and the easiest solution for this is to increase RX_SIZE
            RxFifo_Put((uint8_t)data, UART_BLE);
            pthread_mutex_unlock(&rx_mutex2);
        }

        pthread_yield();

    } while (1);
    // Should not get here
    int retval = 1;
    pthread_exit(&retval);
}

static bool RxFifo_Get(uint8_t *data, UART_Port usart) {
    if(!RxFifo_IsEmpty(UART_USB) && usart == UART_USB) {
        *data = rxBuffer3[rxGet3];
        rxGet3 = (rxGet3 + 1) % RX_SIZE;
        return true;
    }
    if(!RxFifo_IsEmpty(UART_BLE) && usart == UART_BLE) {
        *data = rxBuffer2[rxGet2];
        rxGet2 = (rxGet2 + 1) % RX_SIZE;
        return true;
    }
    return false;
}

static bool RxFifo_Put(uint8_t data, UART_Port usart) {
    if(!RxFifo_IsFull(UART_USB) && usart == UART_USB) {
        rxBuffer3[rxPut3] = data;
        rxPut3 = (rxPut3 + 1) % RX_SIZE;
        return true;
    }
    if(!RxFifo_IsFull(UART_BLE) && usart == UART_BLE) {
        rxBuffer2[rxPut2] = data;
        rxPut2 = (rxPut2 + 1) % RX_SIZE;
        return true;
    }
    return false;
}

static bool RxFifo_Peek(uint8_t *data, UART_Port usart) {
    if(!RxFifo_IsEmpty(UART_USB) && usart == UART_USB) {
        *data = rxBuffer3[rxGet3];
        return true;
    }
    if(!RxFifo_IsEmpty(UART_BLE) && usart == UART_BLE) {
        *data = rxBuffer2[rxGet2];
        return true;
    }
    return false;
}

static bool RxFifo_IsFull(UART_Port usart) {
    if (usart == UART_USB) return (rxPut3 + 1) % RX_SIZE == rxGet3;
    if (usart == UART_BLE) return (rxPut2 + 1) % RX_SIZE == rxGet2;

    // In event of an error, assume it is full
    return true;
}

static bool RxFifo_IsEmpty(UART_Port usart) {
    if (usart == UART_USB) return rxGet3 == rxPut3;
    if (usart == UART_BLE) return rxGet2 == rxPut2;

    // In event of an error, assume it is not empty
    return false;
}
#endif
