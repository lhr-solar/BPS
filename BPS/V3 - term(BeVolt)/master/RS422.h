#ifndef RS422_H
#define RS422_H

void RS422_initialize(void);
void RS422_sendPacket(void);
unsigned char RS422_ready(void);
void RS422_sendTerm(void);
void RS422_sendLCD(void);
unsigned char RS422_commandsWaiting(void);
void RS422_dequeCMD(unsigned char exclusive);


#endif
