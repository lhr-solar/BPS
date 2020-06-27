//***************************************************
//LTC6802 Driver Code
//Archer Finley, Cory Loflin
//BPS-UTSVT
//February 15, 2010
//Description: Drivers for the LTC6802 Multicell Battery Stack Monitor
//Filename: LTC6802.c

#include <p30Fxxxx.h>
#include "SPI.h"

#define WRCFG 0x01

unsigned char PEC;
char ReadVoltageArrayA[18];
char ReadVoltageArrayB[18];
char ReadFlagArray[3];
char ConfigReg[6];

//***************LTC6802_ConfigRegRead************
//Description: Uses SPI.c to read the LTC6802 Configuration
// Register after sending empty (0x00) bytes.
//Inputs: 6 byte char array (sends 0x00 as the temp SPI2BUF)
//Outputs: none
void LTC6802_ConfigRegRead(char ConfigReg[]){

  //Set SPI CS to active low
  SPI_SlaveSelectActiveLow();

  //Send Configuration Register Read Command to LTC6802
  SPI_Send(0x02);

  //Send nothing (0x00), read CFGR0-6, and put in ConfigReg[] 
  SPI_ReceiveArray(ConfigReg, 6);

  //Set SPI CS to off high
  SPI_SlaveSelectOffHigh();

}

//***********LTC6802_ConfigRegWrite**********
//Description: Uses SPI.c to write to the Configuration Register of the LTC6802 (7 bytes: 1 byte command and 6 bytes of data)
//Inputs:  REGISTER              DESCRIPTION
//           CFGR0       <7>WDTEN..........Watchdog Timer Enable
//                            Write: 0=watchdog timer disabled; 1=watchdog timer enabled (default)
//                            Read:  0=WDTB pin at logic ‘0’; 1=WDTB pin at logic ‘1’
//
//                       <6:5>GPIO2-GPIO1..GPIO Pin Control
//                            Write: 0=GPIOx pin pull down on; 1=GPIOx pin pull down off (default)
//                            Read:  0=GPIOx pin at logic ‘0’; 1=GPIOx pin at logic ‘1’
//
//                       <4>LVLPL..........Level Polling Mode
//                            0=toggle polling (default); 1=level polling
//
//                       <3>CELL10.........10-Cell Mode
//                            0=12-cell mode (default); 1=10-cell mode
//
//                       <2>CDC2-0.........Comparator Duty Cycle
//                            CDC               PERIOD                      BETWEEN MEASUREMENTS   MEASUREMENT TIME
//                         0(default)   N/A (Comparator Off)Standby Mode            Yes                 N/A
//                             1        N/A (Comparator Off)                        No                 13ms
//                             2             13ms                                   No                 13ms
//                             3             130ms                                  No                 13ms
//                             4             500ms                                  No                 13ms
//                             5*            130ms                                  Yes                21ms
//                             6             500ms                                  Yes                21ms
//                             7             2000ms                                 Yes                21ms
//                             *Voltage determinations use the decimal value of the registers, 0 to 4095 for 12-bit and 0 to 255 for 8-bit registers
//  
//           CFGR1       <7:0>DCC8-1.......x=1..12 0=turn off shorting switch for cell ‘x’ (default); 1=turn on shorting switch
//
//           CFGR2       <7:4>MC4I-MC1I....Mask Cell x Interrupts
//                            x=1..12 0=enable interrupts for cell ‘x’ (default)
//                            1=turn off interrupts and clear flags for cell ‘x’
//                       <3:0>DCC12-DCC8...Discharge Cell x
//                            x=1..12 0=turn off shorting switch for cell ‘x’ (default); 1=turn on shorting switch
//     
//           CFGR3       <7:0>MC12I-MC5I...Mask Cell x Interrupts
//                            x=1..12 0=enable interrupts for cell ‘x’ (default)
//                            1=turn off interrupts and clear flags for cell ‘x’
//
//           CFGR4       <7:0>VUV7-VUV0....Undervoltage Comparison Voltage
//                            Comparison voltage = VUV * 16 * 1.5mV
//                            (default VUV=0. When MMB pin is low a factory programmed comparison voltage is used)
//
//           CFGR5       <7:0>VOV7-VOV0....Overvoltage Comparison Voltage
//                            Comparison voltage = VOV * 16 * 1.5mV
//                            (default VOV=0. When MMB pin is low a factory programmed comparison voltage is used)
//Outputs: 1 if successful, 0 if unsuccessful
char LTC6802_ConfigRegWrite(char CFGR0, char CFGR1, char CFGR2, char CFGR3, char CFGR4, char CFGR5){

  //Set SPI CS to active low
  SPI_SlaveSelectActiveLow();
  
  //Send Write Configuration Register Command
  SPI_Send(WRCFG);

  //Send Configuration Register 0 Data
  SPI_Send(CFGR0);

  //Send Configuration Register 1 Data
  SPI_Send(CFGR1);

  //Send Configuration Register 2 Data
  SPI_Send(CFGR2);

  //Send Configuration Register 3 Data  
  SPI_Send(CFGR3);

  //Send Configuration Register 4 Data
  // Vcomp=V*16*1.5mV
  SPI_Send(CFGR4);

  //Send Configuration Register 5 Data
  // 6V=250*16*1.5mV
  SPI_Send(CFGR5); 

  //Set SPI CS to off high
  SPI_SlaveSelectOffHigh();
  LTC6802_ConfigRegRead(ConfigReg);
  if( (CFGR0==ConfigReg[0]) &&
      (CFGR1==ConfigReg[1]) &&
      (CFGR2==ConfigReg[2]) &&
      (CFGR3==ConfigReg[3]) &&
      (CFGR4==ConfigReg[4]) &&
      (CFGR5==ConfigReg[5])) {
    return 1; 
  }
  else {
    return 0;
  }
}



