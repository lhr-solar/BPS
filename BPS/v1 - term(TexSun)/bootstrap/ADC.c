/*******************************************************
/ADC.c
/implements the ADC driver for the dsPIC
/it will put all the results for temp into the ADC_temp array
/ADC_scale applies the scaling factors to all ADC readings
/Fred Engelkemeir, Summer 2012
/
/AN0	(Vref of 4.096)							SKIP
/AN1	Tcur						BUF0		 
/AN2	Tsup						BUF1	
/AN3	Tvlt						BUF2	
/AN4	Ttmp						BUF3	
/AN5	5Vo							BUF4
/AN6	debugger used!(Vbus)					SKIP
/AN7	debugger used!(10V)						SKIP
/AN8	Bcur						BUF5
/AN9	Bsup						BUF6
/AN10	Bvlt						BUF7
/AN11	Btmp						BUF8
/AN12	15Vp						BUF9
/AN13	15Vn						BUF10
/AN14	Vsup						BUF11
/AN15	temp						BUF12

====SCALING FACTORS====
The ADC Vref is a precision 4.096V reference on a 12-bit ADC, so 1 count is 0.001V (1mV). range is 0V to 4V
5Vo is divided by half (2.5V nominal, 2500 dec)
Vsup is (need to check the board)

*******************************************************/



#include <p30F6012A.h>

signed int ADC_Tcur;			//top current
unsigned int ADC_Tsup;			//top 24V supply
unsigned int ADC_Tvlt;			//top voltage
unsigned int ADC_Ttmp;			//top temperature
unsigned int ADC_5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
signed int ADC_Bcur;			//bottom current
unsigned int ADC_Bsup;			//bottom supply
unsigned int ADC_Bvlt;			//bottom volt
unsigned int ADC_Btmp;			//bottom temp
unsigned int ADC_15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
unsigned int ADC_15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
unsigned int ADC_Vsup;			//12-15V supply from the master to power the slaves
unsigned int ADC_temp;			//temperature of the bootstrap slave logic board

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
	ADC_Tcur = ADCBUF0;
	ADC_Tsup = ADCBUF1;
	ADC_Tvlt = ADCBUF2;
	ADC_Ttmp = ADCBUF3;
	ADC_5Vo = ADCBUF4;
	ADC_Bcur = ADCBUF5;
	ADC_Bsup = ADCBUF6;
	ADC_Bvlt = ADCBUF7;
	ADC_Btmp = ADCBUF8;
	ADC_15Vp = ADCBUF9;
	ADC_15Vn = ADCBUFA;
	ADC_Vsup = ADCBUFB;
	ADC_temp = ADCBUFC;
	ADC_finishedFlag = 1;
}

void ADC_scale(void)
{
	//current
	ADC_Tcur = ADC_Tcur-2500;
	ADC_Tcur = ADC_Tcur*reg_read(2,1) + reg_read(3,1);
	ADC_Tcur = ADC_Tcur/2;
	ADC_Bcur = ADC_Bcur-2500;
	ADC_Bcur = ADC_Bcur*reg_read(2,5) + reg_read(3,5);
	ADC_Bcur = ADC_Bcur/2;
	//temperature
	ADC_Ttmp = ADC_Ttmp*reg_read(2,4) + reg_read(3,4);
	ADC_Btmp = ADC_Btmp*reg_read(2,8) + reg_read(3,8);
	//batt voltage
	ADC_Tvlt = ADC_Tvlt*reg_read(2,3) + reg_read(3,3);
	ADC_Bvlt = ADC_Bvlt*reg_read(2,7) + reg_read(3,7);
	//charger supply
	ADC_Tsup = ADC_Tsup*reg_read(2,2) + reg_read(3,2);
	ADC_Bsup = ADC_Bsup*reg_read(2,6) + reg_read(3,6);
	
	//TODO: supply voltage and board temps
}


//newline....

