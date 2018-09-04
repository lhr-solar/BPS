/********************************************************
/ I2C2.c
/implments the I2C port to talk with the RTC and the EEPROM
/this code has been copied and pasted and then modified from Microchip app note
********************************************************/

#include <p30F6012A.h>
unsigned int StartI2C(void);
unsigned int RestartI2C(void);
unsigned int StopI2C(void);
unsigned int WriteI2C(unsigned char byte);
unsigned int IdleI2C(void);
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data);
unsigned int LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length);
unsigned int LDByteWriteI2C(unsigned char ControlByte, unsigned char LowAdd, unsigned char data);
unsigned int HDByteWriteWI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char data);
unsigned int HDByteReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *Data, unsigned char Length);
unsigned int ACKStatus(void);
unsigned int NotAckI2C(void);
unsigned int NotAckI2C(void);
unsigned int AckI2C(void);
unsigned int getI2C(void);
unsigned int EEAckPolling(unsigned char control);
unsigned int getsI2C(unsigned char *rdptr, unsigned char Length);

unsigned int StartI2C(void)
{
	//This function generates an I2C start condition and returns status 
	//of the Start.

	I2CCONbits.SEN = 1;		//Generate Start COndition
	while (I2CCONbits.SEN);	//Wait for Start COndition
	//return(I2C1STATbits.S);	//Optionally return status
}

unsigned int RestartI2C(void)
{
	//This function generates an I2C Restart condition and returns status 
	//of the Restart.

	I2CCONbits.RSEN = 1;		//Generate Restart		
	while (I2CCONbits.RSEN);	//Wait for restart	
	//return(I2C1STATbits.S);	//Optional - return status
}

unsigned int StopI2C(void)
{
	//This function generates an I2C stop condition and returns status 
	//of the Stop.

	I2CCONbits.PEN = 1;		//Generate Stop Condition
	while (I2CCONbits.PEN);	//Wait for Stop
	//return(I2C1STATbits.P);	//Optional - return status
}

unsigned int WriteI2C(unsigned char byte)
{
	//This function transmits the byte passed to the function
	//while (I2C1STATbits.TRSTAT);	//Wait for bus to be idle
	I2CTRN = byte;					//Load byte to I2C1 Transmit buffer
	while (I2CSTATbits.TBF);		//wait for data transmission
	//while(I2CSTATbits.TRSTAT);

}

unsigned int IdleI2C(void)
{
	while (I2CSTATbits.TRSTAT);		//Wait for bus Idle
}

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

unsigned int LDByteReadI2C(unsigned char ControlByte, unsigned char Address, unsigned char *Data, unsigned char Length)
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

unsigned int HDByteReadI2C(unsigned char ControlByte, unsigned char HighAdd, unsigned char LowAdd, unsigned char *Data, unsigned char Length)
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
	return(0);
}

unsigned int ACKStatus(void)
{
	return (!I2CSTATbits.ACKSTAT);		//Return Ack Status
}

unsigned int NotAckI2C(void)
{
	I2CCONbits.ACKDT = 1;			//Set for NotACk
	I2CCONbits.ACKEN = 1;
	while(I2CCONbits.ACKEN);		//wait for ACK to complete
	I2CCONbits.ACKDT = 0;			//Set for NotACk
}

unsigned int AckI2C(void)
{
	I2CCONbits.ACKDT = 0;			//Set for ACk
	I2CCONbits.ACKEN = 1;
	while(I2CCONbits.ACKEN);		//wait for ACK to complete
}

unsigned int getI2C(void)
{
	I2CCONbits.RCEN = 1;			//Enable Master receive
	Nop();
	while(!I2CSTATbits.RBF);		//Wait for receive bufer to be full
	return(I2CRCV);				//Return data in buffer
}

unsigned int EEAckPolling(unsigned char control)
{
	IdleI2C();				//wait for bus Idle
	StartI2C();				//Generate Start condition
	
	if(I2CSTATbits.BCL)
	{
		return(-1);			//Bus collision, return
	}

	else
	{
		if(WriteI2C(control))
		{
			return(-3);		//error return
		}

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

			if(WriteI2C(control))
			{
				return(-3);
			}

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

unsigned int getsI2C(unsigned char *rdptr, unsigned char Length)
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

