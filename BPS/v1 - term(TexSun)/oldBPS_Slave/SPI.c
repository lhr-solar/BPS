//***************************************************
//dsPIC30F6012A SPI Driver Code
//Archer Finley, Cory Loflin
//BPS-UTSVT
//February 15, 2010
//Description: Drivers for the SPI2 module on the dsPIC30F6012A
//Filename: SPI.c

#include <p30Fxxxx.h>

char SPIReadArray[20];

//*********SPI_Init***********
//Description: Initializes SPI2 port for communicaiton with LTC6802 voltage board
//Inputs: none
//Outputs: none
void SPI_Init(void) {

  unsigned char dummy;

//Sets the configuration for SPI communication in register SPI2CON
// <bit#>    NAME   VALUE    DESCRIPTION
//  <14>    FRMEN     0    no FramedSPI support
//  <13>    SPISFD    0    /SS2 pin Frame Sync Pulse output
//  <11>    DISSDO    0    SDO2 controlled by module
//  <10>    MODE16    0    8-bit communication
//  <9>     SMP       1    input sampled at end of output
//  <8>     CKE       0    output changes from idle to active clock
//  <7>     SSEN      0    using master mode
//  <6>     CKP       1    idle = high, active = low
//  <5>     MSTEN     1    master mode
//  <4:2>   SPRE     101   secondary prescale = 3:1
//  <1:0>   PPRE      11   primary prescale = 1:1
//
// TOTAL VALUE GOING INTO SPI2CON = 0000 0010 0111 0111 = 0x0277
  SPI2CON = 0x0277;

//Clears the SPIROV bit (SPI2STAT<6>) by reading the SPI buffer
  dummy = SPI2BUF;

//Enables SPI operation by setting the SPIEN bit (SPI2STAT<15>).
//Continues operation while in idle mode by setting the SPISIDL bit (SPI2STAT<13>)    
  SPI2STAT = 0xA000;

//Specify the interrupt priority for the interrupt using the
//T1IP<2:0> control bits in the IPC0 (Interrupt Priority Control 2) register
  IPC0bits.T1IP = 4;

//Clear the interrupt flag status bit associated with the peripheral in the IFS0 Status register
  IFS0bits.T1IF = 0;

//Load PR1 with the timer period
  PR1 = 0xFFFF;

//Set T1CON options for Timer 1
// <15>  TON   = 1 Starts the timer
// <13>  TSIDL = 1 Discontinue timer operation when device enters Idle mode
// <6>   TGATE = 0 Gated time accumulation disabled
// <5:4> TCKPS = 00 Timer Input Clock Prescale Select bits
//               11 = 1:256 prescale value
//               10 = 1:64 prescale value
//               01 = 1:8 prescale value
//               00 = 1:1 prescale value
// <2>   TSYNC = When TCS = 0, this bit is ignored. Read as ‘0’. Timer1 uses the internal clock when TCS = 0.
// <1>   TCS   = 0 Internal clock (FOSC/4)
  T1CON = 0xA000;//1010 0000 0000 0000 

//Set the T1IE bit in the IEC0 (Interrupt Enable Control 0) register
  IEC0bits.T1IE = 1;

//Sets PORTG<9> as an output for CS active low
  TRISG &= ~0x0200;
}

//*********SPI_SendReceive*********
//Description: Sends 8 bits (data) through SDO2 and receives 8 bits through SDI2
//Inputs: 8 bits (char data) to be sent through SDO2
//Outputs: 8 bits (char data) received from SDI2
char SPI_SendReceive(char data){
  //busy-wait for empty TxBuf (SPI2BUF)
  while(SPI2STAT&0x0002){}
  
  //write 8 bits to TxBuf
  SPI2BUF = data; 

  //busy-wait for RxBuf to be full	
  while(!(SPI2STAT&0x0001)){} 
  
  //read 8 bits from RxBuf
  data = SPI2BUF; 

  return data;
}

//**********SPI_Receive************
//Description: Sends nothing(0x00) through SDO2 and receives 
// 8 bits through SDI2
//Inputs: none
//Outputs: 8 bits (char data) received from SDI2
char SPI_Receive(void){
  char data;

  //busy-wait for empty TxBuf (SPI2BUF)
  while(SPI2STAT&0x0002){}
  
  //write 8 bits of nothing to TxBuf
  SPI2BUF = 0x00; 

  //busy-wait for RxBuf to be full
  while(!(SPI2STAT&0x0001)){} 
  
  //read 8 bits from RxBuf
  data = SPI2BUF;
  
  return data;
}

//**********SPI_ReceiveArray**********
//Description: Receives a variable length array set by
// the input parameters.
//Inputs: char pointer to the Array to fill, unsigned 
// short length of input Array
//Outputs: none 
void SPI_ReceiveArray(char *pt, unsigned int length){

  while(length--){
    *pt = SPI_Receive();
    pt++;
  }
}
  

//**********SPI_Send*************
//Description: Sends 8 bits (data) through SDO2
//Inputs: 8 bits (char data) sent through SDO2
//Outputs: Dont Care (char data) received through SDI2
void SPI_Send(char data) {
  // busy-wait for empty TxBuf and RxBuf (SPI2BUF)
  while(SPI2STAT&0x0002){}
  SPI2BUF = data; // write 8 bits to TxBuf
	
  while(!(SPI2STAT&0x0001)){} // busy-wait for RxBuf to be full
  data = SPI2BUF; // read 8 bits from RxBuf
	
}

//***********SPI_slave_select_active_low*********
//Description: Sets PORTG<9> to low for SPI CS active low
//Inputs: none
//Outputs: none
void SPI_SlaveSelectActiveLow( void ) {
  PORTG &= ~0x0200;
}

//***********SPI_slave_select_off_high***********
//Description: Sets PORTG<9> to high for SPI CS off high
//Inputs: none
//Outputs: none
void SPI_SlaveSelectOffHigh(void) {
  PORTG |= 0x0200;
}
	

	
	
	
	
	
	


	
	


