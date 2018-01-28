/*******************************************************
/LTC6803.c
/Implements the Linear Technologies LTC6803 driver
/LTC6803 is a (code compatible) improved version of the LTC6802
/uses SPI.c and SPI2 port
/uses TIMER3 (mostly to give enough time for the AD conversions to occur)
/Fred Engelkemeir, summer 2012
/Edited by Joao Francisco Cristeli do Vale, fall 2012
/Based on Archer Finley's code, spr 2010. THANKS!
*******************************************************/

#include <p30F6012A.h>

#define LTC_WAIT_COUNT 65000 //(40000=16ms at 2.5Mhz Fcy)

int LTC_waiting;			//flag for the timer
int LTC_version;			//check if it is LTC6802 or LTC6803; 2 = LTC6802, 3 = LTC6803
int LTC_verifyPEC;			//do we check the PEC on received data (for both types of chips)(0 = ignore PEC on receive, 1 = reject received data with bad PEC)

static char crc8_table[256];      // 8-bit table for PEC calc
char PECbyte;
static int made_table = 0;         // table made flag
static void init_crc8(void);
void setPECbyte(char m);
char LTC_calculatePEC2(char *data, unsigned int length);

void LTC_readConfig(char *configs);
unsigned char LTC_setConfig(CFG0, CFG1, CFG2, CFG3, CFG4, CFG5);
void LTC_startConversion(void);
void LTC_startOWConversion(void);
char LTC_readVoltage(char *voltages);
char LTC_checkPEC(char *data, char CRC, unsigned int length);
void LTC_sortVoltages(char *input, unsigned int *output);
void LTC_scaleVoltages(unsigned int *voltages);
int LTC_OWcompare(unsigned int *voltages, unsigned int *OWvoltages);
void LTC_startTimer(void);
void LTC_waitConversion(void);
void LTC_timerInitialize(void);
void LTC_sendPoll(void);
char LTC_calculatePEC(char *data, unsigned int length); //Calculate CRC byte for "length" bytes of data
unsigned char LTC_SMBUS(unsigned char inPEC, char inByte);
unsigned char LTC_calculatePEC3(char *data, unsigned int length);


void LTC_readConfig(char *configs)
{
	char data[2];
	data[0] = 0x10;
	data[1] = LTC_calculatePEC(data,1);//could be precalculated and set

	SPI_chipSelectOn();
	SPI_sendReceive(data[0]);
	if (LTC_version == 3)
	{
	    SPI_sendReceive(data[1]);
	}
	SPI_receiveArray(configs, 6);
	SPI_chipSelectOff();
	//TODO: check PEC on receive?
}

unsigned char LTC_setConfig(CFG0, CFG1, CFG2, CFG3, CFG4, CFG5)
{
	char configs[6];
	char tempCFGs[6];
	char temp[2];
	char newcfg[7];
	char cfgPECs[7];
	char PECcfg;
	char PECcmd;
	int z;
	char i;
	/*
	newcfg[0] = 1; //write register command
	newcfg[1] = CFG0;
	newcfg[2] = CFG1;
	newcfg[3] = CFG2;
	newcfg[4] = CFG3;
	newcfg[5] = CFG4;
	newcfg[6] = CFG5;
	tempCFGs[0] = CFG0;
	tempCFGs[1] = CFG1;
	tempCFGs[2] = CFG2;
	tempCFGs[3] = CFG3;
	tempCFGs[4] = CFG4;
	tempCFGs[5] = CFG5;
	temp[0] = newcfg[0];
	temp[1] = LTC_calculatePEC(temp,1);		//PEC for command
	PECcmd = temp[1];

	PECcfg = LTC_calculatePEC(tempCFGs,6);	//PEC for data
	SPI_chipSelectOn();
	if (LTC_version == 2)
	{
		SPI_sendReceive(newcfg[0]);
		SPI_sendReceive(newcfg[1]);
		SPI_sendReceive(newcfg[2]);
		SPI_sendReceive(newcfg[3]);
		SPI_sendReceive(newcfg[4]);
		SPI_sendReceive(newcfg[5]);
		SPI_sendReceive(newcfg[6]);
	}
	else if (LTC_version == 3)
	{
		SPI_sendReceive(newcfg[0]);  
		SPI_sendReceive(PECcmd);
		SPI_sendReceive(newcfg[1]);
		SPI_sendReceive(newcfg[2]);
		SPI_sendReceive(newcfg[3]);
		SPI_sendReceive(newcfg[4]);
		SPI_sendReceive(newcfg[5]);
		SPI_sendReceive(newcfg[6]);
		SPI_sendReceive(PECcfg);
	}
	SPI_chipSelectOff();
	LTC_readConfig(configs);
	*/
	newcfg[0] = 1; //write register command
	newcfg[1] = CFG0;
	newcfg[2] = CFG1;
	newcfg[3] = CFG2;
	newcfg[4] = CFG3;
	newcfg[5] = CFG4;
	newcfg[6] = CFG5;
	tempCFGs[0] = CFG0;
	tempCFGs[1] = CFG1;
	tempCFGs[2] = CFG2;
	tempCFGs[3] = CFG3;
	tempCFGs[4] = CFG4;
	tempCFGs[5] = CFG5;
	temp[0] = newcfg[0];
	temp[1] = LTC_calculatePEC(temp,1);		//PEC for command
	PECcmd = temp[1];
	for(z = 0; z < 256; z++)
	{
		SPI_chipSelectOn();
		SPI_sendReceive(newcfg[0]);  
		SPI_sendReceive(PECcmd);
		SPI_sendReceive(newcfg[1]);
		SPI_sendReceive(newcfg[2]);
		SPI_sendReceive(newcfg[3]);
		SPI_sendReceive(newcfg[4]);
		SPI_sendReceive(newcfg[5]);
		SPI_sendReceive(newcfg[6]);
		SPI_sendReceive(z);
		SPI_chipSelectOff();
		LTC_readConfig(configs);
	}
	if(CFG0 != configs[0])
		return 1;
	if(CFG1 != configs[1])
		return 1;
	if(CFG2 != configs[2])
		return 1;
	if(CFG3 != configs[3])
		return 1;
	if(CFG4 != configs[4])
		return 1;
	if(CFG5 != configs[5])
		return 1;
	return 0;		//it worked, return 0
}

