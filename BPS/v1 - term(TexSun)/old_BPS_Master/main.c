#include <p30Fxxxx.h>
//#include "RS485M.h"


unsigned int VoltBuffer[11];
unsigned int VoltBuffer2[11];
unsigned int TempBuffer[11];
unsigned int TempBuffer2[11];
signed int CurrentBuffer[2];
unsigned char SlaveStatus[16][4];
unsigned char number_of_slaves = 0;
unsigned char initialized = 0;
unsigned char ERRORS = 0;
unsigned char ERROR_COUNT= 0;

void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt( void ) {
  unsigned char MSB = 0, 
       LSB = 0;
       //slave_address = 0,
       //number_of_modules = 0,
       //modules_set = 0;
  unsigned int  i = 0,
                j = 0,
                error_modules = 0;
       //voltage = 0,
       //temperature = 0;
  unsigned char command = U2RXREG;
  while(!U2STAbits.RIDLE);
  unsigned char slave_address_module_number = U2RXREG;  
//  while(!U2STAbits.RIDLE);
//  unsigned char data1 = U2RXREG;
//  while(!U2STAbits.RIDLE);
//  unsigned char data2 = U2RXREG;
U2STAbits.OERR = 0;
  switch(command) {

//command = send number of slaves to gui
    case 253:
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send slave address and module number
      RS422_Send(0);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
      RS422_Send(number_of_slaves);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
      break;   

//command = send contactor status 
    case 252:
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send slave address and module number
      RS422_Send(0);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send ERROR STATUS
      RS422_Send(ERRORS);
      break;

//command = turn contactor off 
    case 251:
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send slave address and module number
      RS422_Send(0);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
      LATG &= ~0x8000;
      LATD &= ~0x200;
      RS422_Send(1);
      break;

//command = turn contactor on 
    case 250:
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send slave address and module number
      RS422_Send(0);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
      for(i = 0; i<number_of_slaves; i++){
        for(j = 1; j<4; j++){
          if(SlaveStatus[i][j]){
            error_modules++;
          }
        }
      }
//ERRORS PRESENT - contactor not turned on
      if(error_modules){
        RS422_Send(0);
      }
//NO ERRORS - contactor turned on
      else{
        LATG |= 0x8000;
        LATD |= 0x200;
        RS422_Send(1);
      }
      break;

//command = Send voltage to GUI 
    case 249:
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send slave address and module number
      RS422_Send(0);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send bytes of voltage data for slave 0   
      for(i=0;i<11;i++) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send MSB of data
        MSB = (VoltBuffer[i]&0xFF00)>>8;
        RS422_Send(MSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
//Send LSB of data
        LSB = VoltBuffer[i]&0x00FF;  
        RS422_Send(LSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
      }
//Send bytes of voltage data for slave 1    
      for(i=0;i<11;i++) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send MSB of data
        MSB = (VoltBuffer2[i]&0xFF00)>>8;
        RS422_Send(MSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
//Send LSB of data
        LSB = VoltBuffer2[i]&0x00FF;  
        RS422_Send(LSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
      }
      break;

//command = Send temperature to GUI  
    case 248:
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send slave address and module number
      RS422_Send(0);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send bytes of temperature data for slave 0   
      for(i=0;i<11;i++) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send MSB of data
        MSB = (TempBuffer[i]&0xFF00)>>8;
        RS422_Send(MSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
//Send LSB of data
        LSB = TempBuffer[i]&0x00FF;  
        RS422_Send(LSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
      }
//Send bytes of temperature data for slave 1   
      for(i=0;i<11;i++) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send MSB of data
        MSB = (TempBuffer2[i]&0xFF00)>>8;
        RS422_Send(MSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
//Send LSB of data
        LSB = TempBuffer2[i]&0x00FF;  
        RS422_Send(LSB);
//Wait for transmit to complete
        while(!U2STAbits.TRMT);
      }
      break;

//command = send bps current to GUI
      case 247:
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send command code
        RS422_Send(command);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(0x00);
        if( ((-5000<CurrentBuffer[0]) || (CurrentBuffer[0]<5000)) &&
            ((-5000<CurrentBuffer[1]) || (CurrentBuffer[1]<5000)) ) {
//Wait for previous transmit to complete
          while(!U2STAbits.TRMT);
//Send MSB of data
          MSB = (CurrentBuffer[1]&0xFF00)>>8;
          RS422_Send(MSB);
//Wait for transmit to complete
          while(!U2STAbits.TRMT);
//Send LSB of data
          LSB = CurrentBuffer[1]&0x00FF;  
          RS422_Send(LSB);
        }
        else {
//Wait for previous transmit to complete
          while(!U2STAbits.TRMT);
//Send MSB of data
          MSB = (CurrentBuffer[0]&0xFF00)>>8;
          RS422_Send(MSB);
//Wait for transmit to complete
          while(!U2STAbits.TRMT);
//Send LSB of data
          LSB = CurrentBuffer[0]&0x00FF;  
          RS422_Send(LSB);
        }
        break;
/*
//command = set overvoltage for module
    case 246:
      slave_address = (slave_address_module_number>>4)&0x0F; 
      voltage = data1<<8;
      voltage = voltage|data2;
      if(RS485M_SetOV(slave_address,voltage)) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send command code
        RS422_Send(command);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(slave_address_module_number);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }
      else {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(0xFF);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }  
      break; 

//command = set undervoltage for module
    case 245:
      slave_address = (slave_address_module_number>>4)&0x0F; 
      voltage = data1<<8;
      voltage = voltage|data2;
      if(RS485M_SetUV(slave_address,voltage)) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send command code
        RS422_Send(command);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(slave_address_module_number);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }
      else {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(0xFF);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }  
      break;  

//command = set overtemperature for module
    case 244:
      slave_address = (slave_address_module_number>>4)&0x0F; 
      temperature = data1<<8;
      temperature = temperature|data2;
      if(RS485M_SetOT(slave_address,temperature)) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send command code
        RS422_Send(command);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(slave_address_module_number);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }
      else {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(0xFF);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }  
      break;    
 
//command = set undertemperature for module
    case 243:
      slave_address = (slave_address_module_number>>4)&0x0F; 
      temperature = data1<<8;
      temperature = temperature|data2;
      if(RS485M_SetUT(slave_address,temperature)) {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send command code
        RS422_Send(command);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(slave_address_module_number);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }
      else {
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(0xFF);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      }  
      break;    

//command = set number of modules connected to slave
    case 242:
      slave_address = (slave_address_module_number>>4)&0x0F; 
      number_of_modules = data1;
      modules_set = RS485M_SetNumberOfModules(slave_address, number_of_modules);
      if(modules_set && (modules_set != 0xFF)){
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send command code
        RS422_Send(command);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
//Send slave address and module number
        RS422_Send(slave_address_module_number);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT);
      }
      else
        while(!U2STAbits.TRMT); 
        RS422_Send(0xFF);
//Wait for previous transmit to complete
        while(!U2STAbits.TRMT); 
      break;
*/
//command = acknowledge signature from GUI
    case 241:
      if(!initialized) {
      RS485M_Init();
      number_of_slaves = RS485M_DetectSlaves(SlaveStatus); 
      ADCM_InitScanInputs();
      initialized++;
      }
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(command);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(0x4C);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(0xC2);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(0xCC);
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);
//Send command code
      RS422_Send(0x0A); 
//Wait for previous transmit to complete
      while(!U2STAbits.TRMT);     
      break;
 
    default:
      while(!U2STAbits.TRMT);
      RS422_Send(0xFF);
      break;
  } 
//Clear the interrupt flag status bit associated with the 
//peripheral in the IFS0 Status register
//to signal that interrupt was handled
  IFS1bits.U2RXIF = 0; 
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt( void ) {
    unsigned char error_modules = 0,
                  command = 0,
                  slave_number = 0;
    unsigned int i = 0,
                 j = 1;
    slave_number = RS485M_DetectSlaves(SlaveStatus);
    while( (slave_number != number_of_slaves) && (ERROR_COUNT < 10)) {
      for(i=0;i<0xFF;i++){} 
      slave_number = RS485M_DetectSlaves(SlaveStatus);
      ERROR_COUNT++;
    }
    for(i = 0; i<number_of_slaves; i++){
      for(j = 1; j<4; j++){
        if(SlaveStatus[i][j]){
          error_modules++;
        }
      }
    }
    /*for(i = 0; i < number_of_slaves; i++) {
      command = i;
      command = command<<4;
      command = command + 2;
      RS485M_ReceiveVoltages(command, VoltBuffer, 11);
      command = command + 1;
      RS485M_ReceiveTemps(command, TempBuffer, 11); 
    }*/
//Get voltages from slave 0
    RS485M_ReceiveVoltages(0x02, VoltBuffer, 11);

//Get temperatures from slave 0
    RS485M_ReceiveVoltages(0x03, TempBuffer, 11);
    if(number_of_slaves > 1) {
//Get voltages from slave 1
    RS485M_ReceiveVoltages(0x12, VoltBuffer2, 11);

//Get temperatures from slave 1
    RS485M_ReceiveVoltages(0x13, TempBuffer2, 11);
    }
    ADCM_GetCurrents(CurrentBuffer);

    if(error_modules ||
       (CurrentBuffer[0]<-8000) ||
       (CurrentBuffer[0]>4000) ||
       (CurrentBuffer[1]<-8000) ||
       (CurrentBuffer[1]>4000) ||
       (ERROR_COUNT > 9) ) {
      ERRORS = 1;
      LATG = LATG&~0x8000;
      LATD = LATD&~0x200;
    }
    else{
      ERRORS = 0;
    }
    ERROR_COUNT = 0;
    if(PORTG&0x4000){
      LATG &= ~0x4000;
    }
    else{
      LATG |= 0x4000;
    }
//Clear the interrupt flag status bit associated with the 
//peripheral in the IFS0 Status register
//to signal that interrupt was handled
    IFS0bits.T1IF = 0;
}

void TimerInit(void) {
  //Load PR1 with the timer period
  PR1 = 0xFFFE;

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
  T1CON = 0xA030;//1010 0000 0010 0000 

//Specify the interrupt priority for the interrupt using the
//T1IP<2:0> control bits in the IPC0 (Interrupt Priority Control 2) register
  IPC0bits.T1IP = 5;

//Clear the interrupt flag status bit associated with the peripheral in the IFS0 Status register
  IFS0bits.T1IF = 0;

//Set the T1IE bit in the IEC0 (Interrupt Enable Control 0) register
  IEC0bits.T1IE = 1;
}

int main( void ) {
  unsigned int i = 0;
  //I2C_Init();
  RS422_Init();
  RS485M_Init();
  ADCM_InitScanInputs();
  for(i = 0;i<0xFFF;i++){}
  number_of_slaves = RS485M_DetectSlaves(SlaveStatus);
  for(i = 0;i<0xFFF;i++){}
  TimerInit();
  TRISG &= ~0xC000;
  TRISD &= ~0x200;
  for(;;){
    //buffer = I2C_WriteEEPROM((unsigned long)0, 0xAA);
    //buffer = I2C_ReadEEPROMRandom((unsigned long)0);
  }
}