//***************LTC6802_StartConversions***********
//Description: Uses SPI.c to send a Start Conversion 
// command to the LTC6802 for all 12 cells
//Inputs: none
//Outputs: none 
void LTC6802_StartConversions(void){

  SPI_SlaveSelectActiveLow();
  SPI_Send(0x10); // STCVAD, all 12 cells
  while(SPI2BUF != 0xFF){} // busy-wait until SDO is pulled high, LVLPL=1
  SPI_SlaveSelectOffHigh();

}

//***************LTC6802_StartOWConversions***********
//Description: Uses SPI.c to send a Start OW Conversion 
// command to the LTC6802 for all 12 cells
//Inputs: none
//Outputs: none 
void LTC6802_StartOWConversions(void){

  SPI_SlaveSelectActiveLow();
  SPI_Send(0x20); // STOWAD, all 12 cells
  while(SPI2BUF != 0xFF){} // busy-wait until SDO is pulled high, LVLPL=1
  SPI_SlaveSelectOffHigh();

}

//***************LTC6802_ReadVoltages****************
//Description: Reads voltage registers from the LTC6802. This 
// should take approx 464us with 1MHz SCK.
//Inputs: none
//Outputs: none
void LTC6802_ReadVoltages(void){

  SPI_SlaveSelectActiveLow();
  SPI_Send(0x04); // RDCV, read voltage registers
  SPI_ReceiveArray(ReadVoltageArrayA,18);
  PEC = SPI_Receive(); // read PEC byte
  SPI_SlaveSelectOffHigh();

}

//***************LTC6802_ReadOWVoltages****************
//Description: Reads voltage registers from the LTC6802. This 
// should take approx 464us with 1MHz SCK.
//Inputs: none
//Outputs: none
void LTC6802_ReadOWVoltages(void){

  SPI_SlaveSelectActiveLow();
  SPI_Send(0x04); // RDCV, read voltage registers
  SPI_ReceiveArray(ReadVoltageArrayB,18);
  PEC = SPI_Receive(); // read PEC byte
  SPI_SlaveSelectOffHigh();

}

//***************LTC6802_ReadFlagRegisters****************
//Description: Reads flags registers from the LTC6802. 
//Inputs: none
//Outputs: none
void LTC6802_ReadFlagRegisters(void){
 
  SPI_SlaveSelectActiveLow();
  SPI_Send(0x06); // RDFLG, read flag registers
  SPI_ReceiveArray(ReadFlagArray,3);
  PEC = SPI_Receive(); // read PEC byte
  SPI_SlaveSelectOffHigh();

}

//************LTC6802_SortVoltageArray*************
//Description: Sorts the ReadVoltageArray[17] (char) 
//from the LTC_6802 filled by LTC6802_ReadVoltages 
//into Voltages[12] (char)
//Inputs: int Voltages[12] array to store data
//Outputs: none 
void  LTC6802_SortVoltageArray(unsigned int Voltages[]){
  int i;
  int j;
  int buffer;
  //int Voltages[17];
  for(j=0, i=1; i<17; ){
    Voltages[j] = (int)ReadVoltageArrayA[i-1];
    Voltages[j] &= 0x00FF;
    buffer = (int)ReadVoltageArrayA[i];
    buffer &= 0x0F;
    buffer = buffer<<8;
    Voltages[j] += buffer;
    Voltages[j] = (Voltages[j]*3)/2;
    Voltages[j+1] = (int)ReadVoltageArrayA[i];
    Voltages[j+1] &= 0xF0;
    Voltages[j+1] = Voltages[j+1]>>4;
    Voltages[j+1] += (int)ReadVoltageArrayA[i+1]<<4;
    Voltages[j+1] &= 0x0FFF;
    Voltages[j+1] = (Voltages[j+1]*3)/2;
    i +=3;
    j +=2;
  }
}

