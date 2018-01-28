#include <p30Fxxxx.h>
#include "SPI.h"
#include "LTC6802.h"
//#include "ADC.h"

//Configuration register settings for the LTC6802
//See LTC6802.c for detailed comments
#define DEFAULT_CFGR0 0x91
#define DEFAULT_CFGR1 0x00
#define DEFAULT_CFGR2 0x00
#define DEFAULT_CFGR3 0x80
#define DEFAULT_CFGR4 125 //default Lower Voltage = 3.000V
#define DEFAULT_CFGR5 175 //default Upper Voltage = 4.200V

unsigned int VoltBufferA[11];
unsigned int VoltBufferB[11];
unsigned int TempBufferA[11] = {0,0,0,0,0,0,0,0,0,0,0};
unsigned int OverTemp = 3176; //default upper temp = 317.6K
unsigned int UnderTemp = 2736; //default lower temp = 273.6K
unsigned int OverVoltage = 4213; //default upper voltage = 4.200V
unsigned int UnderVoltage = 3000; //default lower voltage = 3.000V
unsigned int ErrorFlags = 0;
char Volt12to5Status = 0;
char Volt4to1Temp12to9Status = 0;
char Temp8to1Status = 0;
char CFGR0 = DEFAULT_CFGR0;
char CFGR1 = DEFAULT_CFGR1;
char CFGR2 = DEFAULT_CFGR2;
char CFGR3 = DEFAULT_CFGR3;
char CFGR4 = DEFAULT_CFGR4;
char CFGR5 = DEFAULT_CFGR5;
char SLAVE_STATUS = 0x8F;
char NUMBER_OF_MODULES = 11;
char BUFFER1;
char BUFFER2;
char BUFFER3;
char BUFFER4;
unsigned char TestArray[18];

