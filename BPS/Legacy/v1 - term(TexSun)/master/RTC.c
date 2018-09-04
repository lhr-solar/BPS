//RTC.c
//Fred Engelkemeir, fall 2012
//this code interfaces to the RTC part of the Microchip MCP79411

#include "time.h"

	unsigned char EEPROM[128];
void RTC_test(void);
void RTC_setTime(time newtime);
time RTC_getTime(void);
time RTC_getTimeStampDown(void);
time RTC_getTimeStampDown(void);
void RTC_writeSRAM(unsigned char address, unsigned char data);
unsigned char RTC_readSRAM(unsigned char address);
unsigned char RTC_int2BCD(unsigned char input);
unsigned char RTC_BCD2int(unsigned char input); 

void RTC_test(void)
{
	unsigned char SRAM[16];
	//unsigned char EEPROM[128];
	unsigned char b;
	time temp;
	time new;
	new.year = 12;
	new.month = 11;
	new.day = 10;
	new.hour = 11;
	new.minute = 38;
	new.seconds = 0;
	//RTC_setTime(new);
	//temp = RTC_getTime();
//while(1);
	b = b+1;
	b = b-1;
	//	return;
/*	for(b = 0; b < 16; b++)
	{
		SRAM[b] = RTC_readSRAM(b);
	}
	b = b+1;
	b = b-1;
	for(b = 0; b < 16; b++)
	{
		RTC_writeSRAM(b, b);
	}	
	b = b+1;
	b = b-1;
	for(b = 0; b < 16; b++)
	{
		SRAM[b] = RTC_readSRAM(b);
	}
	b = b+1;
	b = b-1;
	*/
	for(b = 0; b < 128; b++)
	{
		EEPROM[b] = I2C2_ReadEEPROMRandom(b);
		//EEPROM[b] = I2C2_ReadRTC_EEPROMRandom(b);
	}
	b = b+1;
	b = b-1;
	for(b = 0; b < 128; b++)
	{
		I2C2_WriteEEPROM(b,b);
		//I2C2_WriteRTC_EEPROM(b,b);
	}	
	b = b+1;
	b = b-1;
	for(b = 0; b < 128; b++)
	{
		EEPROM[b] = I2C2_ReadEEPROMRandom(b);
		//EEPROM[b] = I2C2_ReadRTC_EEPROMRandom(b);
	}
	b = b+1;
	b = b-1;
	
}	


void RTC_setTime(time newtime)
{
	 unsigned char seconds;
	 unsigned char minutes;
	 unsigned char hours;
	 unsigned char day;
	 unsigned char date;
	 unsigned char month;
	 unsigned char year;
	 
	 seconds = RTC_int2BCD(newtime.seconds);
	 seconds = seconds|0b10000000;				//make sure the ST bit is set
	 minutes = RTC_int2BCD(newtime.minute);
	 hours = RTC_int2BCD(newtime.hour);			//the 12/~24 bit is already cleared (= 24 hour mode)
	 day = 0b00001000;							//set the VBATen bit to enable the battery backup, sets the day of week to 0, and clears the timestamp registers
	 date = RTC_int2BCD(newtime.day);
	 month = RTC_int2BCD(newtime.month);
	 year = RTC_int2BCD(newtime.year);
	 I2C2_WriteRTC(0x00,seconds); 
	 I2C2_WriteRTC(0x01,minutes); 
	 I2C2_WriteRTC(0x02,hours); 
	 I2C2_WriteRTC(0x03,day); 
	 I2C2_WriteRTC(0x04,date); 
	 I2C2_WriteRTC(0x05,month); 
	 I2C2_WriteRTC(0x06,year); 
	 I2C2_WriteRTC(0x07,0b01000000); 	//set up chip
	 I2C2_WriteRTC(0x08,0);				//reset oscilator calibration to 0
}	

time RTC_getTime(void)
{
	unsigned char temp;
	time out;
	temp = I2C2_ReadRTC(0x00);
	temp = temp&0b01111111;		//mask off the ST bit
	out.seconds = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x01);
	temp = temp&0b01111111;		//mask off the unused bit
	out.minute = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x02);
	temp = temp&0b00111111;		//mask off 12/24 bits
	out.hour = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x04);
	temp = temp&0b00111111;		//mask off unused bit
	out.day = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x05);
	temp = temp&0b00011111;		//mask off unused bit and leapyear bit
	out.month = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x06);
	out.year = RTC_BCD2int(temp);
	return out;
}	

time RTC_getTimeStampDown(void)
{
	unsigned char temp;
	time out;
	out.seconds = 0;
	
	temp = I2C2_ReadRTC(0x18);
	temp = temp&0x01111111;		//mask off the unused bit
	out.minute = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x19);
	temp = temp&0x00111111;		//mask off 12/24 bits
	out.hour = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x1A);
	temp = temp&0x00111111;		//mask off unused bit
	out.day = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x1B);
	temp = temp&0x00011111;		//mask off day of week
	out.month = RTC_BCD2int(temp);
	
	out.year = 0;
	return out;
}	

time RTC_getTimeStampUp(void)
{
	unsigned char temp;
	time out;
	out.seconds = 0;
	
	temp = I2C2_ReadRTC(0x1C);
	temp = temp&0x01111111;		//mask off the unused bit
	out.minute = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x1D);
	temp = temp&0x00111111;		//mask off 12/24 bits
	out.hour = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x1E);
	temp = temp&0x00111111;		//mask off unused bit
	out.day = RTC_BCD2int(temp);
	
	temp = I2C2_ReadRTC(0x1F);
	temp = temp&0x00011111;		//mask off day of week
	out.month = RTC_BCD2int(temp);
	
	out.year = 0;
	return out;
}	

void RTC_writeSRAM(unsigned char address, unsigned char data)
{
	if(address >= 64)	//only 64 bytes available in the SRAM
		return;
	address = address + 0x20;
	I2C2_WriteRTC(address,data); 
}
	
unsigned char RTC_readSRAM(unsigned char address)
{
	unsigned char data;
	if(address >= 64)	//only 64 bytes available in the SRAM
		return 0;
	address = address + 0x20;
	return I2C2_ReadRTC(address);
}	

unsigned char RTC_int2BCD(unsigned char input)
{
	unsigned char out;
	unsigned char tens;
	out = input%10;		//get the units
	tens = input/10;		//get the tens
	tens = tens<<4;		//shift the tens digit into the higher nibble
	out = out|tens;
	return out;
}
	
unsigned char RTC_BCD2int(unsigned char input)
{
	unsigned char out;
	unsigned char tens;
	out = input&0x0F;	//mask off the high nibble to get the low nibble = units place
	tens = input&0xF0;	//mask off the low nibble to get the high nibble = the tens place
	tens = tens>>4;		//shift tens 4 places to the right (put it back into the low nibble)
	tens = tens*10;		//multiply by 10
	out = out + tens;
	return out;
}	
