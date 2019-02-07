//***************************************************
//Slave Board dsPIC30F6012A ADC Drivers Code
//Archer Finley, Cory Loflin
//BPS-UTSVT
//March 8, 2010
//Description: Drivers for the ADC module on the dsPIC30F6012A
//Filename: ADCS.c

#include <p30Fxxxx.h>

//***************ADCS_InitSingleSample*************
//Description: Sets the configuration registers for the ADC.
// The ADCON1, ADCON2 and ADCON3 registers control the operation 
// of the A/D module. The ADCHS register selects the input pins 
// to be connected to the S/H amplifiers. The ADPCFG register 
// configures the analog input pins as analog inputs or as 
// digital I/O. The ADCSSL register selects inputs to be 
// sequentially scanned. This initialization function
// sets the ADC to scan input AN15.
//Inputs: none
//Outputs: none
void ADCS_InitSingleSample( void ) {

  //ADCON1
  // <15>     ADON       0      ADC module off  
  // <13>     ADSIDL     0      Continue operation in idle mode
  // <9:8>    FORM       00     Integer data output format
  // <7:5>    SSRC       111    Auto conversion
  // <2>      ASAM       0      Sampling begins when SAMP bit is set
  // <1>      SAMP       x      Write 1 to start sampling
  // <0>      DONE              Flag: Conversion(s) Completed
  ADCON1 = 0x00E0; // 0000 0000 1110 0000

  //ADCON2
  // <15:13>  VCFG       000    Voltage ref pins: AVdd and AVss
  // <12>     RESERVED   0
  // <10>     CSCNA      0      Do not scan inputs
  // <7>      BUFS       Not valid: using 16 bit buffer
  // <5:2>    SMPI       0000   Interrupts at the completion of conversion for each sample/convert sequence
  // <1>      BUFM       0      16-bit buffer size
  // <0>      ALTS       0      Always use MUXA input multiplexer settings
  ADCON2 = 0x0000; // 0000 0000 0000 0000

  //ADCON3
  // <12:8>   SAMC       00001  Auto sample time = 1Tad
  // <7>      ADRC       0      Clock derived from system clock
  // <5:0>    ADCS       000001 ADCS<5:0> = 2*(Tad/Tcy) - 1
  ADCON3 = 0x0101; // 0000 0001 0000 0001

  //ADCHS
  // <12>     CH0NB      0      Channel 0 Neg input for MUXB is Vref-
  // <11:8>   CH0SB      0000   Channel 0 Pos input for MUXB is AN0
  // <4>      CH0NA      0      Channel 0 Neg input for MUXA is Vref-
  // <3:0>    CH0SA      1111   Channel 0 Pos input for MUXB is AN15
  ADCHS = 0x000F; // 0000 0000 0000 1111

  //ADPCFG
  // <15:0>   PCFG       0      Input pin in analog mode
  ADPCFG = 0x0000; // 0000 0000 0000 0000

  //TRISB<15:0> Port directions set to inputs
  TRISB = 0xFFFF;

  //Turn on ADC module
  ADCON1 |= 0x8000;
}

