/*******************************************************
/ADC.c
/implements the ADC driver for the dsPIC
/it will put all the results for temp into the ADC_temp array
/ADC_scale applies the scaling factors to the temps from the register system
/ADC_V5 is the precision 5V reference used for the temp input amplifiers (not the system digital 5V)
/the V10 and Vbus are not available, since the pins they are on are shared with the PGM/PGD pins
/Fred Engelkemeir, Summer 2012
/
/AN0	(Vref of 4.096)		 			SKIP
/AN1	GND-Vref						SKIP		 
/AN2	N/C								SKIP	
/AN3	Z-acc						BUF0	
/AN4	Y-Acc						BUF1	
/AN5	X-acc						BUF2
/AN6	debugger used!(Vbus)			SKIP
/AN7	debugger used!(10V)				SKIP
/AN8	3.3V						BUF3
/AN9	5V							BUF4
/AN10	HighCurr					BUF5
/AN11	LowCurr						BUF6
/AN12	CurrOffset(4V)				BUF7
/AN13	-15Vsense					BUF8
/AN14	+15Vsense					BUF9
/AN15	ContactorVsense				BUF10
/
*******************************************************/

#define ADC_5V_OFFSET 0
#define ADC_5V_GAIN 20

#include <p30F6012A.h>
#include "ADC.h"
#include "Mregisters.h"

signed int ADC_Xacc;
signed int ADC_Yacc;
signed int ADC_Zacc;
unsigned int ADC_3V3;
unsigned int ADC_5V;
signed int ADC_currHigh;
signed int ADC_currLow;
signed int ADC_current;
unsigned int ADC_currOffset;
unsigned int ADC_curr15Vp;
unsigned int ADC_curr15Vn;
unsigned int ADC_contactorVoltage;


static unsigned char ADC_finishedFlag;

void ADC_initialize(void)
{
	ADPCFG = 0x00C0;			//all as analog inputs except for pin 7 and 8 (programming pins)
	ADCSSL = 0xFF38;			//scan all ADCs except for pins 7,8 (programming pins), and 0 (Vref), 1 (-Vref), and 2(No Connect)
	ADCON1bits.FORM0 = 0;		//output in integer mode
	ADCON1bits.FORM1 = 0;
	ADCON1bits.SSRC0 = 1;		//start conversion automagicaly after a sampling time
	ADCON1bits.SSRC1 = 1;
	ADCON1bits.SSRC2 = 1;
	ADCON1bits.ASAM = 0;		//start sampling manually (for now)
	ADCON2 = 0x2428;			//use external Vref+, internal Vref-, scan enabled, 16 word single buffer, use only mux A, and scan 10 inputs
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
	ADC_Xacc 		= ADCBUF2;
	ADC_Yacc 		= ADCBUF1;
	ADC_Zacc 		= ADCBUF0;
	ADC_3V3	 		= ADCBUF3;
	ADC_5V			= ADCBUF4;
	ADC_currHigh	= ADCBUF5;
	ADC_currLow		= ADCBUF6;
	ADC_currOffset	= ADCBUF7;
	ADC_curr15Vp	= ADCBUF8;
	ADC_curr15Vn	= ADCBUF9;
	ADC_contactorVoltage = ADCBUFA;
	ADC_finishedFlag = 1;
}
//unsigned int reg_read(unsigned char group, unsigned char reg)
void ADC_scale(void)
{
	ADC_currLow = (((signed)ADC_currLow - 2048)*(signed)reg_read(5,1))/2 + (signed)reg_read(5,2);
	ADC_currHigh = ((signed)ADC_currHigh - 2048)*(signed)reg_read(5,3) + (signed)reg_read(5,4);
	if((ADC_currHigh > reg_read(5,5)) || ((0-ADC_currHigh) > reg_read(5,5))) //if absolute value of high scale current above threshold, use high scale, otherwise use low
		ADC_current = ADC_currHigh;
	else
		ADC_current = ADC_currLow;
	if(reg_read(5,24))							//invert the direction of the current sensor
		ADC_current = 0-ADC_current;
	ADC_Xacc = ADC_Xacc*reg_read(5,6) + reg_read(5,7);
	ADC_Yacc = ADC_Xacc*reg_read(5,8) + reg_read(5,9);
	ADC_Zacc = ADC_Zacc*reg_read(5,10) + reg_read(5,11);
	ADC_curr15Vp = ADC_curr15Vp*reg_read(5,12) + reg_read(5,13);
	ADC_curr15Vn = ADC_curr15Vn*reg_read(5,14) + reg_read(5,15);
	ADC_currOffset = ADC_currOffset*reg_read(5,16) + reg_read(5,17);
	ADC_3V3 = ADC_3V3*reg_read(5,18) + reg_read(5,19);
	ADC_5V = ADC_5V*reg_read(5,20) + reg_read(5,21);
	ADC_contactorVoltage = ADC_contactorVoltage*reg_read(5,22) + reg_read(5,23);
}


//newline....

