/********************************************
/RS422.c
/Implements the "FMF" protocol.
/similar to the RS485 code
/uses UART2 and TIMER3
/"controller" refers to the BeagleBone or PC running the GUI configurator
********************************************/

#include <p30F6012A.h>
#include "time.h"
#include "globals.h"
#include "RS422.h"
#include "RS485master.h" //use this to get those RS485_split and combine MSB/LSB functions
#include "terminal.h"
#include "Mregisters.h"
#include "RTC.h"
#include "misc.h"
#include "send.h"

#define RS422_START_TOKEN 0x01
#define RS422_END_TOKEN 0xFE
#define RS422_TIMEOUT 25000 //10ms timeout
#define RS422_BPS_DEV_ID 1
#define MAX_ENQUED_CMDS 5
#define RS422_TERM_ESCAPE_CODE 32	//character to enter terminal mode (dec 32 = "space bar")
#define RS422_TERM_ESCAPE_COUNT	3	//need to hit space bar three times in a row to enter terminal mode


//packet mode variables (global global)
unsigned char RS422_outData[255];					//data to controller
unsigned char RS422_inData[255];					//data from controller
unsigned char RS422_inDevID;						//device ID from Master
unsigned char RS422_inCommand;						//command from the controller
unsigned char RS422_outResponse;					//response/data-ID to the controller
unsigned char RS422_outLength;						//DATA length on output packets
unsigned char RS422_inLength;						//DATA length on input packets

//packet mode variables (global to this c file)
static unsigned char RS422_outChecksum;					//checksum to send 
static unsigned char RS422_inChecksum;						//checksum from controller
static unsigned char RS422_inDataPointer;					//array element pointer when receiver DATA from controller
static unsigned char RS422_inStep;							//which section of packet we are receiving
static unsigned char RS422_outStep;						//which section of packet we are sending
static unsigned char RS422_outDataPointer;					//array element pointer when sending DATA to controller
static unsigned char RS422_goodPacket;						//controller sent a good packet
static unsigned char RS422_sending;						//1 if we are currently sending a packet, 0 otherwise

//terminal mode variables(public)
unsigned char RS422_termMode;						//0 = normal (packet) mode, 1 = interactive terminal mode
unsigned char RS422_termInPointer;					//counts the characters received
unsigned char RS422_termInBuff[255];				//holds one line of user input
unsigned char RS422_termOutBuff[255];				//holds the output to the user
unsigned char RS422_termOutLength;

//terminal mode variables (private)
static unsigned char RS422_termEscapeCount;				//counts the number of escape characters received in sequence
static unsigned char RS422_termOutPointer;					//counters the characters sent out
static unsigned char RS422_termReceiving;					//0 = not receiving (waiting for user to type first character), 1 = user is typing (user has sent one command, but not pressed enter yet)
static unsigned char RS422_termSending;					// 0 = not sending, 1 = in the process of sending


//stuff for queing up commands to run from mainline.
typedef struct 										//represents a command that is waiting for execution
{													
	unsigned char CMD;		//command code
	unsigned char length;	//length of DATA
	unsigned char data[255];//the actual DATA
} waitingCMD;
static waitingCMD enquedCMDs[MAX_ENQUED_CMDS];		//holds all of the waitingCMDs (currently 5)
static unsigned char RS422_queIn;							
static unsigned char RS422_queCount;
static unsigned char RS422_inQueHandler;

//LCD-mode variables (public)
unsigned char RS422_LCDsending = 0;		//1 when LCD is displaying stuff
unsigned char RS422_LCDready = 0;		//1 = LCD_buffer is full and ready to send
unsigned char RS422_LCDdone = 1;		//1 = LCD has finished a transmission (mainline is GO to put new stuff in LCD_buffer). init as 1 since the LCD is free and buffer is ready for new messages
//LCD-mode variables(private)
static unsigned char RS422_SwitchReq = 0;		// 1 means on next TX_ISR, switch the serial output
static unsigned char RS422_SwitchCMD = 0;		//0 = RS422 (terminal or packet), 1 = LCD


//local functions
static void RS422_processPacket(void);
static void RS422_enqueCMD(void);
static void RS422_runQuedCMD(unsigned char item);
static void RS422_GOTOLCD(void);
static void RS422_GOTO422(void);



