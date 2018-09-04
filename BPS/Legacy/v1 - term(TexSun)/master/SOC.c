//uses Timer1


#include <p30F6012A.h>



void SOC_initialize(void);
void SOC_update(void);
void SOC_firstRun(void);
void SOC_writeACC(unsigned long int acc);
unsigned char SOC_checksum1(unsigned long int in);
unsigned char SOC_checksum2(unsigned long int in);
unsigned char SOC_getPercent(unsigned long int acc);
unsigned long int SOC_percent2acc(unsigned char in);


/*
Battery backed SRAM (64 bytes) memory address.............
0		ACC1 LSM
1		ACC1
2		ACC1
3		ACC1 MSB
4		ACC1 checksum1
5		ACC1 checksum2
6		ACC2 LSB
7		ACC2 
8		ACC2
9		ACC2 MSB
10		ACC2 checksum1
11		ACC2 checksum2
12
13
14
15
16
17
18
|~|
64

		accumulator is a 32 bit unsigned long int. stored two times with a checksum for each copy, in case power is lost while writing it, there is still a good copy. checksum is doubled to make sure we only read a "good" one
		checksum1 is the same as for the serial comms, add up all 4 bytes of the accumulaotr in an unsigned char, and then bitwise invert(~)
		checksum2 is 1st (LSB 000x) and 3rd byte (0x00) added together, minus 4th byte( MSB x000) and 2nd byte (00x0). 
*/

unsigned char SOC_timerOverflow;		//flag set if the timer overflows.
unsigned char SOC_initialized;			//binary flag - checks if we're the first run or not.
unsigned int SOC_cycletime;				//units = timer ticks (divide by 176 to get miliseconds)
unsigned long int SOC_accumulator;		//units = "amp-milliseconds"
unsigned char SOC_percent;				//units = percent 0-100
unsigned char SOC_initState;			//holds how the SOC was initialized
										//0 = not initialized
										//1 = acc1 i SRAM good and loaded
										//2 = acc2 in SRAM good and loaded
										//3 = acc1 and acc2 are good, acc1 loaded
										//4 = no good SRAM data, default initialization
extern unsigned char BPSstate;
extern unsigned char arrayAgood;
extern unsigned char arrayBgood;
extern signed int currentA;
extern signed int currentB;

void SOC_initialize(void)
{
	T1CON = 0x0020;		//not gated, 64 prescaler, internal FCY clocking
	TMR1 = 0;
	PR1 = 0xFFFF;		//interrupt when the timer overflows
	SOC_timerOverflow = 0;
	SOC_accumulator = 0;
	SOC_percent = 0;
	SOC_initialized = 0;
	SOC_initState = 0;
	SOC_cycletime = 0;
	IEC0bits.T1IE = 1;
}

void SOC_update(void)
{
	unsigned long int deltaSOC;		//units of amp-milliseconds
	unsigned char polarity;			//0 = positive, 1 = negative
	unsigned long int SOC_curr;			//curent for the state of charge (absolute value of real current)
	if(SOC_initialized)
	{
		//normal SOC updates
		SOC_cycletime = TMR1;
		TMR1 = 0;	
		//if(BPSstate != 2)			//only update the SOC if the contactor is on.
	//		return;
		if(arrayAgood)
		{
			//deltaSOC = (currentA*SOC_cycletime);	//the 100 comes from scaling current into AMPS
			if(currentA<0)
			{
				SOC_curr = 0-currentA;
				polarity = 1;
			}
			else
			{
				SOC_curr = currentA;
				polarity = 0;
			}
		}
		else if(arrayBgood)
		{
			//deltaSOC = (currentB*SOC_cycletime);	//the 39 comes from scaling the timer counts into milliseconds
			if(currentB<0)
			{
				SOC_curr = 0-currentB;
				polarity = 1;
			}
			else
			{
				SOC_curr = currentB;
				polarity = 0;
			}
		}
		else
			return;
		deltaSOC = SOC_curr*SOC_cycletime;
		deltaSOC = deltaSOC/(39*100);
		if(polarity)
			SOC_accumulator -= deltaSOC;						//#TODO: check for overflowing
		else
			SOC_accumulator += deltaSOC;
															//#TODO: check if warn or trip, and force a SOC recalibrate.
		SOC_writeACC(SOC_accumulator);
		SOC_percent = SOC_getPercent(SOC_accumulator);
	}
	else
		SOC_firstRun();
}

void __attribute__((__interrupt__, auto_psv)) _T1Interrupt(void)
{
	IFS0bits.T1IF = 0;
	T1CONbits.TON = 0;
	SOC_timerOverflow = 1;
}

