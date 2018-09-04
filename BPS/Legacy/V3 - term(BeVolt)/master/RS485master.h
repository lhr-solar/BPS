#ifndef RS485MASTER_H
#define RS485MASTER_H

void RS485_sendPacket(unsigned char waitResp);
void RS485_initialize(void);
unsigned char RS485_ready(void);				//returns true if the 485 is ready to send out a command


//global utility functions (these are used by the RS422 as well)
unsigned int  RS485_combineBytes(unsigned char MSB, unsigned char LSB);
unsigned char RS485_splitMSB(unsigned int input);
unsigned char RS485_splitLSB(unsigned int input);


#endif