//************LTC6802_SortOWVoltageArray*************
//Description: Sorts the ReadVoltageArray[17] (char) 
//from the LTC_6802 filled by LTC6802_ReadVoltages 
//into Voltages[12] (char)
//Inputs: int Voltages[12] array to store data
//Outputs: none 
void  LTC6802_SortOWVoltageArray(unsigned int Voltages[]){
  int i;
  int j;
  int buffer;
  //int Voltages[17];
  for(j=0, i=1; i<17; ){
    Voltages[j] = (int)ReadVoltageArrayB[i-1];
    Voltages[j] &= 0x00FF;
    buffer = (int)ReadVoltageArrayB[i];
    buffer &= 0x0F;
    buffer = buffer<<8;
    Voltages[j] += buffer;
    Voltages[j] = (Voltages[j]*3)/2;
    Voltages[j+1] = (int)ReadVoltageArrayB[i];
    Voltages[j+1] &= 0xF0;
    Voltages[j+1] = Voltages[j+1]>>4;
    Voltages[j+1] += (int)ReadVoltageArrayB[i+1]<<4;
    Voltages[j+1] &= 0x0FFF;
    Voltages[j+1] = (Voltages[j+1]*3)/2;
    i +=3;
    j +=2;
  }
}

//************LTC6802_SetOV**********
//Description: Sets the overvoltage limit on the LTC6802
//by writing to CFGR5
//Inputs
char LTC6802_SetOV(char CFGR0, char CFGR1, char CFGR2, char CFGR3, char CFGR4, char CFGR5, char data1) { 
  LTC6802_ConfigRegWrite(CFGR0, CFGR1, CFGR2, CFGR3, CFGR4, data1);
  LTC6802_ConfigRegRead(ConfigReg);
  if( (CFGR0==ConfigReg[0]) &&
      (CFGR1==ConfigReg[1]) &&
      (CFGR2==ConfigReg[2]) &&
      (CFGR3==ConfigReg[3]) &&
      (CFGR4==ConfigReg[4]) &&
      (data1==ConfigReg[5]))
    return 1;
  else
    return 0;
}

//************LTC6802_SetUV**********
//Description: Sets the undervoltage limit on the LTC6802
//by writing to CFGR4
//Inputs:
//Outputs:
char LTC6802_SetUV(char CFGR0, char CFGR1, char CFGR2, char CFGR3, char CFGR4, char CFGR5, char data1) {
  LTC6802_ConfigRegWrite(CFGR0, CFGR1, CFGR2, CFGR3, data1, CFGR5);
  LTC6802_ConfigRegRead(ConfigReg);
  if( (CFGR0==ConfigReg[0]) &&
      (CFGR1==ConfigReg[1]) &&
      (CFGR2==ConfigReg[2]) &&
      (CFGR3==ConfigReg[3]) &&
      (data1==ConfigReg[4]) &&
      (CFGR5==ConfigReg[5]))
    return 1;
  else
    return 0;
}

//******LTC6802_SetNumberOfModules*****
//Description: Sets the number of modules measured by 
//the LTC6802
//Inputs:
//Outputs:
char LTC6802_SetNumberOfModules(char CFGR0, char CFGR1, char CFGR2, char CFGR3, char CFGR4, char CFGR5, char amount) {
  if(amount < 11) {
    CFGR0 = CFGR0|0x08; 
  }
  LTC6802_ConfigRegWrite(CFGR0, CFGR1, CFGR2, CFGR3, CFGR4, CFGR5);
  LTC6802_ConfigRegRead(ConfigReg);
  if( (CFGR0==ConfigReg[0]) &&
      (CFGR1==ConfigReg[1]) &&
      (CFGR2==ConfigReg[2]) &&
      (CFGR3==ConfigReg[3]) &&
      (CFGR4==ConfigReg[4]) &&
      (CFGR5==ConfigReg[5]))
    return 1;
  else
    return 0;
}

//***************LTC6802_SelfTest1****************
//Description: Tests ADC on the LTC6802. 
//Inputs: none
//Outputs: none
void LTC6802_SelfTest1(unsigned char TestArray[]){
  int i=0; 
  SPI_SlaveSelectActiveLow();
  SPI_Send(0x1E); // Self test
  SPI_SlaveSelectOffHigh();
  for(i=0;i<8000;i++){}
  SPI_SlaveSelectActiveLow();
  SPI_Send(0x04); // RDCV, read voltage registers
  SPI_ReceiveArray(TestArray,18);
  PEC = SPI_Receive(); // read PEC byte
  SPI_SlaveSelectOffHigh();

}

//***************LTC6802_SelfTest2****************
//Description: Tests ADC on the LTC6802. 
//Inputs: none
//Outputs: none
void LTC6802_SelfTest2(unsigned char TestArray[]){
  int i=0; 
  SPI_SlaveSelectActiveLow();
  SPI_Send(0x1F); // Self test
  SPI_SlaveSelectOffHigh();
  for(i=0;i<8000;i++){}
  SPI_SlaveSelectActiveLow();
  SPI_Send(0x04); // RDCV, read voltage registers
  SPI_ReceiveArray(TestArray,18);
  PEC = SPI_Receive(); // read PEC byte
  SPI_SlaveSelectOffHigh();

}