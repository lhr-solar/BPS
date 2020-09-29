/* Copyright (c) 2020 UT Longhorn Racing Solar */

#include "BSP_UART.h"
#include <stdint.h>
#include <pthread.h>

#define RX_SIZE     64

static char rxBuffer[RX_SIZE];
static uint32_t rxPut = 0;
static uint32_t rxGet = 0;
static bool lineReceived = false;

static bool RxFifo_Get(uint8_t *data);
static bool RxFifo_Put(uint8_t data);
static bool RxFifo_Peek(uint8_t *data);
static bool RxFifo_IsFull(void);
static bool RxFifo_IsEmpty(void);

static pthread_mutex_t rx_mutex = PTHREAD_MUTEX_INITIALIZER;

void *ScanThread(void *arg);

/**
 * @brief   Initializes the UART peripheral
 */
void BSP_UART_Init(void) {
    int err = 0;
    pthread_attr_t attr;
    pthread_t id;

    // Configure stdin to not use a buffer. May not be needed.
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    // Get thread attributes to create a thread
    err = pthread_attr_init(&attr);
    if(err != 0) {
        perror("pthread_attr_init");
        exit(EXIT_FAILURE);
    }

    err = pthread_create(&id, &attr, &ScanThread, NULL);
    if(err != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief   Gets one line of ASCII text that was received.
 * @pre     str should be at least 128bytes long.
 * @param   str : pointer to buffer to store the string. This buffer should be initialized
 *                  before hand.
 * @return  number of bytes that was read
 */
uint32_t BSP_UART_ReadLine(char *str) {
    if(lineReceived) {
        pthread_mutex_lock(&rx_mutex);
        uint8_t data = 0;
        uint32_t recvd = 0;
        RxFifo_Peek(&data);
        while(!RxFifo_IsEmpty() && data != '\r' && data != '\n') {
            recvd += RxFifo_Get((uint8_t *)str++);
            RxFifo_Peek(&data);
        }

        RxFifo_Get(&data);

        *str = 0;

        lineReceived = false;

        pthread_mutex_unlock(&rx_mutex);
        return recvd;
    }

    return 0;
}

/**
 * @brief   Transmits data to through UART line
 * @param   str : pointer to buffer with data to send.
 * @param   len : size of buffer
 * @return  number of bytes that were sent
 */
uint32_t BSP_UART_Write(char *str, uint32_t len) {
    return printf("%.*s", len, str);
}


void *ScanThread(void *arg) {

    do {
        int data = fgetc(stdin);

        if(data == '\r' || data == '\n') {
            lineReceived = true;
        }

        // Check if it was a backspace.
        // '\b' for minicmom
        // '\177' for putty
        if(data != '\b' && data != '\177' && data != 0) {

            pthread_mutex_lock(&rx_mutex);

            // Sweet, just a "regular" key. Put it into the fifo
            // Doesn't matter if it fails. If it fails, then the data gets thrown away
            // and the easiest solution for this is to increase RX_SIZE
            RxFifo_Put((uint8_t)data);

            pthread_mutex_unlock(&rx_mutex);
        }

    } while (1);

    // Should not get here
    int retval = 1;
    pthread_exit(&retval);
}

static bool RxFifo_Get(uint8_t *data) {
    if(!RxFifo_IsEmpty()) {
        *data = rxBuffer[rxGet];
        rxGet = (rxGet + 1) % RX_SIZE;
        return true;
    }
    return false;
}

static bool RxFifo_Put(uint8_t data) {
    if(!RxFifo_IsFull()) {
        rxBuffer[rxPut] = data;
        rxPut = (rxPut + 1) % RX_SIZE;
        return true;
    }
    return false;
}

static bool RxFifo_Peek(uint8_t *data) {
    if(!RxFifo_IsEmpty()) {
        *data = rxBuffer[rxGet];
        return true;
    }
    return false;
}

static bool RxFifo_IsFull(void) {
    return (rxPut + 1) % RX_SIZE == rxGet;
}

static bool RxFifo_IsEmpty(void) {
    return rxGet == rxPut;
}
