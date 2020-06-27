#include <p30F6012A.h>

extern unsigned int ADC_Tcur;			//top current
extern unsigned int ADC_Tsup;			//top 24V supply
extern unsigned int ADC_Tvlt;			//top voltage
extern unsigned int ADC_Ttmp;			//top temperature
extern unsigned int ADC_5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
extern unsigned int ADC_Bcur;			//bottom current
extern unsigned int ADC_Bsup;			//bottom supply
extern unsigned int ADC_Bvlt;			//bottom volt
extern unsigned int ADC_Btmp;			//bottom temp
extern unsigned int ADC_15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int ADC_15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
extern unsigned int ADC_Vsup;			//12-15V supply from the master to power the slaves
extern unsigned int ADC_temp;			//temperature of the bootstrap slave logic board


//set A
unsigned int ATcur;			//top current
unsigned int ATsup;			//top 24V supply
unsigned int ATvlt;			//top voltage
unsigned int ATtmp;			//top temperature
unsigned int A5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
unsigned int ABcur;			//bottom current
unsigned int ABsup;			//bottom supply
unsigned int ABvlt;			//bottom volt
unsigned int ABtmp;			//bottom temp
unsigned int A15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
unsigned int A15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
unsigned int AVsup;			//12-15V supply from the master to power the slaves
unsigned int Atemp;			//temperature of the bootstrap slave logic board
unsigned char ATstatus;		//status of the top charger LEDs
unsigned char ABstatus;		//status of the bottom charger LEDs
//set B
unsigned int BTcur;			//top current
unsigned int BTsup;			//top 24V supply
unsigned int BTvlt;			//top voltage
unsigned int BTtmp;			//top temperature
unsigned int B5Vo;			//5V output to the bootstrap power board (after the polyfuses, etc.)
unsigned int BBcur;			//bottom current
unsigned int BBsup;			//bottom supply
unsigned int BBvlt;			//bottom volt
unsigned int BBtmp;			//bottom temp
unsigned int B15Vp;			//15V positive on the bootstrap power board (powers the instrumentation amplifiers)
unsigned int B15Vn;			//15V negative on the bootstrap power board (powers the instrumentation amplifiers)
unsigned int BVsup;			//12-15V supply from the master to power the slaves
unsigned int Btemp;			//temperature of the bootstrap slave logic board
unsigned char BTstatus;		//status of the top charger LEDs
unsigned char BBstatus;		//status of the bottom charger LEDs

unsigned char setAGood;
unsigned char setBGood;


void initialize(void);
void getData(void);
void defaultRegisters(void);

int main(void)
{
	initialize();
	defaultRegisters();	
	charger_enable(0);
	charger_enable(1);
	while(1)
	{
		getData();
	}	
}	

void initialize(void)
{
	setAGood = 0;
	setBGood = 0;
	misc_initialize();
	RS485_initialize();
	ADC_initialize();
}
	
void getData(void)
{
	unsigned char workingOnA;

	if(setAGood)			//we did A last time
	{						//so we're doing B now
		workingOnA = 0;
		setBGood = 0;		
	}
	else					//we did B last time (or first run)
	{
		workingOnA = 1;
		setAGood = 0;
	}
	misc_setLEDread(1);
	ADC_start();
	while(!ADC_done());		//wait until ADC has finished
	ADC_scale();
	misc_setLEDread(0);
	if(workingOnA)
	{
		ATcur = ADC_Tcur;
		ATsup = ADC_Tsup;
		ATvlt = ADC_Tvlt;
		ATtmp = ADC_Ttmp;
		A5Vo  = ADC_5Vo;
		ABcur = ADC_Bcur;
		ABsup = ADC_Bsup;
		ABvlt = ADC_Bvlt;
		ABtmp = ADC_Btmp;
		A15Vp = ADC_15Vp;
		A15Vn = ADC_15Vn;
		AVsup = ADC_Vsup;
		Atemp = ADC_temp;
		ATstatus = charger_getStatus(1);
		ABstatus = charger_getStatus(0);
	}
	else
	{
		BTcur = ADC_Tcur;
		BTsup = ADC_Tsup;
		BTvlt = ADC_Tvlt;
		BTtmp = ADC_Ttmp;
		B5Vo  = ADC_5Vo;
		BBcur = ADC_Bcur;
		BBsup = ADC_Bsup;
		BBvlt = ADC_Bvlt;
		BBtmp = ADC_Btmp;
		B15Vp = ADC_15Vp;
		B15Vn = ADC_15Vn;
		BVsup = ADC_Vsup;
		Btemp = ADC_temp;
		BTstatus = charger_getStatus(1);
		BBstatus = charger_getStatus(0);
	}		

	if(workingOnA)
	{
		setAGood = 1;
		setBGood = 0;
	}
	else
	{
		setAGood = 0;
		setBGood = 1;
	}
}
	
void defaultRegisters(void)
{
	//current
	reg_write(2,1,3);
	reg_write(3,1,74);
	reg_write(2,5,3);
	reg_write(3,5,62);
	//bat temp
	reg_write(2,4,45);
	reg_write(3,4,0);
	reg_write(2,8,45);
	reg_write(3,8,0);
	//batt voltage
	reg_write(2,3,5);
	reg_write(3,3,0);
	reg_write(2,7,5);
	reg_write(3,7,0);
	//supply voltage to chargers
	reg_write(2,2,11);
	reg_write(3,2,0);
	reg_write(2,6,11);
	reg_write(3,6,0);
	
}


//newline
