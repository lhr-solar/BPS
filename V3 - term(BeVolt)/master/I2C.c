/********************************************************
/ I2C.c
/implments the I2C port to talk with the RTC and the EEPROM
/this code has been copied and pasted and then modified from Microchip app note
/using Archer's initialization code, not Microchip's
********************************************************/

#include <p30F6012A.h>
#include "I2C.h"
static void StartI2C(void);
static void RestartI2C(void);
static void StopI2C(void);
static void WriteI2C(unsigned char byte);
static void IdleI2C(void);
static unsigned int ACKStatus(void);
static void NotAckI2C(void);
static void NotAckI2C(void);
static void AckI2C(void);
static unsigned int getI2C(void);
static unsigned int EEAckPolling(unsigned char control);
static unsigned int getsI2C(unsigned char *rdptr, unsigned char Length);

void I2C_init(void)
{
//Copied from Archer's code, THANKS!	
//Set I2CCON the I2C Control Register
// <15> I2CEN   = 1   Enables the I2C module and configures the SDA and SCL pins as serial port pins
// <13> I2CSIDL = 0   Continue module operation in Idle mode
// <12> SCLREL  = 1   Release SCL clock (when operating as slave)
//                    0 = Hold SCL clock low (clock stretch)
//                    If STREN = 1:
//                      Bit is R/W (i.e., software may write ‘0’ to initiate stretch and write ‘1’ to release clock)
//                      Hardware clear at beginning of slave transmission.
//                      Hardware clear at end of slave reception.
//                    If STREN = 0:
//                      Bit is R/S (i.e., software may only write ‘1’ to release clock)
//                      Hardware clear at beginning of slave transmission.
// <11> IPMIEN  = DNS (operating as master)
// <10> A10M    = 0   I2CADD is a 7-bit slave address
// <9>  DISSLW  = 0   Slew rate control enabled
// <8>  SMEN    = 0   Disable SMBus input thresholds
// <7>  GCEN    = 0   General call address disabled
// <6>  STREN   = 0   Disable software or receive clock stretching
// <5>  ACKDT   = 1   Send NACK during acknowledge
// I2CCON = 1001 0000 0010 0000
  //I2CCON = 0x9020;

//Set I2C baud rate at 400khz
  	I2CBRG = 0x00F;
	I2CCON = 0x1200;
	
	I2CRCV = 0x0000;
	I2CTRN = 0x0000;
	I2CCON = 0x9200;
}


//Public functions to send/receive data
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data)
{
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	//EEAckPolling(ControlByte);		//Perform ACK polling
	return(ErrorCode);
}

unsigned int LDByteWriteWI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data)		//this one "Waits" for the EEPROM to finish the write cycle.
{
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	EEAckPolling(ControlByte);		//Perform ACK polling
	return(ErrorCode);
}

void LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length)
{
	IdleI2C();					//wait for bus Idle
	StartI2C();					//Generate Start Condition
	WriteI2C(ControlByte);		//Write Control Byte
	IdleI2C();					//wait for bus Idle
	WriteI2C(Address);			//Write start address
	IdleI2C();					//wait for bus Idle

	RestartI2C();				//Generate restart condition
	WriteI2C(ControlByte | 0x01);	//Write control byte for read
	IdleI2C();					//wait for bus Idle

	getsI2C(Data, Length);		//read Length number of bytes
	NotAckI2C();				//Send Not Ack
	StopI2C();					//Generate Stop
}

unsigned int HDByteWriteI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data)
{
	unsigned int ErrorCode;

	IdleI2C();						//Ensure Module is Idle
	StartI2C();						//Generate Start COndition
	WriteI2C(ControlByte);			//Write Control byte
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status
	
	WriteI2C(HighAdd);
	IdleI2C();						//Write High Address
	WriteI2C(LowAdd);				//Write Low Address
	IdleI2C();

	ErrorCode = ACKStatus();		//Return ACK Status

	WriteI2C(data);					//Write Data
	IdleI2C();
	StopI2C();						//Initiate Stop Condition
	EEAckPolling(ControlByte);		//perform Ack Polling
	return(ErrorCode);
}

void HDByteReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *Data, unsigned char Length)
{
	IdleI2C();						//Wait for bus Idle
	StartI2C();						//Generate Start condition
	WriteI2C(ControlByte);			//send control byte for write
	IdleI2C();						//Wait for bus Idle

	WriteI2C(HighAdd);				//Send High Address
	IdleI2C();						//Wait for bus Idle
	WriteI2C(LowAdd);				//Send Low Address
	IdleI2C();						//Wait for bus Idle

	RestartI2C();					//Generate Restart
	WriteI2C(ControlByte | 0x01);	//send control byte for Read
	IdleI2C();						//Wait for bus Idle

	getsI2C(Data, Length);			//Read Length number of bytes to Data
	NotAckI2C();					//send Not Ack
	StopI2C();						//Send Stop Condition
}

//=========================================================
//internal functions

static void StartI2C(void)
{
	//This function generates an I2C start condition and returns status 
	//of the Start.

	I2CCONbits.SEN = 1;		//Generate Start COndition
	while (I2CCONbits.SEN);	//Wait for Start COndition
	//return(I2C1STATbits.S);	//Optionally return status
}

static void RestartI2C(void)
{
	//This function generates an I2C Restart condition and returns status 
	//of the Restart.

	I2CCONbits.RSEN = 1;		//Generate Restart		
	while (I2CCONbits.RSEN);	//Wait for restart	
	//return(I2C1STATbits.S);	//Optional - return status
}

static void StopI2C(void)
{
	//This function generates an I2C stop condition and returns status 
	//of the Stop.

	I2CCONbits.PEN = 1;		//Generate Stop Condition
	while (I2CCONbits.PEN);	//Wait for Stop
	//return(I2C1STATbits.P);	//Optional - return status
}

static void WriteI2C(unsigned char byte)
{
	//This function transmits the byte passed to the function
	//while (I2C1STATbits.TRSTAT);	//Wait for bus to be idle
	I2CTRN = byte;					//Load byte to I2C1 Transmit buffer
	while (I2CSTATbits.TBF);		//wait for data transmission
	//while(I2CSTATbits.TRSTAT);
}

static void IdleI2C(void)
{
	while (I2CSTATbits.TRSTAT);		//Wait for bus Idle
}

static unsigned int ACKStatus(void)
{
	return (!I2CSTATbits.ACKSTAT);		//Return Ack Status
}

static void NotAckI2C(void)
{
	I2CCONbits.ACKDT = 1;			//Set for NotACk
	I2CCONbits.ACKEN = 1;
	while(I2CCONbits.ACKEN);		//wait for ACK to complete
	I2CCONbits.ACKDT = 0;			//Set for NotACk
}

static void AckI2C(void)
{
	I2CCONbits.ACKDT = 0;			//Set for ACk
	I2CCONbits.ACKEN = 1;
	while(I2CCONbits.ACKEN);		//wait for ACK to complete
}

static unsigned int getI2C(void)
{
	I2CCONbits.RCEN = 1;			//Enable Master receive
	Nop();
	while(!I2CSTATbits.RBF);		//Wait for receive bufer to be full
	return(I2CRCV);				//Return data in buffer
}

static unsigned int EEAckPolling(unsigned char control)
{
	IdleI2C();				//wait for bus Idle
	StartI2C();				//Generate Start condition
	
	if(I2CSTATbits.BCL)
	{
		return(-1);			//Bus collision, return
	}

	else
	{
	/*	if(WriteI2C(control))
		{
			return(-3);		//error return
		}
	*/
		IdleI2C();			//wait for bus idle
		if(I2CSTATbits.BCL)
		{
			return(-1);		//error return
		}

		while(!ACKStatus())
		{
			RestartI2C();	//generate restart
			if(I2CSTATbits.BCL)
			{
				return(-1);	//error return
			}
/*
			if(WriteI2C(control))
			{
				return(-3);
			}
*/
			IdleI2C();
		}
	}
	StopI2C();				//send stop condition
	if(I2CSTATbits.BCL)
	{
		return(-1);
	}
	return(0);
}

static unsigned int getsI2C(unsigned char *rdptr, unsigned char Length)
{
	while (Length --)
	{
		*rdptr++ = getI2C();		//get a single byte
		
		if(I2CSTATbits.BCL)		//Test for Bus collision
		{
			return(-1);
		}

		if(Length)
		{
			AckI2C();				//Acknowledge until all read
		}
	}
	return(0);
}

//newline
