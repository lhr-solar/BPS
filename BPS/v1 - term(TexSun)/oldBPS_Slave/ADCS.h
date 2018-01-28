//***************************************************
//Slave Board dsPIC30F6012A ADC Drivers Header File
//Archer Finley
//BPS-UTSVT
//March 8, 2010
//Description: Header file for drivers for the ADC module on the dsPIC30F6012A
//Filename: ADCS.h

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
void ADCS_InitSingleSample( void );

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
void ADCS_InitScanInputs( void );

//************ADC_Sample************
//Description: Starts sampling on ADC channel AN15 and 
// returns the converted value as an unsigned
// 12-bit integer
//Inputs: none
//Outputs: unsigned integer
unsigned int ADCS_Sample(void);

//************ADC_ScanInputs*********
//Description: Scans inputs AN15-8 and AN5-2 to gather
// 12 temperature readings as unsigned 12-bit integers.
//Inputs: unsigned int TempArray[12] to place data in
//Outputs: none
void ADCS_ScanInputs(unsigned int TempArray[]);
