/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */

#include "common.h"
#include "sys/stat.h"
#include "BSP_UART.h"

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

int _write(int fd, char *buffer, unsigned int len) {
    if(buffer != NULL) {
        BSP_UART_Write(buffer, len, UART_USB);
    }

    return len;
}

int _read(int const fd, char *buffer, unsigned const len) {
    if(buffer != NULL) {
        
    }
    return 1;
}

int _close(int file)
{
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/* These functions are included to remove GCC warnings when compiling and are not necessary
// TODO: We should make sure redefining them does not interfere with RTOS
int _isatty(int fd) {
    return 0;
}

int _fstat(int fd, struct stat *statbuf) {
    return -1;
}
*/


/**
 * TODO: for print queue
 * 
 * Dynamic memory allocation of blocks: OSMemCreate(), OSMemGet(), OSMemPut()
 * 
 */

