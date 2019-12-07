#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "FIFO.h"

//set up empty fifo
void fifoInit(Fifo *fifo){
	fifo->head = 0;
	fifo->tail = 0;
}

bool fifoIsEmpty(Fifo fifo){
	return fifo.head == fifo.tail;
}

bool fifoIsFull(Fifo fifo){
	return (((fifo.head + 1) % fifo_size) == fifo.tail);
}

//put a character into the fifo
void fifoPut(Fifo *fifo, char newChar){
	if(fifoIsFull(*fifo) == false){
		fifo->queue[fifo->head] = newChar;
		fifo->head = ((fifo->head + 1) % fifo_size);
	}
}

//return a character from fifo
//return DANGER if empty
char fifoGet(Fifo *fifo){
	if(fifoIsEmpty(*fifo) == false){
		char temp = fifo->queue[fifo->tail];
		fifo->tail = ((fifo->tail + 1) % fifo_size);
		return temp;
	}
	return DANGER;
}

//removes last element in fifo
void removeLastElement(Fifo *fifo){
	if (fifoIsEmpty(*fifo))
		return;
	fifo->head = fifo->head - 1;
	if (fifo->head < 0){
		fifo->head = fifo_size - 1;
	}
}


