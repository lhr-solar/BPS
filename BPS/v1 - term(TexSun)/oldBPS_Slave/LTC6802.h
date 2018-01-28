//***************************************************
//LTC6802 Driver Header File
//Archer Finley, Cory Loflin
//BPS-UTSVT
//February 15, 2010
//Description: Header file for drivers for the LTC6802 Multicell Battery Stack Monitor
//Filename: LTC6802.h

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
char LTC6802_ConfigRegWrite(char CFGR0, char CFGR1, char CFGR2, char CFGR3, char CFGR4, char CFGR5);

//***************LTC6802_ConfigRegRead************
//Description: Uses SPI.c to read the LTC6802 Configuration 
// Register after sending empty (0x00) bytes.
//Inputs: 6 byte char array (sends 0x00 as the temp SPI2BUF)
//Outputs: none
void LTC6802_ConfigRegRead(char ConfigReg[]);

//***************LTC6802_StartConversions***********
//Description: Uses SPI.c to send a Start Conversion 
// command to the LTC6802 for all 12 cells
//Inputs: none
//Outputs: none 
void LTC6802_StartConversions(void);

//***************LTC6802_ReadVoltages****************
//Description: Reads voltage registers from the LTC6802. This 
// should take approx 464us with 1MHz SCK.
//Inputs: none
//Outputs: none
void LTC6802_ReadVoltages(void);

//************LTC6802_SortVoltageArray*************
//Description: Sorts the ReadVoltageArray[17] (char) 
//from the LTC_6802 filled by LTC6802_ReadVoltages 
//into Voltages[12] (char)
//Inputs: int Voltages[12] array to store data
//Outputs: none 
void  LTC6802_SortVoltageArray(unsigned int Voltages[]);