void LTC_startConversion(void)
{
	char data[2];
	data[0] = 0x10;		//start conversion command
	data[1] = LTC_calculatePEC(data,1);//0xB0

	SPI_chipSelectOn();
	if (LTC_version == 3)
	{
		SPI_sendReceive(data[0]);
		SPI_sendReceive(data[1]);
	}
	else if (LTC_version == 2)
	{
		SPI_sendReceive(data[0]);
	}
	SPI_chipSelectOff();
}

void LTC_startOWConversion(void)
{
	char data[2];
	data[0] = 0x20;
	data[1] = LTC_calculatePEC(data,1);//0X20

	SPI_chipSelectOn();
    SPI_sendReceive(data[0]);
	if (LTC_version == 3)
	{
		SPI_sendReceive(data[1]);
	}
	SPI_chipSelectOff();
}

char LTC_readVoltage(char *voltages)
{
	char PEC;
	char data[2];
	data[0] = 0x04;		//code to read voltage/ADC result registers
	data[1] = LTC_calculatePEC(data,1);//0xDC

	SPI_chipSelectOn();
	SPI_sendReceive(data[0]);
    if (LTC_version == 3)
	{
		SPI_sendReceive(data[1]);
	}
	SPI_receiveArray(voltages, 18);
	PEC = SPI_sendReceive(0);
	SPI_chipSelectOff();
	return LTC_checkPEC(voltages, PEC, 18);
}



//return 0 if good, 1 if bad
char LTC_checkPEC(char *data, char CRC, unsigned int length)
{	
	int i;
	unsigned char calcPEC;
	unsigned char calcPEC2;
	calcPEC = LTC_calculatePEC(data, length);
	calcPEC2 = LTC_calculatePEC2(data, length);
	i++;
	i++;
	i++;
	if (CRC == calcPEC)
		return 0;
	else
		return 1;

}

void LTC_sortVoltages(char *input, unsigned int *output)
{	//written by Archer
	int i;
	int j;
	int buffer;
	for(j=0, i=1; i < 17; )
	{
		output[j] = (int)input[i-1];
		output[j] &= 0x00FF;
		buffer = (int)input[i];
		buffer &= 0x0F;
		buffer = buffer<<8;
		output[j] += buffer;
		output[j+1] = (int)input[i];
		output[j+1] &= 0x0F;
		output[j+1] = output[j+1]>>4;
		output[j+1] += (int)input[i+1]<<4;
		output[j+1] &= 0x0FFF;
		i += 3;
		j += 2;
	}
}

void LTC_scaleVoltages(unsigned int *voltages)
{
	int i;
	for(i = 0; i < 12; i++)
	{
		if (LTC_version == 3)
			voltages[i] = voltages[i] - 0x200;	//LTC6803 has an offset (so it can measure negative voltages on supercapacitor strings...)
		//voltages[i] = voltages[i] * 15;
		voltages[i] = (voltages[i]*reg_read(2,i)) + reg_read(3,i);
	}
}

int LTC_OWcompare(unsigned int *voltages, unsigned int *OWvoltages)
{
	int OWgood = 1;
	unsigned int OWlowThresh = 1000;		//TODO: get from the register system
	unsigned int OWhighThresh = 1000;
	int i;
	for(i = 0; i < 12; i++)
	{
		if(OWvoltages[i] > (voltages[i] + OWhighThresh))
			OWgood = 0;
		if(OWvoltages[i] < (voltages[i] - OWlowThresh))
			OWgood = 0;		 
	}
	return OWgood;
}

