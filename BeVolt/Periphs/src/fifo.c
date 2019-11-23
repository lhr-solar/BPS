#include <stdint.h>
#include <stdbool.h>
#include "config.h"


#define size 32
#define carriage_return 0x0d

typedef struct Fifo {
	char queue[size];
	uint8_t head;
	uint8_t tail;
	bool hasCommand;//use when fifo is storing commands received over uart
	}Fifo;


//set up empty fifo
void fifoInit(Fifo *fifo){
	fifo->head = 0;
	fifo->tail = 0;
	fifo->hasCommand = false;
}

bool fifoIsEmpty(Fifo fifo){
	return fifo.head == fifo.tail;
}

bool fifoIsFull(Fifo fifo){
	return (((fifo.head + 1) % size) == fifo.tail);
}

//put a character into the fifo
void fifoPut(Fifo *fifo, char newChar){
	if(fifoIsFull(*fifo) == false){
		fifo->queue[fifo->head] = newChar;
		fifo->head = ((fifo->head + 1) % size);
		
		//for if fifo is used to store commands given by uart
		if (newChar == carriage_return){
			fifo->hasCommand = true;
		}
	}
}

//return a character from fifo
//return DANGER if empty
char fifoGet(Fifo *fifo){
	if(fifoIsEmpty(*fifo) == false){
		char temp = fifo->queue[fifo->tail];
		fifo->tail = ((fifo->tail + 1) % size);
		return temp;
	}
	return DANGER;
}


//for using fifo to store commands received from UART
//fails if multiple commands are stored in queue
//preconditions: fifo has a command stored in it, buffer is large enough to hold command
//parameters: fifo with command stored in it, buffer to receive stored command
//fills buffer with received command
void getCommand(Fifo *fifo, char buffer[]){
	if (fifo->hasCommand){
		int i = 0;
		while(fifo->queue[fifo->tail] != carriage_return){
			buffer[i] = fifoGet(fifo);
			i++;
		}
		fifoGet(fifo);//get rid of carriage return
		fifo->hasCommand = false;
	}
}

