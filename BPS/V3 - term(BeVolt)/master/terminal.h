#ifndef TERMINAL_H
#define TERMINAL_H

void term_serviceWaitingCMD(void);
void term_executeCMD(void); //input is in RS422_termInBuff[255] and length (not counting CR/LF) is in RS422_termInPointer
							//is called from RS422-receive interrupt handler
void term_entered(void);
void term_sendDone(void);
void term_invalidEntry(void);

#endif
