/*******************************************************
/ADC.c
/implements the ADC driver for the dsPIC
/it will put all the results for temp into the ADC_temp array
/ADC_scale applies the scaling factors to the temps from the register system
/ADC_V5 is the precision 5V reference used for the temp input amplifiers (not the system digital 5V)
/the V10 and Vbus are not available, since the pins they are on are shared with the PGM/PGD pins
/Fred Engelkemeir, Summer 2012
/
/AN0	(Vref of 4.096)							SKIP
/AN1	5 volts (divided by 2)		BUF0		 
/AN2	Cell 9						BUF1	
/AN3	CEll 10						BUF2	
/AN4	Cell 11						BUF3	
/AN5	Cell 12						BUF4
/AN6	debugger used!(Vbus)					SKIP
/AN7	debugger used!(10V)						SKIP
/AN8	Cell 8						BUF5
/AN9	Cell 7						BUF6
/AN10	Cell 6						BUF7
/AN11	Cell 5						BUF8
/AN12	Cell 4						BUF9
/AN13	Cell 3						BUF10
/AN14	Cell 2						BUF11
/AN15	Cell 1						BUF12
/
*******************************************************/

#define ADC_5V_OFFSET 0
#define ADC_5V_GAIN 20

#include <p30F6012A.h>

unsigned int ADC_temp[12];
unsigned int ADC_Vbus;			//stores the bus voltage
unsigned int ADC_V10;			//10V supply for the temp sensors
unsigned int ADC_V5;			//5V precision supply for temp circuit

void ADC_initialize(void);
unsigned char ADC_done(void);
void ADC_start(void);
void ADC_scale(void);

unsigned char ADC_finishedFlag;

void ADC_initialize(void)
{
	ADPCFG = 0x00C0;			//all as analog inputs except for pin 7 and 8 (programming pins)
	ADCSSL = 0xFF3E;			//scan all ADCs except for pins 7,8 (programming pins), and 0 (Vref)
	ADCON1bits.FORM0 = 0;		//output in integer mode
	ADCON1bits.FORM1 = 0;
	ADCON1bits.SSRC0 = 1;		//start conversion automagicaly after a sampling time
	ADCON1bits.SSRC1 = 1;
	ADCON1bits.SSRC2 = 1;
	ADCON1bits.ASAM = 0;		//start sampling manually (for now)
	ADCON2 = 0x2434;				//use external Vref+, internal Vref-, scan enabled, 16 word sinle buffer, use only mux A, and scan 13 inputs
    ADCON3 = 0x0303;			//sample for 3TAD, and TAD = 1.5Tcy ( = 600ns, almost double the spec)
	ADC_finishedFlag = 0;
	IFS0bits.ADIF = 0;
	IEC0bits.ADIE = 1;
	ADCON1bits.ADON = 1;		//turn on ADC module
}

unsigned char ADC_done(void)
{
	return ADC_finishedFlag;
}

void ADC_start(void)
{
	ADC_finishedFlag = 0;
	ADCON1bits.ASAM = 1;		//start sampling automatically (so that it can get the whole array)
	ADCON1bits.SAMP = 1;		//start sampling/conversion sequence.
}

void __attribute__((__interrupt__, auto_psv)) _ADCInterrupt(void)
{
	IFS0bits.ADIF = 0;		//reset ADC interrupt flag
	ADCON1bits.ASAM = 0;	//turn off the autosampling
	ADCON1bits.SAMP = 1;	//stop any in-progress conversions....
	ADC_V5 = ADCBUF0;
	ADC_temp[0] = ADCBUFC;
	ADC_temp[1] = ADCBUFB;
	ADC_temp[2] = ADCBUFA;
	ADC_temp[3] = ADCBUF9;
	ADC_temp[4] = ADCBUF8;
	ADC_temp[5] = ADCBUF7;
	ADC_temp[6] = ADCBUF6;
	ADC_temp[7] = ADCBUF5;
	ADC_temp[8] = ADCBUF1;
	ADC_temp[9] = ADCBUF2;
	ADC_temp[10] = ADCBUF3;
	ADC_temp[11] = ADCBUF4;
	ADC_finishedFlag = 1;
}

void ADC_scale(void)
{
	int i;
	for(i = 0; i < 12; i++)
	{
		ADC_temp[i] = (reg_read(5,i)) - (ADC_temp[i]*reg_read(4,i));
	}
	ADC_V5 = ADC_V5*ADC_5V_GAIN + ADC_5V_OFFSET;
}


//newline....