void RS422_sendPacket(void)
{
	if(RS422_termMode)				//don't actually send messages when in terminal mode
		return;
	RS422_outChecksum = 0;			//initialize checksum
	RS422_sending = 1;
	RS422_outStep = 0;				//start at step 0
	RS422_outDataPointer = 0;		//start at first byte of response data/payload array
	U2TXREG = RS422_START_TOKEN;	//put start token in Transmit register
}

void RS422_initialize()
{
	RS422_termMode = 0;
	RS422_termEscapeCount = 0;
	RS422_termSending = 0;
	RS422_termReceiving = 0;
	RS422_sending = 0;
	RS422_queIn = 0;							
	RS422_queCount = 0;
	RS422_inQueHandler = 0;	//semaphore to prevent stepping on toes when the mainline is pulling stuff out of que and interrupt handler is adding to que.
	RS422_inStep = 0;
	RS422_inDataPointer = 0;
//set up the inter-byte receive timeout timer (Timer3)
	T3CONbits.TGATE = 0;	//don't gate it
	T3CONbits.TCKPS0 = 0;	//1:1 (no) prescaller
	T3CONbits.TCKPS1 = 0;
	T3CONbits.TCS = 0;		//use Fcy clock
	PR3 = RS422_TIMEOUT;	//load the timer period register
	IFS0bits.T3IF = 0;		//clear interrupt flag
	IEC0bits.T3IE = 1;		//enable interrupt
	T3CONbits.TON = 0;		//timer only runs when the receiver is in the process of receiving a packet (inter-packet timeout)

//if(boardType == 1)
	TRISDbits.TRISD2 = 0;

//Set U1MODE (UART1 Mode Register) options 
// <15>  UARTEN = 1  UART Enable bit
// <13>  USIDL  = 0  Continue operation in Idle mode
// <10>  ALTIO  = 0  UART communicates using UxTX and UxRX I/O pins
// <7>   WAKE   = 1  Wake-up on Start bit Detect During Sleep Mode enabled
// <6>   LPBACK = 0  Loopback mode is disabled
// <5>   ABAUD  = 1  Auto Baud Enable bit, Input to Capture module from UxRX pin
// <2:1> PDSEL  = 00 8-bit data, no parity
// <0>   STSEL  = 1  2 Stop bits
// U1MODE = 1000 0000 1010 0001
	U2MODE = 0x80A0;
	U2MODEbits.ABAUD = 0;

//Set U1STA (UART1 Status and Control Register) options
// <15>  UTXISEL = 1  Interrupt when a character is transferred to the Transmit Shift 
//                    register and as result, the transmit buffer becomes empty
// <11>  UTXBRK  = 0  U1TX pin operates normally
// <10>  UTXEN   = 1  UART transmitter enabled, UxTX pin controlled by UART (if UARTEN = 1)
// <7:6> URXISEL = 00 Interrupt flag bit is set when Receive Buffer receives 1 character(1 byte of data)
// <5>   ADDEN   = 0  Address Detect mode disabled. If 9-bit mode is not selected, this control bit has no effect.
// U1STA = 1000 0100 0000 0000
	U2STA = 0x8400;

//Set U1BRG (UART1 Baud Rate Register) options
// U1BRG = (Fcy/(16*BaudRate)) - 1 = 0 for 625 kbaud
	U2BRG = 15; //close to 9600 buad


	IFS1bits.U2TXIF = 0;
	IFS1bits.U2RXIF = 0;
	IEC1bits.U2TXIE = 1;
	IEC1bits.U2RXIE = 1;

}
	

