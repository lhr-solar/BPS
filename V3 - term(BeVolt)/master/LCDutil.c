#include "LCDutil.h"
#include "globals.h"

//does the whole pack voltage (fixed width) (xxx.x)
void LCD_appendPackVolt(unsigned long volt)
{
	unsigned int tenths;
	LCD_appendUintZ(volt/10000,3);
	LCD_appendChar('.');
	tenths = volt%10000;
	tenths = tenths/1000;
	LCD_appendNum(tenths);
}

//shows the cell voltage
//(fixed width) (x.xx)
void LCD_appendCellVolt(unsigned int volt)
{
	unsigned int hundredth;
	LCD_appendNum(volt/10000);
	LCD_appendChar('.');
	hundredth = volt%10000;
	hundredth = hundredth/100;
	LCD_appendUintZ(hundredth,2);
}

//shows a cell temperature
//(fixed width) (xx.x)
void LCD_appendCellTemp(unsigned int temp)
{
	signed int cel;
	unsigned int tenths;
	cel = temp-27315;		//convert into celcious(sp?)
	if(cel >= 0)
	{
		LCD_appendUintZ(cel/100,2);
		tenths = cel%100;
		tenths = tenths/10;
		LCD_appendChar('.');
		LCD_appendNum(tenths);
	}
	else
	{
		LCD_appendChar('-');
		LCD_appendUintZ(0-(cel/100),3);
	}
}

void LCD_appendPackCurrent(signed int amps)
{
	unsigned int tenths;
	if(amps >= 0)
	{
		LCD_appendChar('+');
	}
	else
	{
		LCD_appendChar('-');
		amps = 0-amps;
	}
	LCD_appendUintZ(amps/100,3);
	LCD_appendChar('.');
	tenths = amps%100;
	tenths = tenths/10;
	LCD_appendNum(tenths);
}

void LCD_appendHexByte(unsigned char in)
{
	unsigned char temp;
	temp = in&0xF0;
	temp = temp>>4;
	LCD_appendHexChar(temp);
	in = in&0x0F;
	LCD_appendHexChar(in);
}

void LCD_appendSOC(unsigned char SOC)
{
	if(SOC >= 100)
		LCD_appendString("100");
	else
		LCD_appendUintZ(SOC,2);
}

//appends a single numeral to the terminal output buffer
void LCD_appendHexChar(unsigned char num)
{
	if(LCD_outLength == 255)
		return;
	if(num >=16)
		return;
	if(num>=10)				//letter
		LCD_buffer[LCD_outLength] = num+55;
	else					//number
		LCD_buffer[LCD_outLength] = num+48;
	LCD_outLength++;
}

void LCD_appendVolt(unsigned int volt)
{
	LCD_appendUint(volt/10000);
	LCD_appendChar('.');
	LCD_appendUintZ(volt%10000,4);
	LCD_appendChar('V');
	LCD_appendString(" \t\t\tdirect: ");
	LCD_appendUint(volt);
}

void LCD_appendBSVolt(unsigned int volt)
{
	LCD_appendUint(volt/1000);
	LCD_appendChar('.');
	LCD_appendUintZ(volt%1000,3);
	LCD_appendChar('V');
	LCD_appendString(" \t\tdirect: ");
	LCD_appendUint(volt);
}

void LCD_appendTemp(unsigned int temp)
{
	signed int cel;
	LCD_appendUint(temp/100);
	LCD_appendChar('.');
	LCD_appendUintZ(temp%100,2);
	LCD_appendChar(248);
	LCD_appendString("K = ");
	cel = temp-27315; 			//TODO:problems with only slightly negative temps?
	if(cel >= 0)
	{
		LCD_appendChar('+');
		LCD_appendUint(cel/100);
	}
	else
	{
		LCD_appendChar('-');
		LCD_appendUint(0-(cel/100));
	}
	LCD_appendChar('.');
	cel = cel%100;
	if(cel < 0)
		cel = 0-cel;
	LCD_appendUintZ(cel,2);
	LCD_appendChar(248);
	LCD_appendChar('C');
	LCD_appendString(" \tdirect: ");
	LCD_appendUint(temp);
}