void Timer1Init(void) {
//Load PR1 with the timer period
  PR1 = 0xFFF;

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
  T1CON = 0xA030;//1010 0000 0001 0000 

//Specify the interrupt priority for the interrupt using the
//T1IP<2:0> control bits in the IPC0 (Interrupt Priority Control 2) register
  IPC0bits.T1IP = 4;

//Clear the interrupt flag status bit associated with the peripheral in the IFS0 Status register
  IFS0bits.T1IF = 0;

//Set the T1IE bit in the IEC0 (Interrupt Enable Control 0) register
  IEC0bits.T1IE = 1;
}

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt( void ) {
  char i = 0,
       j = 0, 
       hw_address = (PORTD>>2)&0x0F,
       address = 0,
       command = 0,       
       MSB   = 0, 
       LSB   = 0,
       data1 = 0,
       data2 = 0,
       data3 = 0,
       PEC1 = 0,
       PEC2 = 0,
       PEC3 =0;
  int test = 0;
//Receive PEC's
  PEC1 = U1RXREG;
  while(!U1STAbits.RIDLE);
  PEC2 = U1RXREG;
  while(!U1STAbits.RIDLE);
//Determine address of slave sent in master transmit
  address = U1RXREG;
  while(!U1STAbits.RIDLE);
  PEC3 = U1RXREG;
  while(!U1STAbits.RIDLE);
  data1 = U1RXREG;
  while(!U1STAbits.RIDLE);
  data2 = U1RXREG;
  while(!U1STAbits.RIDLE);
  data3 = U1RXREG;
  command = address&0x0F;
  address = address>>4;
  address &= 0x0F;
  if( (hw_address == address) && 
      (command < 0x09) && 
      (command > 0) &&
      (PEC1 == 0x53) &&
      (PEC2 == 0x54) &&
      (PEC3 == 0x50) ) {
//Turn off /RE (PORTD<1>) and turn on TE (PORTD<0>) for slave transfer
    for(;i<0x7A;i++){}
    i = 0;
    LATD |= 0x003; 
    switch ( command ) {

//command = Initialize/Detect Slave
      case 1:
        if(!(SLAVE_STATUS&0x40)) {
          ADCS_InitScanInputs();
          SPI_Init();
          LTC6802_ConfigRegWrite(CFGR0, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5);
          LTC6802_SelfTest1(TestArray);
          SLAVE_STATUS = SLAVE_STATUS|0x40;
          SLAVE_STATUS = SLAVE_STATUS&0xF0;
          Timer1Init();
        }
        RS485S_Send(SLAVE_STATUS);
		RS485S_Send(Volt12to5Status);
		RS485S_Send(Volt4to1Temp12to9Status);
        RS485S_Send(Temp8to1Status);
        break;

//command = Get Voltage, All
      case 2:
         /*while(!U1STAbits.TRMT);
        RS485S_Send(SLAVE_STATUS);
          while(!U1STAbits.TRMT);
		RS485S_Send(Volt12to5Status);
          while(!U1STAbits.TRMT);
		RS485S_Send(Volt4to1Temp12to9Status);
          while(!U1STAbits.TRMT);
        RS485S_Send(Temp8to1Status);
          while(!U1STAbits.TRMT);*/
        for(i=0;i<11;i++) {
//Send MSB of Volt data
          while(!U1STAbits.TRMT); 
          RS485S_Send((VoltBufferA[i]&0xFF00)>>8);
//Send LSB of Volt data
          while(!U1STAbits.TRMT); 
          RS485S_Send(VoltBufferA[i]&0x00FF);
        }
        break;

//command = Get Temperature, All
      case 3:
         /* while(!U1STAbits.TRMT);
        RS485S_Send(SLAVE_STATUS);
          while(!U1STAbits.TRMT);
		RS485S_Send(Volt12to5Status);
          while(!U1STAbits.TRMT);
		RS485S_Send(Volt4to1Temp12to9Status);
          while(!U1STAbits.TRMT);
        RS485S_Send(Temp8to1Status);
          while(!U1STAbits.TRMT);*/
        for(i=0;i<11;i++) {
//Send MSB of Temp data
          while(!U1STAbits.TRMT); 
          MSB = (TempBufferA[i]>>8);
          RS485S_Send(MSB);

//Send LSB of Temp data
          while(!U1STAbits.TRMT);
          LSB = TempBufferA[i]&0x00FF;
          RS485S_Send(LSB);
        }
        break;

//command = set over voltage
      case 4:
        if(LTC6802_SetOV(CFGR0, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5, data1)) {
          SLAVE_STATUS = SLAVE_STATUS&~0x01;
          OverVoltage = data1&0x00FF;
          OverVoltage = (OverVoltage*48)/2;
        }
        else {
          SLAVE_STATUS = SLAVE_STATUS|0x01;
        }
//Send Slave Status     
        RS485S_Send(SLAVE_STATUS);
        while(!U1STAbits.TRMT);
        U1STAbits.OERR = 0;        
        break;

//command = set under voltage
      case 5:
        if(LTC6802_SetUV(CFGR0, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5, data1)) {
          SLAVE_STATUS = SLAVE_STATUS&~0x02;
          UnderVoltage = data1&0x00FF;
          UnderVoltage = (UnderVoltage*48)/2;
		}
        else {
          SLAVE_STATUS = SLAVE_STATUS|0x02;
        }
//Send Slave Status
        RS485S_Send(SLAVE_STATUS); 
        while(!U1STAbits.TRMT);  
        U1STAbits.OERR = 0;     
        break;

//command = set over temperature
      case 6:
        OverTemp = (data1<<8)&0xFF00;
        OverTemp = OverTemp|(data2&0xFF);
        SLAVE_STATUS = SLAVE_STATUS&~0x04;
//Send Slave Status
        RS485S_Send(SLAVE_STATUS);
        U1STAbits.OERR = 0;       
        break;

//command = set under temperature
      case 7:
        UnderTemp = (data1<<8)&0xFF00;
        UnderTemp = UnderTemp|(data2&0xFF);
        SLAVE_STATUS = SLAVE_STATUS&~0x08;
//Send Slave Status
        RS485S_Send(SLAVE_STATUS);        
        U1STAbits.OERR = 0;
        break;

//command = set number of modules
      case 8:
        if(data1 < 11) {
          CFGR0 = CFGR0|0x08; 
        }
        if(LTC6802_SetNumberOfModules(CFGR0, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5, data1)){
          NUMBER_OF_MODULES = data1;
          RS485S_Send(SLAVE_STATUS);  
        }
        else {
          RS485S_Send(0xFF);
        }
        U1STAbits.OERR = 0;
        break;

      default:
		//RS485S_Send(0xFF);
        U1STAbits.OERR = 0;
        break;
    }

    i = 0;
    for(;i<0x1A;i++){}
//Turn on /RE (PORTD<1>) and turn off TE (PORTD<0>) for slave receive
    LATD &= 0xFFC;  
  } 
  for(i=0; i<4;i++) {
    data1 = U1RXREG;
  }
  U1STAbits.OERR = 0;
//Clear the interrupt flag status bit associated with the peripheral in the IFS0 Status register
//to signal that interrupt was handled
  IFS0bits.U1RXIF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt( void ) {
  unsigned int i = 0;
  if(SLAVE_STATUS&0x40) {

    LTC6802_ConfigRegWrite(0x90, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5);   
    LTC6802_ConfigRegWrite(0x91, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5);  
    LTC6802_StartConversions();
    for(i=0;i<10000;i++){}
    LTC6802_ReadVoltages();
    LTC6802_SortVoltageArray(VoltBufferA);
    LTC6802_ConfigRegWrite(0x90, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5);   
    LTC6802_ConfigRegWrite(0x91, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5); 
    LTC6802_StartOWConversions();
    for(i=0;i<10000;i++){}
    LTC6802_ReadOWVoltages();
    LTC6802_SortOWVoltageArray(VoltBufferB);
    LTC6802_ConfigRegWrite(0x90, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5);   
    LTC6802_ConfigRegWrite(0x91, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5); 
    for(i=0;i<11;i++){
      if( (VoltBufferA[i]<UnderVoltage) || (VoltBufferA[i]>OverVoltage) ) {
        ErrorFlags = ErrorFlags|(0x0001<<i);
      }
      else {
        ErrorFlags = ErrorFlags&(~(0x0001<<i));
      }
      if((i < 10) && (((signed int)(VoltBufferB[i+1] - VoltBufferA[i+1])) >= 200)) {
        ErrorFlags = ErrorFlags|(0x0001<<i);
      }
    }
    Volt12to5Status = ErrorFlags>>4;
    Volt4to1Temp12to9Status = (ErrorFlags&0x000F)<<4;
    ADCS_ScanInputs(TempBufferA);
    for(i=0;i<11;i++){
      if( (TempBufferA[i]<UnderTemp) || (TempBufferA[i]>OverTemp) ) {
        ErrorFlags = ErrorFlags|(0x0001<<i);
      }
      else {
        ErrorFlags = ErrorFlags&(~(0x0001<<i));
      }
    }
    Volt4to1Temp12to9Status = Volt4to1Temp12to9Status|((ErrorFlags&0x0F00)>>8);
    Temp8to1Status = ErrorFlags&0x00FF;
  }
//Clear the interrupt flag status bit associated with the peripheral in the IFS0 Status register
  IFS0bits.T1IF = 0;
}

int main(void) {

  RS485S_Init();
  for(;;){
  }
}