void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt(void)
{	//gets called whenever the TXbuffer is empty. so this code basically runs right when the previously sent byte's start bit heads down the wire
	IFS1bits.U2TXIF = 0;					//clear interrupt flag
	if(RS422_SwitchReq >= 2)
	{
		if(RS422_SwitchCMD)		//change to the LCD
		{
			RS422_GOTOLCD();
		}
		else					//change to the RS422
		{
			RS422_GOTO422();
		}
		RS422_SwitchReq = 0;
	}
	if(RS422_SwitchReq == 1)
	{
		RS422_SwitchReq = 2;
		//now start us up again
		if(RS422_SwitchCMD)		//if CMD was to switch to LCD
		{						//start sending LCD stuff
			RS422_LCDsending = 1;
			RS422_LCDready = 0;		//turn off the flag that indicates that the LCD wants to talk
			U2TXREG = LCD_buffer[0];
			LCD_outPointer = 1;
			return;
		}
		else					//if CMD was to change back to RS422 mode
		{						//start sending either terminal mode stuff or battery data packets
			if(RS422_termMode)
			{
				RS422_termSending = 0;		//go back and see if anything is left from the 
				term_sendDone();		//#TODO: check if the terminal is done here, it might need to send out some junk data
				return;	
			}
			else
			{
				if(TX_sendingBatt)		//we're sending battery data
					send_next();			//so send the next packet in the schedule
				return;					//serial port wasn't in use
			}
		}
	}
	if(RS422_LCDsending)		//we're currently sending crap to the LCD
	{
		
		if(LCD_outPointer < LCD_outLength)	//we're not done sending
		{
			U2TXREG = LCD_buffer[LCD_outPointer];
			LCD_outPointer++;
			if(LCD_outPointer >= LCD_outLength)		//we're sending the last LCD byte. switch back to RS422 mode
			{										//NOT an else if. control is supposed to fall through here after the above increment outpointer since this won't be called again after the last character is put into TXREG
				RS422_LCDsending = 0;
				RS422_LCDdone    = 1;
				RS422_SwitchCMD  = 0;
				RS422_SwitchReq  = 1;
			}
		}
		return;
	}
	if(RS422_termSending)					//sending out terminal mode responses
	{
		if(RS422_termOutPointer >= RS422_termOutLength)
		{	
			if(!RS422_LCDready)				//update the LCD instead of sending the (possible) next terminal buffer if it's ready
			{								//otherwise deal with the terminal stuff
				RS422_termSending = 0;
				term_sendDone();
				return;	
			}
			else
			{
				RS422_SwitchCMD = 1;
				RS422_SwitchReq = 1;
				//U2TXREG = 0;			//send a junk byte	 (will be received by the PC termianl. a null shouldn't do anything)
				RS422_GOTOLCD();
				RS422_LCDsending = 1;
				RS422_LCDready = 0;		//turn off the flag that indicates that the LCD wants to talk (since we just set LCDsending = 1)
				U2TXREG = LCD_buffer[0];
				LCD_outPointer = 1;
			}
		}
		else
		{
			U2TXREG = RS422_termOutBuff[RS422_termOutPointer];
			RS422_termOutPointer++;
			return;
		}
	}
	if(RS422_sending == 0)
	{
		RS422_outStep = 0;
		return;		//terminal mode was entered while sending a packet.
					//so stop sending the current packet.
	}
	if(RS422_outStep == 0)						//just finished from step 0, outputting the start token
	{											//so we output the length
		U2TXREG = RS422_outLength;				//queue up the response packet length field
		RS422_outChecksum += RS422_outLength;	//add sent byte to checksum
		RS422_outStep = 1;						//just finished step 1
		return;									//exit from ISR					
	}
	else if (RS422_outStep == 1)				//just finished from step 1, outputting the length
	{											//so we send the master-ID code
		U2TXREG = RS422_BPS_DEV_ID;					//send out the slave-ID
		RS422_outChecksum += RS422_BPS_DEV_ID;		//add to checksum	
		RS422_outStep = 2;						//just finished step 2
		return;									//exit from ISR
	}
	else if (RS422_outStep == 2)				//just finished step 2, outputting the master-ID
	{											//now send the status
		U2TXREG = RS422_outResponse;			//put response in serial port transmit waiting hold
		RS422_outChecksum += RS422_outResponse;		//add to the checksum
		if (RS422_outLength == 0)				//check to see if there are any DATA/PAYLOAD byte(s) to send
			RS422_outStep = 4;					//if there aren't any, we just finished "sending" the non-existant data (step4), go to the step after send-DATA
		else	
			RS422_outStep = 3;					//just finished step 3, no goto step 4
		return;									//exit ISR
	}
	else if (RS422_outStep == 3)				//just finished sending the status-response byte, and there's data to send
	{											//now send response-DATA 
		if (RS422_outLength <= (RS422_outDataPointer+1))				//either only one byte for response-DATA, or need to send out last data byte
		{	
			U2TXREG = RS422_outData[RS422_outDataPointer];	//send data byte
			RS422_outChecksum += RS422_outData[RS422_outDataPointer];	//add data byte to the checksum
			RS422_outStep = 4;					//just finished sending the DATA, step 4, so goto step 5, sending checksum				
			return;								//exit ISR								
		}
		else									//we have at least one more byte to send after this
		{
			U2TXREG = RS422_outData[RS422_outDataPointer];	//send data byte
			RS422_outChecksum += RS422_outData[RS422_outDataPointer];	//add data byte to the checksum
			RS422_outDataPointer += 1;			//increment to the next data byte (to be sent next cycle)
			return;								//exit ISR
		}
	}
	else if (RS422_outStep == 4)				//just finished from step 4, outputting the DATA (or we skipped it)
	{											//so we send the checksum
		RS422_outChecksum = ~RS422_outChecksum; //complement the checksum
		U2TXREG = RS422_outChecksum;			//send out the checksum
		RS422_outStep = 5;						//just finished step 5
		return;									//exit from ISR
	}
	else if (RS422_outStep == 5)				//just finished from step 5, outputting the checksum
	{											//so we send the end-token
		U2TXREG = RS422_END_TOKEN;				//send out the end toekn
		RS422_outStep = 6;						//now we're done with the sending
		return;									//exit from ISR
	}
	else if (RS422_outStep == 6)
	{
		RS422_outStep = 0;						//TXREG is empty, this means that end-token has been sent
		RS422_sending = 0;
		if(RS422_LCDready)			
		{							
			RS422_SwitchCMD = 1;
			RS422_SwitchReq = 1;
			//U2TXREG = 0;			//send a junk byte	 (will be received by the PC termianl. a null shouldn't do anything)
			RS422_GOTOLCD();
			RS422_LCDsending = 1;
			RS422_LCDready = 0;		//turn off the flag that indicates that the LCD wants to talk (since we just set LCDsending = 1)
			U2TXREG = LCD_buffer[0];
			LCD_outPointer = 1;
		}
		else
			send_next();							//send the next output packet
	}
	return;									//exit ISR, we're done sending the packet. the ISR shouldn't be called again unless the RS485_sendResponse is called since the UART TX is empty
}