//***************ADCS_InitScanInputs*************
//Description: Sets the configuration registers for the ADC.
// The ADCON1, ADCON2 and ADCON3 registers control the operation 
// of the A/D module. The ADCHS register selects the input pins 
// to be connected to the S/H amplifiers. The ADPCFG register 
// configures the analog input pins as analog inputs or as 
// digital I/O. The ADCSSL register selects inputs to be 
// sequentially scanned. This initialization function
// sets the ADC to scan inputs AN0-15.
//Inputs: none
//Outputs: none
void ADCS_InitScanInputs( void ) {

  //ADCON1
  // <15>     ADON       0      ADC module off  
  // <13>     ADSIDL     0      Continue operation in idle mode
  // <9:8>    FORM       00     Integer data output format
  // <7:5>    SSRC       111    Auto conversion
  // <2>      ASAM       1      Sampling begins automatically
  // <1>      SAMP       x      Write 1 to start sampling
  // <0>      DONE              Flag: Conversion(s) Completed
  ADCON1 = 0x00E4; // 0000 0000 1110 0100

  //ADCON2 
  // <15:13>  VCFG       000    Voltage ref pins: AVdd and AVss
  // <12>     RESERVED   0
  // <10>     CSCNA      1      Scan inputs
  // <7>      BUFS       Not valid: using 16 bit buffer
  // <5:2>    SMPI       1011   Interrupts at the completion of conversion for each 12th sample/convert sequence
  // <1>      BUFM       0      16-bit buffer size
  // <0>      ALTS       0      Always use MUXA input multiplexer settings
  ADCON2 = 0x042C; // 0000 0100 0010 1100

  //ADCON3
  // <12:8>   SAMC       00001  Auto sample time = 1Tad
  // <7>      ADRC       0      Clock derived from system clock
  // <5:0>    ADCS       000001 ADCS<5:0> = 2*(Tad/Tcy) - 1
  ADCON3 = 0x0301; // 0000 0001 0000 0001

  //ADCHS
  // <12>     CH0NB      0      Channel 0 Neg input for MUXB is Vref-
  // <11:8>   CH0SB      0000   Channel 0 Pos input for MUXB is AN0
  // <4>      CH0NA      0      Channel 0 Neg input for MUXA is Vref-
  // <3:0>    CH0SA      0000   Channel 0 Pos input for MUXA is AN0
  ADCHS = 0x0000; // 0000 0000 0000 0000

  //ADPCFG
  // <15:0>   PCFG       0      Input pin in analog mode
  ADPCFG = 0x0000; // 0000 0000 0000 0000

  //ADCSSL
  // <15:0>   CSSL       1      Pin selected for scan
  ADCSSL = 0xFF3C;  // 1111 1111 0011 1100

  //TRISB<15:0> Port directions set to inputs
  TRISB = 0xFFFF;

  //Turn on ADC module
  ADCON1 |= 0x8000;
}

//************ADCS_Sample************
//Description: Starts sampling on ADC channel AN15 and 
// returns the converted value as an unsigned 12-bit 
// integer
//Inputs: none
//Outputs: unsigned integer
unsigned int ADCS_Sample(void) {

  //Start Sampling: SAMP(ADCON<1>) = 1
  ADCON1 |= 0x0002;

  //Wait for conversion to complete
  while( ADCON1bits.DONE == 0 );
  return ADCBUF0;
}

//************ADCS_ScanInputs*********
//Description: Scans inputs AN15-8 and AN5-2 to gather
// 12 temperature readings as unsigned 12-bit integers.
//Inputs: unsigned int TempArray[12] to place data in
//Outputs: none
void ADCS_ScanInputs(unsigned int TempArray[]) {
  char i = 0;
  //Start Sampling
  //ADCON1 |= 0x0002;
  unsigned long Buffer[11] = {0,0,0,0,0,0,0,0,0,0,0};
  //Wait for 12 conversions to complete
  while( ADCON1bits.DONE == 0);
  Buffer[0] =  5*(((4095-((unsigned long)ADCBUFB))*200)/819);
  Buffer[1] =  5*(((4095-((unsigned long)ADCBUFA))*200)/819);
  Buffer[2] =  5*(((4095-((unsigned long)ADCBUF9))*200)/819);
  Buffer[3] =  5*(((4095-((unsigned long)ADCBUF8))*200)/819);
  Buffer[4] =  5*(((4095-((unsigned long)ADCBUF7))*200)/819);
  Buffer[5] =  5*(((4095-((unsigned long)ADCBUF6))*200)/819);
  Buffer[6] =  5*(((4095-((unsigned long)ADCBUF5))*200)/819);
  Buffer[7] =  5*(((4095-((unsigned long)ADCBUF4))*200)/819);
  Buffer[8] =  5*(((4095-((unsigned long)ADCBUF0))*200)/819);
  Buffer[9] =  5*(((4095-((unsigned long)ADCBUF1))*200)/819);
  Buffer[10] =  5*(((4095-((unsigned long)ADCBUF2))*200)/819);
  for(;i<11;i++) {
    TempArray[i] = (unsigned int) Buffer[i];
  }
}










