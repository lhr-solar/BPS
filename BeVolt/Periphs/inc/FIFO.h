// FIFO.h

#ifndef __FIFO_H__
#define __FIFO_H__


//fifo implementation by Clark Poon and Siobhan Madden
#include <stdint.h>
#include <stdbool.h>
#include "config.h"

//struct definition of fifo
#define fifo_size 64
typedef struct Fifo {
	char queue[fifo_size];
	int8_t head;
	int8_t tail;
	}Fifo;

//set up empty fifo
void fifoInit(Fifo *fifo);

//checks to see if fifo is empty	
bool fifoIsEmpty(Fifo fifo);
	
//checks to see if fifo is full
bool fifoIsFull(Fifo fifo);
	
//put a character into the fifo
void fifoPut(Fifo *fifo, char newChar);
	
//return a character from fifo
//return DANGER if empty
char fifoGet(Fifo *fifo);
	
//removes last element in fifo
void removeLastElement(Fifo *fifo);

#endif //  __FIFO_H__