//interrupt for when the RS422 receives a byte
void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void)
{
	unsigned char temp;							//holds the last received byte
	unsigned char RXproc;						//flag indicating if we are processing data or done for this loop around
	IFS1bits.U2RXIF = 0;
	while(U2STAbits.URXDA)						//while there is data in receive FIFO (4 levels deep)
	{
		if(RS422_termMode)		//terminal mode receive code
		{
				T3CONbits.TON = 0;
				temp = U2RXREG;
				if(temp == 27)					// "escape" key
				{								//to exit terminal mode
					RS422_termMode = 0;
					TX_sendingBatt = 0;		//alert the mainline code that we aren't sending battery data (it just though it was during the terminal session....)
					RS422_termEscapeCount = 0;
					return;
				}
				if(RS422_termReceiving)
				{
					if((temp == '\r') || (temp == '\n'))		//if we get an enter key, send the captured input to command interrpreter
					{
						term_executeCMD();
						RS422_termReceiving = 0;
						RS422_termInPointer = 0;
						return;
					}
					RS422_termInBuff[RS422_termInPointer] = temp;
					if(RS422_termInPointer == 255)
					{	//when inbuff overflows, reset (discard input) and exit interrupt handler
						RS422_termReceiving = 0;
						RS422_termInPointer = 0;
						return;
					}
					else
						RS422_termInPointer++;
					return;
				}
				else		//first character of user input
				{
					RS422_termReceiving = 1;
					RS422_termInBuff[RS422_termInPointer] = temp;
					RS422_termInPointer++;
				}
				return;
		}
		//packet mode receive code
		RXproc = 1;
		if (RS422_inStep == 0)
		{											//this is the first byte received
			temp = U2RXREG;
			if (temp == RS422_START_TOKEN)
			{
				TMR3 = 0;							//reset the timer
				T3CONbits.TON = 1;					//start the timer
				PR3 = RS422_TIMEOUT;				//switch the timer over from the (long) slave-thinking timer to the inter-byte timer
				RS422_goodPacket = 1;				// start off with the assumption that it's a good packet
				RS422_inStep = 1;					//just finished the first step (start token)
				RXproc = 0;								//exit interrupt handler
			}
			else if(temp == RS422_TERM_ESCAPE_CODE) //trying to enter terminal mode
			{
				T3CONbits.TON = 0;
				RS422_termEscapeCount++;
				if(RS422_termEscapeCount >= RS422_TERM_ESCAPE_COUNT)
				{
					RS422_termMode = 1;
					RS422_sending = 0;
					term_entered();
				}
				RXproc = 0;	
			}
			else
			{
				T3CONbits.TON = 0;
				RXproc = 0;									//exit interrupt handler
			}
		}
		else if (RS422_inStep == 1 && RXproc == 1)
		{											//this is the second (length) byte
			TMR3 = 0;								//reset the timer
			temp = U2RXREG;							//read out of the receiver  buffer (can only do once, as its a FIFO?, hence the temp variable)
			RS422_inLength = temp;					//set packet length
			RS422_inChecksum = temp;				//start the checksum
			RS422_inStep = 2;						//just finished the second step (length)
			RXproc = 0;										//exit ISR
		}
		else if (RS422_inStep == 2 && RXproc == 1)
		{
			TMR3 = 0;								//reset the timer
			temp = U2RXREG;
			if(temp == RS422_BPS_DEV_ID)					//packet is for the master?
			{									//yes it is for me
				RS422_inChecksum += temp;			//add to checksum
			}
			else
			{
				RS422_goodPacket = 0;
			}
										//crap packet, no need to worry about checksum from now on....
			RS422_inStep = 3;
			RXproc = 0;	
		}
		else if (RS422_inStep == 3 && RXproc == 1)					//if we're on the command step
		{
			TMR3 = 0;								//reset the timer
			temp = U2RXREG;
			if (RS422_goodPacket)					//only do stuff with the byte if it's a packet we care about
			{
				RS422_inCommand = temp;				//set the command
				RS422_inChecksum += temp;			//add to checksum
			}
			if(RS422_inLength)						//if DATA field length non-zero
				RS422_inStep = 4;					//go to the data packet reception step
			else
				RS422_inStep = 5;					//if no data packet contents, skip that step
			RXproc = 0;		
		}
		else if (RS422_inStep == 4 && RXproc == 1)
		{
			TMR3 = 0;								//reset the timer
			temp = U2RXREG;
			if (RS422_goodPacket)
			{
				RS422_inChecksum += temp;
				RS422_inData[RS422_inDataPointer] = temp;
			}
			RS422_inDataPointer += 1;
			if(RS422_inDataPointer >= RS422_inLength)	//did we get the last data byte?
				RS422_inStep = 5;						//then go to the next step
			RXproc = 0;											//othewise just come back here and grab the next data byte
		}
		else if (RS422_inStep == 5 && RXproc == 1)
		{
			TMR3 = 0;								//reset the timer
			temp = U2RXREG;
			RS422_inChecksum = ~RS422_inChecksum;		//bitwise invert the checksum 
			if(temp != RS422_inChecksum)				//if the checksums don't match
				RS422_goodPacket = 0;					//then the packet is no good
			RS422_inStep = 6;		
			RXproc = 0;	
		} 
		else if (RS422_inStep == 6 && RXproc == 1)
		{
			T3CONbits.TON = 0;								//turn off the timer
			if( RS422_goodPacket && (U2RXREG == RS422_END_TOKEN))		
			{	//we have a good packet here, process it
				RS422_inStep = 0;
				RS422_inDataPointer = 0;
				RS422_processPacket();
			}
			else
			{	//we have a bad packet, so reset for the next one
				RS422_inStep = 0;
				RS422_inDataPointer = 0;
			}
			RXproc = 0;	
		}
		/*
		//shouldn't get here
		//unless this interrupt triggers while we are processing/sending a packet (shouldn't happen)
		T3CONbits.TON = 0;
		RS422_inStep = 0;
		RS422_inDataPointer = 0;
		*/
	}
}