void SOC_firstRun(void)
{
	unsigned char checksum1;
	unsigned char checksum2;
	unsigned char checksum3;
	unsigned char checksum4;
	unsigned long int acc1;
	unsigned long int acc2;
	unsigned char acc1good;
	unsigned char acc2good;
	TMR1 = 0;
	T1CONbits.TON = 1;
	SOC_initialized = 1;
	//first read ACC1
	acc1good = 0;
	acc1 = RTC_readSRAM(0);
	acc1 += (unsigned long int)RTC_readSRAM(1)<<8;
	acc1 += (unsigned long int)RTC_readSRAM(2)<<16;
	acc1 += (unsigned long int)RTC_readSRAM(3)<<24;
	checksum1 = RTC_readSRAM(4);
	checksum2 = RTC_readSRAM(5);
	if((checksum1 == SOC_checksum1(acc1)) && (checksum2 == SOC_checksum2(acc1)))
		acc1good = 1;
	//then read ACC2
	acc2good = 0;
	acc2 = (unsigned long int)RTC_readSRAM(6);
	acc2 += (unsigned long int)RTC_readSRAM(7)<<8;
	acc2 += (unsigned long int)RTC_readSRAM(8)<<16;
	acc2 += (unsigned long int)RTC_readSRAM(9)<<24;
	checksum3 = RTC_readSRAM(10);
	checksum4 = RTC_readSRAM(11);
	if((checksum3 == SOC_checksum1(acc1)) && (checksum4 == SOC_checksum2(acc1)))
		acc2good = 1;
	if(acc1good && acc2good)
	{
		SOC_accumulator = acc1;
		SOC_initState = 3;
	}
	else if(acc1good)
	{
		SOC_accumulator = acc1;
		SOC_initState = 1;
	}
	else if(acc2good)
	{
		SOC_accumulator = acc2;
		SOC_initState = 2;
	}
	else
	{
		SOC_accumulator = SOC_percent2acc(reg_read(3,9));		//default SOC initialization value
		SOC_initState = 4;
	}
	SOC_percent = SOC_getPercent(SOC_accumulator);
}

void SOC_writeACC(unsigned long int acc)
{	
	RTC_writeSRAM(0,(unsigned char)((unsigned long int)acc&0x000000FF));	
	RTC_writeSRAM(1,(unsigned char)(((unsigned long int)acc&0x0000FF00)>>8));
	RTC_writeSRAM(2,(unsigned char)(((unsigned long int)acc&0x00FF0000)>>16));
	RTC_writeSRAM(3,(unsigned char)(((unsigned long int)acc&0xFF000000)>>24));
	RTC_writeSRAM(4,SOC_checksum1(acc));
	RTC_writeSRAM(5,SOC_checksum2(acc));
	RTC_writeSRAM(6, acc&0x000000FF);	
	RTC_writeSRAM(7,(acc&0x0000FF00)>>8);
	RTC_writeSRAM(8,(acc&0x00FF0000)>>16);
	RTC_writeSRAM(9,(acc&0xFF000000)>>24);
	RTC_writeSRAM(10,SOC_checksum1(acc));
	RTC_writeSRAM(11,SOC_checksum2(acc));
}

unsigned char SOC_checksum1(unsigned long int in)
{
	unsigned char out;
	out =   in&0x000000FF;
	out += (in&0x0000FF00)>>8;
	out += (in&0x00FF0000)>>16;
	out += (in&0xFF000000)>>24;
	out = ~out;
	return out;
}

unsigned char SOC_checksum2(unsigned long int in)
{
	unsigned char out;
	out =   in&0x000000FF;
	out += (in&0x0000FF00)>>16;
	out -= (in&0x00FF0000)>>8;
	out -= (in&0xFF000000)>>24;
	return out;
}

unsigned char SOC_getPercent(unsigned long int acc)
{
	unsigned char out;
	unsigned long int temp;
	unsigned long int capacity;
	capacity = reg_read(3,2);		//in amp hours
	capacity = capacity*60*60*10;	//in amp milliseconds / 100
	temp = acc/capacity;				//I divide the capacity by 100 such that the output percentage is a 0-100 instead of a 0-1. if I multiply the result of the division, the remainder bits wil get cut out...
	if(temp > 100)
		temp = 100;
	out = temp;
	return out;
}

unsigned long int SOC_percent2acc(unsigned char in)
{
	unsigned long int intemp;
	unsigned long int out;
	intemp = in;
	out = (unsigned long int) reg_read(3,2);
	if(intemp>100)
		intemp = 100;
	//out = out*60*60*10*intemp;	//60 and 60 change hours to seconds, the 10 is the combination of seconds to millseconds (*1000) and the percent (/100)
	out = intemp*out;
	out = out*36000;
	SOC_accumulator = out; 
	return out;
}
//newline
