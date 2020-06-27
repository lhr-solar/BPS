//***************************************************
//dsPIC30F6012A RS422 Master board Driver Source Code File
//Connor Koen, Archer Finley 
//BPS-UTSVT
//April 7, 2010
//Description: Source code for drivers for the RS422 module
//on the dsPIC30F6012A(Master Board)
//Filename: RS422.c

#include <p30Fxxxx.h>

//*********RS22_Init***********
//Description: Initializes master board dsPIC for 
//communication with the GUI using UART and RS-422
//communication protocol
//Inputs: none
//Outputs: none
void RS422_Init(void){

//Set U2MODE (UART2 Mode Register) options 
// <15>  UARTEN = 1  UART Enable bit
// <13>  USIDL  = 0  Continue operation in Idle mode
// <10>  ALTIO  = 0  UART communicates using UxTX and UxRX I/O pins
// <7>   WAKE   = 1  Wake-up on Start bit Detect During Sleep Mode enabled
// <6>   LPBACK = 0  Loopback mode is disabled
// <5>   ABAUD  = 1  Auto Baud Enable bit, Input to Capture module from UxRX pin
// <2:1> PDSEL  = 00 8-bit data, no parity
// <0>   STSEL  = 1  2 Stop bits
// U2MODE = 1000 0000 1010 0001
  U2MODE = 0x80A1;

//Set U2STA (UART2 Status and Control Register) options
// <15>  UTXISEL = 1  Interrupt when a character is transferred to the Transmit Shift 
//                    register and as result, the transmit buffer becomes empty
// <11>  UTXBRK  = 0  U2TX pin operates normally
// <10>  UTXEN   = 1  UART transmitter enabled, U2TX pin controlled by UART (if UARTEN = 1)
// <7:6> URXISEL = 00 Interrupt flag bit is set when Receive Buffer is full (i.e., has 4 data characters)
// <5>   ADDEN   = 0  Address Detect mode disabled. If 9-bit mode is not selected, this control bit has no effect.
// U2STA = 1000 0100 0000 0000
  U2STA = 0x8400;

//Set U2BRG (UART2 Baud Rate Register) options
// U2BRG = (Fcy/(16*BaudRate)) - 1 = 0 for 625 kbaud
  U2BRG = 520;

//
  INTCON1 = 0x8000;

  INTCON2 = 0x4000;

//Specify the interrupt priority for the interrupt using the
//U2RXIP<2:0> control bits in the IPC2 (Interrupt Priority Control 2) register
  IPC6bits.U2RXIP = 4;

//Clear the interrupt flag status bit associated with 
//the peripheral in the IFS0 Status register
  IFS1bits.U2RXIF = 0;

//Set the U2RXIE bit in the IEC0 (Interrupt Enable Control 0) register
  IEC1bits.U2RXIE = 1;
}

//*********RS422_Send***********
//Description: Sends a byte using UART2 to transmit data via 
//RS-422 protocol. Waits for the transmit register to be empty
//before sending the data
//Inputs: 1 byte of char data
//Outputs: none
void RS422_Send(unsigned char data) {
  while(!U2STAbits.TRMT);
  U2TXREG = data;
  while(!U2STAbits.TRMT);

}

