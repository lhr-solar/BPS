#ifndef SERIAL_LIB_H
#define SERIAL_LIB_H

#include "constants.h"

#define BUFFER_SIZE 50

static Serial pc(SERIAL_TX, SERIAL_RX);
static char message[BUFFER_SIZE];
static char nextChar;

void getMessage();
void interruptFunction();

#endif 