void LCD_appendCurrent(signed int current)
{
	int temp;
	if(current >= 0)
	{
		LCD_appendChar('+');
		LCD_appendUint(current/100);
	}
	else
	{
		LCD_appendChar('-');
		LCD_appendUint(0-(current/100));
	}
	LCD_appendChar('.');
	temp = current%100;
	if(temp < 0)
		temp = 0-temp;
	LCD_appendUintZ(temp,2);
	LCD_appendString(" Amps");
	LCD_appendString(" \t\tdirect: ");
	LCD_appendSint(current);
}

void LCD_appendInt(signed int input)
{
	if(input >= 0)
		LCD_appendUint(input);
	else
	{
		LCD_appendUint(input);
		LCD_appendString(" (unsigned) or ");
		LCD_appendSint(input);
		LCD_appendString(" (signed)");
	}
}


//appends a single numeral to the terminal output buffer
void LCD_appendNum(unsigned char num)
{
	if(LCD_outLength == 255)
		return;
	if(num >=10)
		return;
	LCD_buffer[LCD_outLength] = num+48;
	LCD_outLength++;
}

//appends a single ASCII charater to the terminal output buffer
void LCD_appendChar(char chr)
{
	if(LCD_outLength == 255)
		return;
	LCD_buffer[LCD_outLength] = chr;
	LCD_outLength++;
}

//appends a string to the terminal output buffer
void LCD_appendString(char *input)
{
	unsigned int i = 0;
	unsigned int length = 0;
	while(input[length] && length < 255)
		length++;
	if(((unsigned int)length + (unsigned int)LCD_outLength) > 255)	//check if we overflow the buffer
		return;
	for(i = 0; i < length; i++)
	{
		LCD_buffer[LCD_outLength] = input[i];
		LCD_outLength++;
	}
}

//appends an unsigned integer to the output buffer
void LCD_appendUint(unsigned int input)
{
	unsigned int temp;
	unsigned char started = 0;
	temp = input/10000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10000;
	}
	temp = input/1000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%1000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/100;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%100;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/10;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10;
	}
	else if(started)
		LCD_appendNum(0);
	LCD_appendNum(input);
}

//append a unsigned  long int to the output bufer
void LCD_appendULint(unsigned long input)
{
	long temp;
	unsigned char started;
	started = 0;
	temp = input/1000000000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%1000000000;
	}
	temp = input/100000000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%100000000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/1000000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10000000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/1000000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%1000000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/100000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%100000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/10000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/1000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%1000;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/100;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%100;
	}
	else if(started)
		LCD_appendNum(0);
	temp = input/10;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10;
	}
	else if(started)
		LCD_appendNum(0);
	LCD_appendNum(input);
}

//appends an unsigned integer to the output buffer with the specified number of preceeding zeros
void LCD_appendUintZ(unsigned int input,unsigned char zeros)
{
	unsigned int temp;
	unsigned char started = 0;
	temp = input/10000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10000;
	}
	else if(zeros > 4)
		LCD_appendNum(0);
	temp = input/1000;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%1000;
	}
	else if(started|| (zeros > 3))
		LCD_appendNum(0);
	temp = input/100;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%100;
	}
	else if(started|| (zeros > 2))
		LCD_appendNum(0);
	temp = input/10;
	if(temp)
	{
		started = 1;
		LCD_appendNum(temp);
		input = input%10;
	}
	else if(started || (zeros > 1))
		LCD_appendNum(0);
	LCD_appendNum(input);
}

//appends a signed integer to the output buffer
void LCD_appendSint(signed int input)
{
	if(input >= 0)
	{
		LCD_appendChar('+');
		LCD_appendUint(input);
	}
	else
	{
		LCD_appendChar('-');
		LCD_appendUint(0-input);
	}
}

//appends a signed integer to the output buffer, with the specified number of preceeding zeros
void LCD_appendSintZ(signed int input, unsigned char zeros)
{
	if(input >= 0)
	{
		LCD_appendChar('+');
		LCD_appendUintZ(input, zeros);
	}
	else
	{
		LCD_appendChar('-');
		LCD_appendUintZ(0-input, zeros);
	}
}
