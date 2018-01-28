#include "term_util.h"
#include "globals.h"
#include "terminal.h"


void term_appendHexByte(unsigned char in)
{
	unsigned char temp;
	temp = in&0xF0;
	temp = temp>>4;
	term_appendHexChar(temp);
	in = in&0x0F;
	term_appendHexChar(in);
}

//appends a single numeral to the terminal output buffer
void term_appendHexChar(unsigned char num)
{
	if(RS422_termOutLength == 255)
		return;
	if(num >=16)
		return;
	if(num>=10)				//letter
		RS422_termOutBuff[RS422_termOutLength] = num+55;
	else					//number
		RS422_termOutBuff[RS422_termOutLength] = num+48;
	RS422_termOutLength++;
}

void term_appendVolt(unsigned int volt)
{
	term_appendUint(volt/10000);
	term_appendChar('.');
	term_appendUintZ(volt%10000,4);
	term_appendChar('V');
	term_appendString(" \t\tdirect: ");
	term_appendUint(volt);
}

void term_appendBSVolt(unsigned int volt)
{
	term_appendUint(volt/1000);
	term_appendChar('.');
	term_appendUintZ(volt%1000,3);
	term_appendChar('V');
	term_appendString(" \t\tdirect: ");
	term_appendUint(volt);
}

void term_appendTemp(unsigned int temp)
{
	signed int cel;
	term_appendUint(temp/100);
	term_appendChar('.');
	term_appendUintZ(temp%100,2);
	term_appendChar(248);
	term_appendString("K = ");
	cel = temp-27315; 			//TODO:problems with only slightly negative temps?
	if(cel >= 0)
	{
		term_appendChar('+');
		term_appendUint(cel/100);
	}
	else
	{
		term_appendChar('-');
		term_appendUint(0-(cel/100));
	}
	term_appendChar('.');
	cel = cel%100;
	if(cel < 0)
		cel = 0-cel;
	term_appendUintZ(cel,2);
	term_appendChar(248);
	term_appendChar('C');
	term_appendString(" \tdirect: ");
	term_appendUint(temp);
}

void term_appendCurrent(signed int current)
{
	int temp;
	if(current >= 0)
	{
		term_appendChar('+');
		term_appendUint(current/100);
	}
	else
	{
		term_appendChar('-');
		term_appendUint(0-(current/100));
	}
	term_appendChar('.');
	temp = current%100;
	if(temp < 0)
		temp = 0-temp;
	term_appendUintZ(temp,2);
	term_appendString(" Amps");
	term_appendString(" \t\tdirect: ");
	term_appendSint(current);
}

void term_appendInt(signed int input)
{
	if(input >= 0)
		term_appendUint(input);
	else
	{
		term_appendUint(input);
		term_appendString(" (unsigned) or ");
		term_appendSint(input);
		term_appendString(" (signed)");
	}
}

int term_getNum(void)
{
	unsigned char negative;
	unsigned char pointer;
	unsigned char valid;
	int temp;
	temp = 0;
	negative = 0;
	pointer = 0; 
	valid = 1;
	if(RS422_termInPointer == 0)
	{
		valid = 0;
		term_invalidEntry();
		return 0;
	}
	if(RS422_termInBuff[0] == '-')
	{
		negative = 1;
		pointer++;
	}
	else if(RS422_termInBuff[0] == '+')
	{
		negative = 0;
		pointer++;
	}
	while(pointer < RS422_termInPointer && valid)
	{
		temp = temp*10;
		if ((RS422_termInBuff[pointer] > 57) || (RS422_termInBuff[pointer] < 48))
		{
			valid = 0;
			break;
		}
		temp+=(RS422_termInBuff[pointer] - 48);
		pointer++;
	}
	if(negative)
		temp = 0-temp;
	if(valid)
		return temp;
	else
	{
		term_invalidEntry();
		return 0;
	}
}

//appends a single numeral to the terminal output buffer
void term_appendNum(unsigned char num)
{
	if(RS422_termOutLength == 255)
		return;
	if(num >=10)
		return;
	RS422_termOutBuff[RS422_termOutLength] = num+48;
	RS422_termOutLength++;
}

//appends a single ASCII charater to the terminal output buffer
void term_appendChar(char chr)
{
	if(RS422_termOutLength == 255)
		return;
	RS422_termOutBuff[RS422_termOutLength] = chr;
	RS422_termOutLength++;
}
//appends a string to the terminal output buffer
void term_appendString(char *input)
{
	unsigned int i = 0;
	unsigned int length = 0;
	while(input[length] && length < 255)
		length++;
	if(((unsigned int)length + (unsigned int)RS422_termOutLength) > 255)	//check if we overflow the buffer
		return;
	for(i = 0; i < length; i++)
	{
		RS422_termOutBuff[RS422_termOutLength] = input[i];
		RS422_termOutLength++;
	}
}

//appends an unsigned integer to the output buffer
void term_appendUint(unsigned int input)
{
	unsigned int temp;
	unsigned char started = 0;
	temp = input/10000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10000;
	}
	temp = input/1000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%1000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/100;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%100;
	}
	else if(started)
		term_appendNum(0);
	temp = input/10;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10;
	}
	else if(started)
		term_appendNum(0);
	term_appendNum(input);
}

//append a unsigned  long int to the output bufer
void term_appendULint(unsigned long input)
{
	long temp;
	unsigned char started;
	started = 0;
	temp = input/1000000000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%1000000000;
	}
	temp = input/100000000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%100000000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/1000000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10000000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/1000000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%1000000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/100000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%100000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/10000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/1000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%1000;
	}
	else if(started)
		term_appendNum(0);
	temp = input/100;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%100;
	}
	else if(started)
		term_appendNum(0);
	temp = input/10;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10;
	}
	else if(started)
		term_appendNum(0);
	term_appendNum(input);
}

//appends an unsigned integer to the output buffer with the specified number of preceeding zeros
void term_appendUintZ(unsigned int input,unsigned char zeros)
{
	unsigned int temp;
	unsigned char started = 0;
	temp = input/10000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10000;
	}
	else if(zeros > 4)
		term_appendNum(0);
	temp = input/1000;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%1000;
	}
	else if(started|| (zeros > 3))
		term_appendNum(0);
	temp = input/100;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%100;
	}
	else if(started|| (zeros > 2))
		term_appendNum(0);
	temp = input/10;
	if(temp)
	{
		started = 1;
		term_appendNum(temp);
		input = input%10;
	}
	else if(started || (zeros > 1))
		term_appendNum(0);
	term_appendNum(input);
}

//appends a signed integer to the output buffer
void term_appendSint(signed int input)
{
	if(input >= 0)
	{
		term_appendChar('+');
		term_appendUint(input);
	}
	else
	{
		term_appendChar('-');
		term_appendUint(0-input);
	}
}

//appends a signed integer to the output buffer, with the specified number of preceeding zeros
void term_appendSintZ(signed int input, unsigned char zeros)
{
	if(input >= 0)
	{
		term_appendChar('+');
		term_appendUintZ(input, zeros);
	}
	else
	{
		term_appendChar('-');
		term_appendUintZ(0-input, zeros);
	}
}