//timmer interrupt. it's for the inter-byte timout on a 422 receive
void __attribute__((__interrupt__, auto_psv)) _T3Interrupt(void)
{
	IFS0bits.T3IF = 0;
	T3CONbits.TON = 0;
	RS422_inStep = 0;
	RS422_inDataPointer = 0;
}

//starts the process of sending a message to the terminal user
void RS422_sendTerm(void)
{
	if(RS422_termMode == 0)
		return;
	if(RS422_termOutLength == 0)
		return;
	RS422_termSending = 1;
	RS422_sending = 0;
	U2TXREG = RS422_termOutBuff[0];
	RS422_termOutPointer = 1;
	return;
}

void RS422_sendLCD(void)
{
	RS422_LCDdone = 0;		//inicate that the LCD code can't update the buffer

	//what we want to do is have teh ISR switch the TX ISR switch the serial ports for us (at the end of a transmission) if it is in 
	//the process of transmitting. if it is not transmitting, we need to fire the first byte (and change direction) ourselves since otherwise we won't see the ISR anytime soon.

	//while((RS422_sending == 1) || (RS422_termSending == 1));		//spin until LCD is free #TODO: seems stupid
	
	if((RS422_sending == 1) || (RS422_termSending == 1))			//#TODO: does this code need to be atomic?
	{	//i.e., serial port is talking and we should see an ISR
		RS422_LCDready = 1;		//tell the ISR that it is ready to transmitt LCD stuff. LCD_ready is a bad choice of name. it really means "hey ISR, change over to LCD stuff when you're done with what you're sending right now"
	}
	else	//we can't relay on the ISR to start sending the data, need to manually send the first byte
	{
		RS422_GOTOLCD();
		RS422_LCDsending = 1;
		RS422_LCDready = 0;		//turn off the flag that indicates that the LCD wants to talk (since we just set LCDsending = 1)
		LCD_outPointer = 1;
		RS422_SwitchReq = 0; 		//we already switched the LCD lines over, so make sure that the ISR doesn't switch immediately back (in the case of a silent termianl, the last cleanup ISR run will not run)
		U2TXREG = LCD_buffer[0];
	}
}

