#ifndef LCDUTIL_H
#define LCDUTIL_H

//does the whole pack voltage (fixed width) (xxx.x)
void LCD_appendPackVolt(unsigned long volt);

//shows the cell voltage
//(fixed width) (x.xx)
void LCD_appendCellVolt(unsigned int volt);

//shows a cell temperature
//(fixed width) (xx.x)
void LCD_appendCellTemp(unsigned int temp);

void LCD_appendPackCurrent(signed int amps);

void LCD_appendSOC(unsigned char SOC);

//print a hex byte (two characters)
void LCD_appendHexByte(unsigned char in);

//appends a single numeral to the LCD output buffer
void LCD_appendHexChar(unsigned char num);

//display cell voltage
void LCD_appendVolt(unsigned int volt);

//display BootStrap Voltage
void LCD_appendBSVolt(unsigned int volt);

//display temperature
void LCD_appendTemp(unsigned int temp);

//display current
void LCD_appendCurrent(signed int current);	

//print an integer (will display both the high value (unsigned) and the negative value if both values are possible)
void LCD_appendInt(signed int input);

//appends a single numeral to the terminal output buffer
void LCD_appendNum(unsigned char num);

//appends a single ASCII charater to the terminal output buffer
void LCD_appendChar(char chr);

//appends a string to the terminal output buffer
void LCD_appendString(char *input);

//appends an unsigned integer to the output buffer
void LCD_appendUint(unsigned int input);

//append a unsigned  long int to the output bufer
void LCD_appendULint(unsigned long input);

//appends an unsigned integer to the output buffer with the specified number of preceeding zeros
void LCD_appendUintZ(unsigned int input,unsigned char zeros);

//appends a signed integer to the output buffer
void LCD_appendSint(signed int input);

//appends a signed integer to the output buffer, with the specified number of preceeding zeros
void LCD_appendSintZ(signed int input, unsigned char zeros);



#endif
