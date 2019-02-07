/*******************************************************
/SPI.c
/Implements the slave SPI bus
/to communicate with the LTC6802/LTC6803
/this file implementes the SPI bus and interfaces with the hardware.
/LTC6803.c implements the protocol of the LTC chip.
/the LTC is on SPI 2
/the CS is on pin G9
/Fred Engelkemeir, summer 2012
/Based on Archer Finley's code, spr 2010. THANKS!
*******************************************************/

#include <p30F6012A.h>
#include "SPI.h"

void SPI_initialize()
{
	SPI2CON = 0x0277;
	if(SPI2BUF);				//dummy read to clear the SPIROV bit
	SPI2STAT = 0xA000;
	IFS1bits.SPI2IF = 0;
	IEC1bits.SPI2IE = 1;		//enable SPI ionterrupt
	TRISGbits.TRISG9 = 0;		//LATG9 is the CS output to the LTC
	SPI_chipSelectOff();
}

void __attribute__((__interrupt__, auto_psv)) _SPI2Interrupt(void)
{
	IFS1bits.SPI2IF = 0;		//reset SPI interrupt flag
}

void SPI_chipSelectOn(void)
{
	LATGbits.LATG9 = 0;
}

void SPI_chipSelectOff(void)
{
	LATGbits.LATG9 = 1;
}

unsigned char SPI_sendReceive(unsigned char data)
{

	while(SPI2STATbits.SPITBF);				//busy-wait until TXbuf is empty
	SPI2BUF = data;							//load transmit buffer
	while(!SPI2STATbits.SPIRBF);			//wait until TXbuffer is sent/RXbuffer is received
	return SPI2BUF;							//return the RXbuffer
}

void SPI_receiveArray(char *pt, unsigned int length)
{
	while(length)
	{
		*pt = SPI_sendReceive(0);
		pt += 1;
		length -= 1;
	}
}

//fucking newline at end of file