//flip the LCDsel line
static void RS422_GOTOLCD(void)
{
	PORTDbits.RD2 = 1;
}

static void RS422_GOTO422(void)
{
	PORTDbits.RD2 = 0;
}

//returns 1 if ready, 0 if busy (waiting or transmitting)
//always returns 1 "ready" if in terminal mode (since RS422 interface dos not send data when in term mode, just pretends to do so!)
unsigned char RS422_ready(void)
{
	if(RS422_termMode)
		return 1;
	if(RS422_LCDsending)
		return 0;
	if(RS422_SwitchReq)
		return 1;
	//return (RS422_outStep == 0);
	if(RS422_sending)
		return 0;
	else
		return 1;
}

//respond to BBone commands
static void RS422_processPacket(void)
{
	if(RS422_inCommand == 1 && RS422_inLength == 0)				//start measuring
	{
		if(BPSstate == 0 || BPSstate == 4)
			BPSstate = 1;
		return;
	}
	else if	(RS422_inCommand == 2 && RS422_inLength == 0)		//stop measuring
	{
		if(BPSstate == 1 || BPSstate == 3)
			BPSstate = 0;
		return;
	}
	else if	(RS422_inCommand == 3 && RS422_inLength == 0)		//turn on system
	{
		if(BPSstate != 2)
			BPSstate = 5;
		return;
	}
	else if	(RS422_inCommand == 4 && RS422_inLength == 0)		//turn off system
	{
		if(BPSstate == 0 || BPSstate == 4)
			BPSstate = 0;
		else
			BPSstate = 1;
		return;
	}
	else if	(RS422_inCommand == 5 && RS422_inLength == 0)		//reset trip
	{
		if(BPSstate == 3 || BPSstate == 4)
			BPSstate = 5;
		return;
	}
	else if	(RS422_inCommand == 6 && RS422_inLength == 3)		//read register
	{
		RS422_enqueCMD();	//do it later (needs RS422 bus transmit access, maybe RS485 bus access)
		return;
	}
	else if	(RS422_inCommand == 7 && RS422_inLength == 5)		//set register
	{
		if(RS422_inData[0] == 0)								//if we write to one of the master's registers, do it now.
			reg_write(RS422_inData[1],RS422_inData[2], RS485_combineBytes(RS422_inData[3], RS422_inData[4]));
		else
			RS422_enqueCMD();		//if it's a slave byte, do it later (needs RS485 bus access)
		return;
	} 
	else if	(RS422_inCommand == 8)								//read freezeframe
	{
		RS422_enqueCMD();	//later, since needs RS422 transmit access
		return;
	}
	else if	(RS422_inCommand == 9 && RS422_inLength == 0)		//read health
	{
		RS422_enqueCMD();	//do it later since need RS422 transmitt access
		return;
	}
	else if	(RS422_inCommand == 100 && RS422_inLength == 0)		//identify system			
	{
		RS422_enqueCMD();	//do it later since need RS422 transmitt access
		return;
	}
	else if	(RS422_inCommand == 101)							//passthrough
	{
		RS422_enqueCMD();	//do it later since need RS422 transmitt access and full RS485 access
		return;
	}
	else if	(RS422_inCommand == 102 && RS422_inLength == 0)							//readRTC
	{
		RS422_enqueCMD();	//do it later, since need RS422 transmitt access and I2C access
		return;
	}
	else if	(RS422_inCommand == 103)
	{
		RS422_enqueCMD();	//do it later since need I2C access
		return;
	}
}
//meant to be called from receive interrupt handler
static void RS422_enqueCMD(void)
{
	unsigned char i;
	unsigned char end;
	if(RS422_inQueHandler)		//drop command on the floor if the mainline code is currently adjusting the que pointers to avoid race condition
		return;
	if(RS422_queCount >= MAX_ENQUED_CMDS)	//is que full? then drop command on the floor
		return;	
	end = (RS422_queIn + RS422_queCount) % MAX_ENQUED_CMDS;		//generate slot to fill into
	enquedCMDs[end].CMD = RS422_inCommand;
	enquedCMDs[end].length = RS422_inLength;
	for(i = 0; i < RS422_inLength; i++)
		enquedCMDs[end].data[i] = RS422_inData[i];
	RS422_queCount++;								
}	