void LTC_sendPoll(void)
{
	char data[2];
	data[0] = 0x20;
	data[1] = LTC_calculatePEC(data,1);//0x20

	SPI_chipSelectOn();
    SPI_sendReceive(data[0]);
	if (LTC_version == 3)
	{
		SPI_sendReceive(data[1]);
	}
	SPI_chipSelectOff();
}
	

void LTC_startTimer(void)
{
	TMR3 = 0;
	T3CONbits.TON = 1;
	LTC_waiting = 1;
}


void LTC_waitConversion(void)
{
	while(LTC_waiting);
}

//OK, also initializes the type of LTC...
void LTC_timerInitialize(void)
{
	LTC_version = reg_read(1,8);
	made_table = 0;
	LTC_verifyPEC = reg_read(1,9);
	LTC_version = 3;				//TODO: why does reg system not work?
	LTC_verifyPEC = 0;
	T3CONbits.TGATE = 0;	//don't gate it
	T3CONbits.TCKPS0 = 0;	//1:1 (n+-e timer period register
	IFS0bits.T3IF = 0;		//clear interrupt flag
	IEC0bits.T3IE = 1;		//enable interrupt
}

void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)
{
	IFS0bits.T3IF = 0;
	T3CONbits.TON = 0;
	LTC_waiting = 0;
}

char LTC_calculatePEC(char *data, unsigned int length) //Calculate CRC byte for "length" bytes of data
{
//Reference: LTC6803 datasheet
//Thanks Joao!
	int num_byte; //number of current byte being evaluated
	int num_bit; //number of bit within the byte
	unsigned char temp;
	int c; //loop counter
	char PEC; //result - CRC byte
	char IN0, IN1,IN2; //temporary bits

	PEC = 0x41;//start at 0100 0001
	//PEC = 0;

	num_byte = 0;
	for(c=0;c<(length*8);c++)
	{
		num_bit = c%8; 	//integer division remainder - number from 0 to 7
		num_byte = c/8;	//integer division, the number of the byte <<<added by fred>>>
		temp = data[num_byte];
		IN0 = ((PEC >> 7) ^ (temp >> (7 - num_bit))) & 0x01; // IN0 = DIN XOR PEC[7]
								//moves PEC and DIN to pos. 0, XOR, clears other bits
		IN1 = ((PEC ^ IN0) << 1) & 0x02; //IN1 = PEC[0] XOR IN0 - XOR, moves to pos. 1, clears other bits
		IN2 = (((PEC >> 1) ^ IN0) << 2) & 0x04; //IN2 = PEC[1] XOR IN0 - XOR, moves to pos. 2, clears other bits
		PEC = (PEC << 1) & ~0x07; //shifts PEC 1 bit, clears final 3 bits
		PEC = PEC | IN2; //PEC[2] = IN2
		PEC = PEC | IN1; //PEC[1] = IN1
		PEC = PEC | IN0; //PEC[0] = IN0
	}

	return(PEC);
}

//all of this stuff copied and pasted from KiloOne's post on Endless Sphere.com. I have no idea how the hell it works.

static void init_crc8() {
   int z,j;
   char cr;
   if (!made_table) {
      for (z = 0; z < 256; z ++) {
         cr = z;
         for (j = 0; j < 8; j ++) {
            cr = (cr << 1) ^ ((cr & 0x80) ? 0x07 : 0);
         }
         crc8_table[z] = cr & 0xFF;
      }
      made_table = 1;
   }
}

    void setPECbyte(char m) {
       PECbyte = 0x41;                   // initialize PECbyte
       if (!made_table) {
          init_crc8();
       }
       PECbyte = crc8_table[(PECbyte) ^ m];
    }

    char LTC_calculatePEC2(char *data, unsigned int length) {            // np is number of bytes currently in packet[]
       int z;
       char PECpacket = 0x41;                  // initialize PECpacket
       if (!made_table) {
          init_crc8();
       }
       for (z = 0; z < length; z++) {
          PECpacket = crc8_table[(PECpacket) ^ data[z]];
       }
		return PECpacket;
    }

//Copied from some SMBUS page ( http://websvn.hylands.org/filedetails.php?repname=Projects&path=%2Fcommon%2FCrc8.c&sc=1 )
//also don't know how the hell this one works.
unsigned char LTC_calculatePEC3(char *data, unsigned int length)
{
	unsigned char i;
	unsigned char tempPEC = 0x41;
	for(i = 0; i < length; i++)
	{
		tempPEC = LTC_SMBUS(tempPEC,data[i]);
	}
	return tempPEC;
}

unsigned char LTC_SMBUS(unsigned char inPEC, char inByte)
{
    unsigned char   i;
    unsigned char   data;

    data = inPEC ^ inByte;
  
    for ( i = 0; i < 8; i++ ) 
    {
        if (( data & 0x80 ) != 0 )
        {
            data <<= 1;
            data ^= 0x07;
        }
        else
        {
            data <<= 1;
        }
    }
	return data;
}

//why the hell does it need a newline at the end of the damn file?!?!?!?



