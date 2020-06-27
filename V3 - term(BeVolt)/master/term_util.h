#ifndef TERM_UTIL_H
#define TERM_UTIL_H

void term_appendCurrent(signed int current);
void term_appendTemp(unsigned int temp);
void term_appendVolt(unsigned int volt);
void term_appendBSVolt(unsigned int volt);
void term_appendHexByte(unsigned char in);
void term_appendHexChar(unsigned char num);
int term_getNum(void);
void term_appendInt(signed int input);
void term_appendString(char *input);
void term_appendChar(char chr);
void term_appendNum(unsigned char num);
void term_appendUint(unsigned int input);
void term_appendULint(unsigned long input);
void term_appendUintZ(unsigned int input, unsigned char zeros);
void term_appendSint(signed int input);
void term_appendSintZ(signed int input, unsigned char zeros);



#endif