//meant to be called from mainline code
//if exclusive is 1, it will pause until the RS422 is done transmitting (i.e., it WILL always execute a command if one is there), otherwise will only execute if the RS422 transmit is idle
void RS422_dequeCMD(unsigned char exclusive)
{
	if(RS422_queCount == 0)
		return;							//nothing in que
	if(exclusive)
		while (RS422_ready() != 1);		
	else
		if(RS422_ready() != 1)
			return;
	RS422_runQuedCMD(RS422_queIn);
	RS422_inQueHandler = 1;
    RS422_queIn= (RS422_queIn + 1) % MAX_ENQUED_CMDS;
    RS422_queCount--;
	RS422_inQueHandler = 0;
}

unsigned char RS422_commandsWaiting(void)		//returns the number of unprocessed items
{
	return RS422_queCount;
}	

//called from RS422_dequeCMD (that's where the item is actually taken off the cue)
//hence this code is executed from mainline and not interrupt handler
static void RS422_runQuedCMD(unsigned char item)
{
	int temp;
	unsigned char retryCntr;
	unsigned char badRead;
	time ttemp;
	if(item >= MAX_ENQUED_CMDS)
		return;						//indexing off the end of the array?
	if(enquedCMDs[item].CMD == 6 && enquedCMDs[item].length == 3)			//read register
	{
		while(TX_sendingBatt);			//don't stomp on the RS422 system's values until the port has been idled.+++-
		RS422_outLength = 5;
		RS422_outResponse = 6;
		RS422_outData[0] = enquedCMDs[item].data[0];	//slave-ID (0=master)
		RS422_outData[1] = enquedCMDs[item].data[1];	//group
		RS422_outData[2] = enquedCMDs[item].data[2];	//register
		if(enquedCMDs[item].data[0] == 0)				//from the master
		{
			temp = reg_read(enquedCMDs[item].data[1], enquedCMDs[item].data[2]);	
			RS422_outData[3] = RS485_splitMSB(temp);
			RS422_outData[4] = RS485_splitLSB(temp);
		}
		else if (enquedCMDs[item].data[0] <= 15)		//from the slaves
		{
			retryCntr = reg_read(1,10);
			badRead = 0;
			do				//loops around trying to talk
			{
				if(badRead)						//if we're looping back around...
					retryCntr--;
				RS485_slave = enquedCMDs[item].data[0];
				RS485_command = 3;
				RS485_outLength = 2;
				RS485_sendPacket(1);
				while(!RS485_ready())
				{
					//TODO: misc task
				}
				badRead = (RS485_status != 4);
			}while(badRead && retryCntr);
			if(badRead)
				return;			//should do something smarter if there's a problem?
			if(RS485_response)
				return;
			if(RS485_inLength != 2)
				return;
			RS422_outData[3] = RS485_inData[0];
			RS422_outData[4] = RS485_inData[1];
		}
		else
			return;		
		RS422_sendPacket();
	}	
	else if	(enquedCMDs[item].CMD == 7 && enquedCMDs[item].length == 5)		//set register
	{
		if(enquedCMDs[item].data[0])
		{	//master's register
			reg_write(enquedCMDs[item].data[1],enquedCMDs[item].data[2], RS485_combineBytes(enquedCMDs[item].data[3], enquedCMDs[item].data[4]));
		}
		else if (enquedCMDs[item].data[0] <= 15)
		{
			retryCntr = reg_read(1,10);
			badRead = 0;
			do				//loops around trying to talk
			{
				if(badRead)						//if we're looping back around...
					retryCntr--;
				RS485_slave = enquedCMDs[item].data[0];
				RS485_outData[0] = enquedCMDs[item].data[1];
				RS485_outData[1] = enquedCMDs[item].data[2];
				RS485_outData[2] = enquedCMDs[item].data[3];
				RS485_outData[3] = enquedCMDs[item].data[4];
				RS485_command = 2;
				RS485_outLength = 4;
				RS485_sendPacket(1);
				while(!RS485_ready())
				{
					//TODO: misc task
				}
				badRead = (RS485_status != 4);
			}while(badRead && retryCntr);
			return;			//should do something smarter if there's a problem?
		}
		else
			return;		
	}	
	else if	(enquedCMDs[item].CMD == 8)		//read freezeframe
	{
		//TODO: code me!
	}	
	else if	(enquedCMDs[item].CMD == 9 && enquedCMDs[item].length == 0)		//read system health
	{
		//TODO: code me!
	}	
	else if	(enquedCMDs[item].CMD == 100 && enquedCMDs[item].length == 0)		//identify systen
	{
		//TODO: code me!
	}	
	else if	(enquedCMDs[item].CMD == 101)		//slave passthrough	
	{
		if((BPSstate != 0) && !misc_isDebug())	//BPS must not be measuring and must have debug turned on
			return;
		retryCntr = reg_read(1,10);
		badRead = 0;
		do				//loops around trying to talk
		{
			if(badRead)						//if we're looping back around...
				retryCntr--;
			RS485_slave = enquedCMDs[item].data[1];
			RS485_command = enquedCMDs[item].data[2];
			RS485_outLength = enquedCMDs[item].data[0];
			if(enquedCMDs[item].data[0] > 252)					//packet payload is to long
				return;
			for(temp = 0; temp < enquedCMDs[item].data[0]; temp++)		//copy over packet payload
				RS485_outData[temp] = enquedCMDs[item].data[temp+3];
			RS485_sendPacket(1);
			while(!RS485_ready())
			{
				//TODO: misc task
			}
			badRead = (RS485_status != 4);
		}while(badRead && retryCntr);
		if(badRead)
			return;			//should do something smarter if there's a problem?
		if(RS485_inLength > 252)					//output packet payload is to long
			return;
		while(TX_sendingBatt);
		RS422_outLength = RS485_inLength+3;
		RS422_outResponse = 101;
		RS422_outData[0] = RS485_inLength;
		RS422_outData[1] = RS485_slave;
		RS422_outData[2] = RS485_response;
		for(temp = 0; temp < enquedCMDs[item].data[0]; temp++)		//copy over packet payload
			RS422_outData[temp+3] = RS485_inData[temp];
		RS422_sendPacket();
	}	
	else if	(enquedCMDs[item].CMD == 102 && enquedCMDs[item].length == 0)		//read RTC
	{
		ttemp = RTC_getTime();
		while(TX_sendingBatt);
		RS422_outLength = 6;
		RS422_outResponse = 102;
		RS422_outData[0] = ttemp.year;
		RS422_outData[1] = ttemp.month;
		RS422_outData[2] = ttemp.day;
		RS422_outData[3] = ttemp.hour;
		RS422_outData[4] = ttemp.minute;
		RS422_outData[5] = ttemp.seconds;
		RS422_sendPacket();
	}
	else if	(enquedCMDs[item].CMD == 103 && enquedCMDs[item].length == 6)		//write RTC
	{
		ttemp.year = 	enquedCMDs[item].data[0];
		ttemp.month = 	enquedCMDs[item].data[1];
		ttemp.day = 	enquedCMDs[item].data[2];
		ttemp.hour = 	enquedCMDs[item].data[3];
		ttemp.minute = 	enquedCMDs[item].data[4];
		ttemp.seconds = enquedCMDs[item].data[5];
		RTC_setTime(ttemp);
	}		
}	
	
	
//newline
