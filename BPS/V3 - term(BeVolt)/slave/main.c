#include <p30Fxxxx.h>
#include <p30F6012A.h>
#include "slave_misc.h"
#include "slave_globals.h"
#include "LTC6803.h"
#include "RS485slave.h"
#include "SPI.h"
#include "ADC.h"
#include "Sregisters.h"

//I'm setting the config fuses in the code since the IDE sometimes forgets (i.e., chip reverts to internal oscillator and baud rates are too slow)
//the bits not set here are at their default (all 1's)
_FOSC(HS)	//set up for external "high speed" 10Mhz XTAL with no PLL


//global globals
unsigned int voltArrayA[12];
unsigned int voltArrayB[12];
unsigned int tempArrayA[12];
unsigned int tempArrayB[12];
unsigned char arrayAgood;		//if the voltarrayA and tempArrayA are good/fresh/consistent
unsigned char arrayBgood;		//same for B. only one of these will be a 1 at a time (both 0 if not sampled at least once yet
unsigned char acquisition_enable;	//1 = system is measuring stuff, 0 = system is not measuring stuff

//local global
static unsigned char OWgood;			//1 = connections good, 0 = open sense wire


void initialize(void);		//initialize the slave hardware
void getData(void);
void defaultRegisters(void);

int main(void)
{
	//defaultRegisters();
	initialize();
	LTC_setConfig(0x91,0,0,0x80,125,175); //set up the LTC config registers
	for(;;)
	{
		if(acquisition_enable)
			getData();	
	}	
}

void initialize()
{
	misc_initialize();		//set up LEDs and DIPswitches
	RS485_initialize();		//need to come after misc_initialize, since the slaveID (from misc) is grabbed during 485 init.
	SPI_initialize();		//set up SPI peripheral to talk to LTC
	LTC_timerInitialize();	//configure the timer(that waits for the LTC's AD conversion) (doesn't send the init command to the LTC)
	ADC_initialize();		//get ADC converter up and running
	if(reg_read(1,10))
		acquisition_enable = 1;
	else
		acquisition_enable = 0;
	arrayAgood = 0;
	arrayBgood = 0;
}


void getData(void)
{
	unsigned char workingOnA;
	char buffer[18];
	unsigned int OWbuffer[12];
	unsigned char i;
	if(arrayAgood)			//we did A last time
	{						//so we're doing B now
		workingOnA = 0;
		arrayBgood = 0;		
	}
	else					//we did B last time (or first run)
	{
		workingOnA = 1;
		arrayAgood = 0;
	}
	misc_setLEDread(1);
	LTC_startConversion();
	ADC_start();
	LTC_startTimer();
	while(!ADC_done());		//wait until ADC has finished
	ADC_scale();
	misc_setLEDread(0);
	if(workingOnA)
	{
		for(i = 0; i < 12; i++)
		{
			tempArrayA[i] = ADC_temp[i];
		}	
	}
	else
	{
		for(i = 0; i < 12; i++)
		{
			tempArrayB[i] = ADC_temp[i];
		}	
	}		
	LTC_waitConversion();
	LTC_readVoltage(buffer);
	if(workingOnA)
	{
		LTC_sortVoltages(buffer, voltArrayA);
		LTC_scaleVoltages(voltArrayA);
	}
	else
	{
		LTC_sortVoltages(buffer, voltArrayB);
		LTC_scaleVoltages(voltArrayB);
	}
	LTC_startTimer();
	LTC_startOWConversion();
	LTC_waitConversion();
	LTC_readVoltage(buffer);
	LTC_sortVoltages(buffer, OWbuffer);
	if(workingOnA)
		OWgood = LTC_OWcompare(voltArrayA, OWbuffer);
	else
		OWgood = LTC_OWcompare(voltArrayB, OWbuffer);
	if(workingOnA)
	{
		arrayAgood = 1;
		arrayBgood = 0;
	}
	else
	{
		arrayAgood = 0;
		arrayBgood = 1;
	}	
}


void defaultRegisters(void)
{
	unsigned char i;
	for(i = 0; i < 12; i++)
	{
		reg_write(3, i, 0);		//LTC offset
		reg_write(2, i, 15);	//LTC gain
		//reg_write(5, i, 2269);		//temp offset 
		//reg_write(4, i, 10);	//temp gain
		reg_write(5, i, 50000);		//temp offset 
		reg_write(4, i, 10);	//temp gain
	}
	reg_write(1, 1, 12)	;		// 12 cells (voltages)
	reg_write(1, 2, 12);		//12 cell temps
	reg_write(1, 8, 3);			//LTC6802
	reg_write(1, 9, 0);			//ignore PEC on receive
	reg_write(1,10, 1);			//start converting registers after boot		
}


//newline
