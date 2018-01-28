#include <p30F6012A.h>
#include "globals.h"
#include "time.h"
#include "ADC.h"
#include "I2Cdev.h"
#include "RTC.h"
#include "SOC.h"
#include "misc.h"
#include "RS422.h"
#include "RS485master.h"
#include "send.h"
#include "terminal.h"
#include "Mregisters.h"
#include "balancing.h"
#include "LCD.h"
#include "CAN.h"

//I'm setting the config fuses in the code since the IDE sometimes forgets (i.e., chip reverts to internal oscillator and baud rates are too slow)
//the bits not set here are at their default (all 1's)
_FOSC(HS)	//set up for external "high speed" 10Mhz XTAL with no PLL


//====LIST OF TIMERS USED=====
// microcontroller has these resources:
//Timer1 - 16 bit (can be used as RTC)
//Timer2 - 16 bit
//Timer3 - 16 bit (T2 and T3 can be ciombined into a 32bit
//timer4 - 16 bit
//timer5 - 16 bit (T4 and T5 can be combined into one 32 bit)
// code uses the following timers:
//T1 - SOC/system health (times main loop)
//T2 - RS485 (receive timout)
//T3 - RS422 (receive timeout)
//T4 - precharge cycling
//T5 - free for now.

//functions local to this file
static void initialize(void);
static unsigned char GetReadings(unsigned char whichArray);
static unsigned char Measure(void);
static void unpackVolts(void);
static void unpackTemps(void);
static void WDT_service(void);		//feed the watchdog timer
static void serviceMiscTask(void);
static void serviceCommands(void);
static unsigned char checkLimits(void);
static void read_bootstrap(void);
static void contactorControl(unsigned char battLimitStatus);
static void default_registers(void);
static void checkWarn(void);
static void serviceInput(void);
static void findMinMax(void);
static void PrechargeTimerSetup(void);
static void stateControl(unsigned char battLimitStatus);			//BPS state machine
static void outputControl(void);				//contacotr (precharge) control logic

//=====global globals!======

unsigned char numCells;				//number of cells (modules) in the BPS system
unsigned char boardType;			//type of board (0 = old type, 1 = new type)
//battery crap
unsigned int batteryVoltsA[128];
unsigned int batteryTempsA[128];
unsigned int batteryVoltsB[128];
unsigned int batteryTempsB[128];
unsigned int slaveVolts[12];
unsigned int slaveTemps[12];
unsigned char balancingPeriod[128];
unsigned char arrayAgood;
unsigned char arrayBgood;
signed int currentA;
signed int currentB;
unsigned int minVoltage;
unsigned int maxVoltage;
unsigned int minTemp;
unsigned int maxTemp;
unsigned long totalVoltage;
unsigned char minVcell;		//number of the battery cell at each extreme
unsigned char maxVcell;
unsigned char minTcell;
unsigned char maxTcell;
							
unsigned char BPSstate;		//shows current state of the BPS system
							//0 = system is "off" and not measuring any cells, contactor is off
							//1 = system is measuring cell values but contactor is off
							//2 = system is on measuring cells and the contactor is on
							//3 = system is tripped, also is measuring batteries, contacotr off
							//4 = system has an error, contactor off, not measuring
							//5 = system is in initial measurement ("contactor pending")
unsigned char BPSsubState = 0;	//shows which step of the precharge sequence we are currently in (i.e., we're waiting for the next step). only meaningful when BPSstate == 2
							//0 = contactor was just enabled or BPS is not in "ON" (#2) state
							//1 = only aux contactor is on
							//2 = precharge (and aux) contactor are on
							//3 = all contactors are on
							//4 = terminal state, aux and main are on, precharge is off
unsigned char BPSreqState;	//requested BPS state
							//0 = no measure, no contactor
							//1 = measuring, no contactor
							//2 = measuring and contactors (i.e., fully on)
unsigned char lastWDToutput;	//remember last WDT output pin status
unsigned char balancingOn;	//=1 for when at least one balance switch is on, 0 otherwise
//===bootstrap stuff===
unsigned char BSsetAGood;
unsigned char BSsetBGood;
//set A
unsigned int ATcur;			//top current
unsigned int ATsup;			//top 24V supply
unsigned int ATvlt;			//top voltage
unsigned int ATtmp;			//top temperature
unsigned int ABcur;			//bottom current
unsigned int ABsup;			//bottom supply
unsigned int ABvlt;			//bottom volt
unsigned int ABtmp;			//bottom temp
unsigned char ATstatus;		//status of the top charger LEDs
unsigned char ABstatus;		//status of the bottom charger LEDs
//set B
unsigned int BTcur;			//top current
unsigned int BTsup;			//top 24V supply
unsigned int BTvlt;			//top voltage
unsigned int BTtmp;			//top temperature
unsigned int BBcur;			//bottom current
unsigned int BBsup;			//bottom supply
unsigned int BBvlt;			//bottom volt
unsigned int BBtmp;			//bottom temp
unsigned char BTstatus;		//status of the top charger LEDs
unsigned char BBstatus;		//status of the bottom charger LEDs
//system health stuff
unsigned long numCycles;	//counts number of main lop cycles since system boot
unsigned int  commErrorsSlave;		//counts the number of comm errors (getting readings from the regular slaves
unsigned int  commErrorsBalance;	//counts the number of comm errors (while sending balancing commands to the slaves)
unsigned int  commErrorsBootstrap;	//counts the number of comm errors (while getting bootstrap data)

//trip logging stuff
unsigned char tripStatus = 0;	//0 = no trip cause
								//1= current (discharge)
								//2= current (charge)
								//3= undertemp
								//4= overtemp
								//5= undervoltage
								//6= overvoltage
								//7= other system fault/contactor malfuction
								//8= internal BPS malfunction
unsigned char tripCell = 0;		//0 = no trip cell
								//any other number is the first cell to cause a tripout
unsigned int tripValue = 0;		//holds he value that caused the system to trip out


//===="local" globals ====
static unsigned int prechargeTimer;			//number of 0.1 second ticks needed before the next step of precharge sequence

int main(void)
{
	unsigned char battOK;
	initialize();
	//default_registers();
	if(reg_read(1,8))		//chek if BMS should start up in measure mode
		BPSstate = 1;
	while(1)
	{
		Measure();				//get the battery readings		
		findMinMax();					//find the highest/lowest voltages and temperatures
		battOK = checkLimits();			//check against limits: 1 = battery OK, 0 = bad
		checkWarn();				//check if the battery is close to tripping (and set the outputs)
		//contactorControl(battOK);		//contactor control logic (and BPS state machine)
		stateControl(battOK);			//BPS state machine
		outputControl();				//contacotr (precharge) control logic
		read_bootstrap();				//handle the bootstrap slave(if we have one)	
		run_balancing();				//calculate balancing, send it to the slaves
		serviceCommands();				//run any 422 commands that can't be run from the 422RX ISR
		send_start();					//start sending battery updates (kick off the TX interrupt, self contained from there)
		CAN_StartSend();				//send CAN bus data (send one info block. then stops)
		LCD_update();					//update the little LCD screen
		WDT_service();					//toggle WDT pin. ALSO needs to check if WDT has failed and put BPS into error state
		SOC_update();					//calcualte SOC and update (send to batt backed RTC SRAM). first run will pull SOC from BB SRAM
		term_serviceWaitingCMD();		//run any commands from the terminal that can't run from the 422RX ISR
		//serviceInput();					//read the input/ignition line and update the desired state if needed.
	}	
	for(;;);
}



static void initialize(void)
{
	lastWDToutput = 0;
	arrayAgood = 0;
	arrayBgood = 0;
	BSsetAGood = 0;
	BSsetBGood = 0;
	BPSstate = 0;
	commErrorsSlave = 0;
	commErrorsBalance = 0;
	commErrorsBootstrap = 0;
	numCycles = 0;

	//now load initial configurations from the registers
	numCells = 	reg_read(3,1);
	if(reg_read(1,13) == 3)
		boardType = 1;
	else
		boardType = 0;
	//now init all subsystems
	misc_initialize();
	send_init();
	I2C_init();
	RS485_initialize();
	RS422_initialize();
	ADC_initialize();
	SOC_initialize();
	CAN_initialize();
	LCD_initialize();
	PrechargeTimerSetup();
	IPC6 = 0x4433; //default is 0x4444, change U2TX and RX (RS422) interrupt from priority 4 to priority 3 (the RS485 has priority 4 on everything as well. I want to make the 422 TX lower priority)
}

//looks at battery data, and populates the max and min values for temp and voltage
//also calculates the total battery stack voltage.
static void findMinMax(void)
{
	unsigned char i;
	if ((BPSstate == 0) || (BPSstate == 4))				//if not measuring, just skip the min/max crap
	{
		minVoltage   = 0;
		maxVoltage   = 0;
		minTemp      = 0;
		maxTemp      = 0;
		totalVoltage = 0;	
		minVcell = 0;		//if not measuring, the min and max cells are zeroed
		maxVcell = 0;
		minTcell = 0;
		maxTcell = 0;
		return;
	}
	minVcell = 1;			//the rest of this code just goes iterates over all the cells and finds the minimum and max values (and the cell #). also adds up total pack voltage.
	maxVcell = 1;
	minTcell = 1;
	maxTcell = 1;
	if(arrayAgood)
	{
		minVoltage   = batteryVoltsA[0];
		maxVoltage   = batteryVoltsA[0];
		minTemp      = batteryTempsA[0];
		maxTemp      = batteryTempsA[0];
		totalVoltage = batteryVoltsA[0];
		for(i = 1; i < numCells; i++)
		{
			if(batteryVoltsA[i] > maxVoltage)
				{maxVoltage = batteryVoltsA[i]; maxVcell = i;}
			if(batteryVoltsA[i] < minVoltage)
				{minVoltage = batteryVoltsA[i]; minVcell = i;}
			if(batteryTempsA[i] > maxTemp)
				{maxTemp = batteryTempsA[i]; maxTcell = i;}
			if(batteryTempsA[i] < minTemp)
				{minTemp = batteryTempsA[i]; minTcell = i;}
			totalVoltage += batteryVoltsA[i];
		}
	}
	else if(arrayBgood)
	{
		minVoltage   = batteryVoltsB[0];
		maxVoltage   = batteryVoltsB[0];
		minTemp      = batteryTempsB[0];
		maxTemp      = batteryTempsB[0];
		totalVoltage = batteryVoltsB[0];
		for(i = 1; i < numCells; i++)
		{
			if(batteryVoltsB[i] > maxVoltage)
				{maxVoltage = batteryVoltsB[i]; maxVcell = i;}
			if(batteryVoltsB[i] < minVoltage)
				{minVoltage = batteryVoltsB[i]; minVcell = i;}
			if(batteryTempsB[i] > maxTemp)
				{maxTemp = batteryTempsB[i];	maxTcell = i;}
			if(batteryTempsB[i] < minTemp)
				{minTemp = batteryTempsB[i];	minTcell = i;}
			totalVoltage += batteryVoltsB[i];
		}
	}
	else		//no valid data
	{
		minVoltage   = 0;
		maxVoltage   = 0;
		minTemp      = 0;
		maxTemp      = 0;
		totalVoltage = 0;
	}
	
}


//reads in stuff from the ADC and the polls the (normal) slaves for voltages and temperatures. scales it all, and combines it into the propper arrays
//returns 0 for sucess
//returns 1 for a fault (i.e. slave no response)
//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
static unsigned char GetReadings(unsigned char whichArray)
{
	unsigned char i;					//for loop variable for zeroing out bad readings
	unsigned char badRead;				//1 = a bad read, 0 = got good data
	unsigned char retryCntr;			//counts number of retries
	unsigned char currentSlave = 1;		//the slave we are currently interrogating, has NOTHING to do with the current sensor slave board! current here means "one we're dealing with now", not current as in amperes or V over R
	unsigned char cellsHandled = 0;		//number of cell readings we have gotten in
	unsigned char commError = 0;
	unsigned char slaveCellCtr;			//iterate over the cells of each slave assembly
	ADC_start();	//start the AD converter reading the currents
	while((currentSlave < 16) && (cellsHandled < reg_read(3,1))) //loop through all slaves untill all cells are accounted for (or we run out of slaves)
	{	
		if(reg_read(2, currentSlave) > 12 || reg_read(2, currentSlave) == 0) //skip this loop altogether if no cells on this slave, or too many
		{
			currentSlave++;
			continue;
		}	
		//TODO: commError += send read command and wait for response (and do retrys)
		//retry if failed
		//if failed, it should fill volt and temp array with 0's
		//do for both temp and volt 
		//probabally stick in the RS485 retry stuff into the end of the RS485 interrupt handlers somewhere.

		//===voltages===//
		retryCntr = reg_read(1,10);
		badRead = 0;
		do				//loops around trying to get some readings
		{
			if(badRead)						//if we're looping back around...
				retryCntr--;
			RS485_slave = currentSlave;
			RS485_command = 7;
			RS485_outLength = 0;
			RS485_sendPacket(1);
			while(!RS485_ready())
			{
				//TODO: misc task
			}
			badRead = (RS485_status != 4) || (RS485_response != 0);
			if(badRead)
				if(commErrorsSlave + 1)	//make sure it doesn't roll over
					commErrorsSlave++;
			commError++;
		}while(badRead && retryCntr);
		if(badRead)						//came out of the reading/retry loop without valid data				
		{
			misc_setLEDFault(1);
			for(i = 0; i < 12; i++)	//zero out readings array to guarantee a trip if there's a problem
				slaveVolts[i] = 0;   
		}
		else
			unpackVolts();
		
		//===temperatures===//
		retryCntr = reg_read(1,10);
		badRead = 0;
		do				//loops around trying to get some readings
		{
			if(badRead)						//if we're looping back around...
				retryCntr--;
			RS485_slave = currentSlave;
			RS485_command = 8;
			RS485_outLength = 0;
			RS485_sendPacket(1);
			while(!RS485_ready())
			{
				//TODO: misc task
			}
			badRead = (RS485_status != 4) || (RS485_response != 0);
			if(badRead)
				if(commErrorsSlave + 1)	//make sure it doesn't roll over
					commErrorsSlave++;
			commError++;
		}while(badRead && retryCntr);
		if(badRead)						//came out of the reading/retry loop without valid data				
		{
			misc_setLEDFault(1);
			for(i = 0; i < 12; i++)	//zero out readings array to guarantee a trip if there's a problem
				slaveVolts[i] = 0;   
		}
		else
			unpackTemps();

		if(whichArray)
		{
			for(slaveCellCtr = 0; slaveCellCtr < reg_read(2, currentSlave); slaveCellCtr++)  //run the loop for the number of cells on the current slave
			{
				batteryVoltsB[cellsHandled+slaveCellCtr] = slaveVolts[slaveCellCtr];
				batteryTempsB[cellsHandled+slaveCellCtr] = slaveTemps[slaveCellCtr];
			}	
		}
		else
		{
			for(slaveCellCtr = 0; slaveCellCtr < reg_read(2, currentSlave); slaveCellCtr++)
			{
				batteryVoltsA[cellsHandled+slaveCellCtr] = slaveVolts[slaveCellCtr];
				batteryTempsA[cellsHandled+slaveCellCtr] = slaveTemps[slaveCellCtr];
			}	
		}
		cellsHandled += reg_read(2, currentSlave); 		//OK, we are done with reading the values for the curent slave, so we incremen the counter as these cells have been dealt with already
		currentSlave++;
	}	
	while(!ADC_done()); //wait for ADC to finish (it should have finished before the first slave responds)
	ADC_scale();	//convert ADC raw count output into currents (and volts and acceleration G's for the misc inputs)
	if(whichArray)
		currentB = ADC_current;
	else	
		currentA = ADC_current;
	return commError;
}	

//reads in the battery parameters. calls GetReadings, and switches between the two arrays.
//returns 0 for OK, or a 1 for some error
static unsigned char Measure()
{
	unsigned char error;
	unsigned char workingOnA;
	if ((BPSstate == 0) || (BPSstate == 4))				//if not measuring, just skip the measurements
	{
		serviceMiscTask();
		arrayBgood = 0;
		arrayAgood = 0;
		return 0;
	}	
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
	if(workingOnA)
	{
		error = GetReadings(0); 	//read from slaves into arrayA
		arrayAgood = 1;
		arrayBgood = 0;
	}	
	else
	{
		error =  GetReadings(1);	//read from slaves into ArrayB
		arrayBgood = 1;
		arrayAgood = 0;
	}
	return error;
}	

//copies the voltage data from a slave response packet into the slaveVolts array
static void unpackVolts(void)
{
	unsigned char i;
	for (i = 0; (2*i) < RS485_inLength; i++)
	{
		if(i > 12)
			return;
		slaveVolts[i] = RS485_combineBytes(RS485_inData[2*i], RS485_inData[(2*i)+1]); //MSB, LSB
	}	
}	

//copies the temperature data from a slave response packet into the slaveTemps aray
static void unpackTemps(void)
{
	unsigned char i;
	for (i = 0; (2*i) < RS485_inLength; i++)
	{
		if(i > 12)
			return;
		slaveTemps[i] = RS485_combineBytes(RS485_inData[2*i], RS485_inData[(2*i)+1]); //MSB, LSB
	}
}

static void WDT_service(void)		//feed the watchdog timer
{
	if (!((BPSstate == 0) || (BPSstate == 4)))	//only increment number of main loop iterations if the BPS is in a measure mode
		numCycles++;
	if(BPSstate == 4)  		//we don't service the watchdog timer if the BPS is in an error status
		return;
	if(lastWDToutput)		//toggle the WDT output pin
	{
		misc_setWDT(0);
		lastWDToutput = 0;
	}
	else
	{
		misc_setWDT(1);
		lastWDToutput = 1;
	}		
}	

static void serviceMiscTask(void)
{
}
	
static void serviceCommands(void)		//runs all RS422 commands that were put in the que during the previous cycle
{
	while(RS422_commandsWaiting())
	{	
		TX_waiting = 1;			//request the interrupt-based RS422 status update code to relinquish the bus
		RS422_dequeCMD(1);
	}

}

//returns 1 if the batteries are within limits, 0 otherwise.	
//also populates the trip status reason globals so we can tell why it tripped out....
static unsigned char checkLimits(void)
{
	unsigned int hightemp;
	unsigned int lowtemp;
	if(BPSstate == 0 || BPSstate == 3 || BPSstate == 4)				//don't bother checking the limits if we aren't in the ready state
		return 0;
	if(!(arrayAgood || arrayBgood)) //it's bad if neither array is OK, this means that the system did not yet read the cells
		return 0;
	tripStatus = 0;
	tripCell = 0;
	if(arrayAgood)
	{
		if(currentA > (signed)reg_read(4,3))  //charging current
		{	
			tripStatus = 2;
			tripValue = currentA;
			return 0;
		}
		if(currentA < (signed)reg_read(4,4)) //discharging current
		{
			tripStatus = 1;
			tripValue = currentA;
			return 0;
		}
		if(currentA > 0)					//get the high and low temp set points based on if battery is charging or not
		{
			hightemp = reg_read(4,5);
			lowtemp = reg_read(4,6);
		}	
		else
		{
			hightemp = reg_read(4,7);
			lowtemp = reg_read(4,8);
		}
	}
	else if(arrayBgood)
	{
		if(currentB > (signed)reg_read(4,3))
		{	
			tripStatus = 2;
			tripValue = currentB;
			return 0;
		}
		if(currentB < (signed)reg_read(4,4))		//gotta typecast this as signed otherwise it thinks the value out of the register is a large positive number...
		{
			tripStatus = 1;
			tripValue = currentB;
			return 0;
		}
		if(currentB > 0)
		{
			hightemp = reg_read(4,5);
			lowtemp = reg_read(4,6);
		}	
		else
		{
			hightemp = reg_read(4,7);
			lowtemp = reg_read(4,8);
		}	
	}
	else
		return 0;	
		
	if(minVoltage < reg_read(4,2))		//undervoltage
		{tripStatus = 5; tripCell = minVcell; tripValue = minVoltage; return 0;}
	if(maxVoltage > reg_read(4,1))		//overvoltage
		{tripStatus = 6; tripCell = maxVcell; tripValue = maxVoltage; return 0;}
	if(minTemp < lowtemp)				//too cold
		{tripStatus = 3; tripCell = minTcell; tripValue = minTemp; return 0;}
	if(maxTemp > hightemp)				//too hot
		{tripStatus = 4; tripCell = maxTcell; tripValue = maxTemp; return 0;}
	return 1; //hey, no bad readings!
}	

static void contactorControl(unsigned char battLimitStatus)
{
//	unsigned char BPSstate;		//0 = system is "off" and not measuring any cells, contactor is off
							//1 = system is measuring cell values but contactor is off
							//2 = system is on measuring cells and the contactor is on
							//3 = system is tripped, also is measuring batteries, contacotr off
							//4 = system has an error, contactor off, not measuring
							//5 = system commanded on, check limits and then turn it on
	
	if(((BPSstate == 2)||(BPSstate == 5)) && battLimitStatus)
	{		//contactor is on
		misc_setMainContactor(1);		//these are commented out untill the new precharge capable system is built
		misc_setAuxContactor(1);
		misc_setPreContactor(1);
		misc_setOutBPSON(1);
	}	
	else
	{		//contactor is off
		misc_setMainContactor(0);
		misc_setAuxContactor(0);
		misc_setPreContactor(0);
		misc_setOutBPSON(0);
	}
	if(((BPSstate == 2)||(BPSstate == 5)) && (!battLimitStatus))		//handle tripping
		BPSstate = 3;
	if((BPSstate == 5) && battLimitStatus)			//handle first measure cycle
		BPSstate = 2;
	if((BPSstate == 3) || (BPSstate == 4))			//the "TRIP" light comes on for a trip or BPS internal error
		misc_setOutTrip(1);
	else
		misc_setOutTrip(0);
}	



//reads parameters from the bootstrap board (if present)
static void read_bootstrap(void)
{
	unsigned char retryCntr;
	unsigned char badRead;
	unsigned char BSaddy;
	if ((BPSstate == 0) || (BPSstate == 4))				//if not measuring, just skip the measurements
	{
		serviceMiscTask();
		BSsetAGood = 0;
		BSsetBGood = 0;
		return;
	}
	BSaddy = reg_read(1,7);
	unsigned char setA;	//true if reading from set A
	if(BSaddy == 0)		//no bootstrap slave in system
		return;
	if(BSaddy > 16)		//not a valid address.
		return;
	if(reg_read(2,BSaddy) != 0)		//slave is marked as having regular cells.
		return;
	if(BSsetBGood)
	{
		setA = 1;
	}
	else
	{
		setA = 0;	//or the first run...
	}
	retryCntr = reg_read(1,10);
	badRead = 0;
	do				//loops around trying to get some readings
	{
		if(badRead)						//if we're looping back around...
			retryCntr--;
		RS485_slave = BSaddy;
		RS485_command = 11;
		RS485_outLength = 0;
		RS485_sendPacket(1);
		while(!RS485_ready())
		{
			//TODO: misc task
		}
		badRead = (RS485_status != 4) || (RS485_response != 0);
		if(badRead)
			if(commErrorsBootstrap + 1)	//make sure it doesn't roll over
				commErrorsBootstrap++;
	}while(badRead && retryCntr);
	if(badRead)
	{
		BSsetAGood = 0;
		BSsetBGood = 0;
		misc_setLEDFault(1);
		return;
	}
	else	//got a good response from BS board; stick the data from packet in the correct variables
	{
		if(setA)
		{
			ATvlt = RS485_combineBytes(RS485_inData[0], RS485_inData[1]);
			ATcur = RS485_combineBytes(RS485_inData[2], RS485_inData[3]);
			ATtmp = RS485_combineBytes(RS485_inData[4], RS485_inData[5]);
			ATsup = RS485_combineBytes(RS485_inData[6], RS485_inData[7]);
			ATstatus = RS485_inData[8];
			ABvlt = RS485_combineBytes(RS485_inData[9], RS485_inData[10]);
			ABcur = RS485_combineBytes(RS485_inData[11], RS485_inData[12]);
			ABtmp = RS485_combineBytes(RS485_inData[13], RS485_inData[14]);
			ABsup = RS485_combineBytes(RS485_inData[15], RS485_inData[16]);
			ABstatus = RS485_inData[17];
			BSsetAGood = 1;
			BSsetBGood = 0;
		}
		else
		{
			BTvlt = RS485_combineBytes(RS485_inData[0], RS485_inData[1]);
			BTcur = RS485_combineBytes(RS485_inData[2], RS485_inData[3]);
			BTtmp = RS485_combineBytes(RS485_inData[4], RS485_inData[5]);
			BTsup = RS485_combineBytes(RS485_inData[6], RS485_inData[7]);
			BTstatus = RS485_inData[8];
			BBvlt = RS485_combineBytes(RS485_inData[9], RS485_inData[10]);
			BBcur = RS485_combineBytes(RS485_inData[11], RS485_inData[12]);
			BBtmp = RS485_combineBytes(RS485_inData[13], RS485_inData[14]);
			BBsup = RS485_combineBytes(RS485_inData[15], RS485_inData[16]);
			BBstatus = RS485_inData[17];
			BSsetBGood = 1;
			BSsetAGood = 0;
		}	
	}	
}

static void checkWarn(void)
{
//TODO: code this to check for the warning limits
}

//this checks if the IGNITION input is active
static void serviceInput(void)
{
	if(reg_read(1,11))			//if ignition input is enabled
	{
		if(misc_isIgnition())	//if ignition input is high
		{
			if((BPSstate == 0) || (BPSstate == 1))		//if the BPS contactor was off, put into contactor pending mode
				BPSstate = 5;  							//do nothing if it's tripped or already on or contactor pending
		}
		else
		{
			if(reg_read(1,8))	//revert to the power-up state when ignition is turned off
				BPSstate = 1;
			else
				BPSstate = 0;
		}
	}
}


static void default_registers(void)
{
//reg_write(unsigned char group, unsigned char reg, unsigned int value)
	reg_write(3,1,40); 	//cells in BPS system = 40
	reg_write(2,1,10);	//11 cells on slave 1
	reg_write(2,2,10);	//10 cells on slave 2
	reg_write(2,3,10);	//10 cells on slave 3
	reg_write(2,4,10);	//10 cells on slave 4
	reg_write(2,5,0);	//no cells on slave 5 (bootstrap)
	reg_write(1,2,1);	//1 cycles per status packet
	reg_write(1,3,1);	//send a volt packet every 1 cycles
	reg_write(1,4,1);	//send a temp packet every 1 cycles
	reg_write(1,5,0); 	//no balncing packets
	reg_write(1,6,0);	//no botstrap packets
	reg_write(1,7,0);	//bootstrap board at address 0 (i.e., no bootstrap).
	reg_write(1,8,1);	//startup in state 1
	reg_write(1,9,0);	//no balancing
	
	reg_write(4,1,36000);	//overvoltage = 3.6V
	reg_write(4,2,25000);	//undervoltage = 2.5V
	reg_write(4,3,5000);	//chaging overcurrent = 50A
	reg_write(4,4,-15000);	//discharge limit = 150A
	reg_write(4,5,33300);	//charge overtemp = 60C
	reg_write(4,6,27300);	//charge undertemp = 0C
	reg_write(4,7,33300);	//discharge overtemp = 60C
	reg_write(4,8,24400);	//discharge undertemp = -30C

	reg_write(5,1,5);			//low range gain is 2.5 (/2 is hardcoded)
	reg_write(5,2,0);			//analog zero-shift is hard-coded
	reg_write(5,3,10);			//high range scale factor is 10 
	reg_write(5,4,0);			//analog zero-shift offset ishard-coded, this complements it
	reg_write(5,5,5000);		//high/low transition point is 50A

	reg_write(7,1,10);			//LCD update every 10 main loop iterations
	//BPS uses an unsigned int for temp, in tenths of a degree kelvin
	//BPS uses unsigned int for the cell voltages, in tenths of a milivolt
	//BPS uses signed int for the pack current, positive is charge, and is in hundredths of amps
}

static void PrechargeTimerSetup(void)
{
	//pretty much just set up the timer
	T4CON = 0x0020;		//run from Fcy and /64 divider
	PR4 = 3906;			//pop off an interrupt once every ~0.1 second
	TMR4 = 0;
	IFS1bits.T4IF = 0;	//clear and tehn enable the Timer4 interrupt
	IEC1bits.T4IE = 1;
}

static void stateControl(unsigned char battLimitStatus)			//BPS state machine
{	//#TODO: handle the terminal overide functionality
	if(reg_read(1,11))			//if ignition input is enabled
	{
		if(misc_isIgnition())	//if ignition input is high
		{
			if((BPSstate == 0) || (BPSstate == 1))		//if the BPS contactor was off, put into contactor pending mode
				BPSstate = 5;  							//do nothing if it's tripped or already on or contactor pending
		}
		else
		{	//note that turning off the system will 
			if(reg_read(1,8))	//revert to the power-up state when ignition is turned off
				BPSstate = 1;
			else
				BPSstate = 0;
		}
	}
	if((BPSstate == 2)||(BPSstate == 5))	//if the BPS should turn on contactor or leave it on
	{
		if(battLimitStatus)					//if battery is within limits
			BPSstate = 2;	//contactor on
		else
		{
			BPSstate = 3;	//tripped
			//#TODO: log a trip...
		}
	}
}
static void outputControl(void)				//contacotr (precharge) control logic
{
	if(BPSstate != 2)
	{
		T4CONbits.TON = 0;
		BPSsubState = 0;
		misc_setMainContactor(0);
		if((reg_read(1,16) != 4) || (reg_read(1,16) == 3))			//handle the "always on" aux contactor state and the "keep on during trip" mode
			misc_setAuxContactor(0);
		misc_setPreContactor(0);
		misc_setOutBPSON(0);	
	}
	else	//if contactor should be on
	{
		misc_setOutBPSON(1);
		if(reg_read(1,14))	//precharging
		{
			//BPSsubState should be at 0 here if we just turned on, since nothing else sets it away from zero other than this code and the T4 ISR
			if(BPSsubState == 0)	//we just turned on the system
			{
				if(reg_read(1,16) != 0)
					misc_setAuxContactor(1);	//turn on the aux contactor
				BPSsubState = 1;
				prechargeTimer = reg_read(1,17);
				TMR4 = 0;
				T4CONbits.TON = 1;	
			}
		}
		else		//no precharging
		{
			misc_setMainContactor(1);
			if(reg_read(1,16) == 2)			//if in "straight mirror main contactor" mode
				misc_setAuxContactor(1);
		}
	}
	if(reg_read(1,16) == 4)			//handle the "always on" aux contacotr setting
		misc_setAuxContactor(1);
	if((BPSstate == 3) || (BPSstate == 4))	//turn on the trip LED if the system is tripped (or in internal error state)
		misc_setOutTrip(1);
	else
		misc_setOutTrip(0);
}

//fires off once every 0.1 seconds during precharge sequence
void __attribute__((__interrupt__, auto_psv)) _T4Interrupt(void)
{
//	unsigned char BPSsubState = 0;	//shows which step of the precharge sequence we are currently in (i.e., we're waiting for the next step). only meaningful when BPSstate == 2
							//0 = contactor was just enabled or BPS is not in "ON" (#2) state
							//1 = only aux contactor is on
							//2 = precharge (and aux) contactor are on
							//3 = all contactors are on
							//4 = terminal state, aux and main are on, precharge is off
	IFS1bits.T4IF = 0;
	if((prechargeTimer == 0) && (BPSstate == 2))		//it's time (and BPS is still in "on" state)
	{
		if(BPSsubState == 0)
		{
			//shouldn't be here
		}
		else if (BPSsubState == 1)
		{
			BPSsubState = 2;
			misc_setPreContactor(1);
			prechargeTimer = reg_read(1,18);
		}
		else if (BPSsubState == 2)
		{
			BPSsubState = 3;
			misc_setMainContactor(1);
			prechargeTimer = reg_read(1,19);
		}
		else if (BPSsubState == 3)
		{
			BPSsubState = 4;
			misc_setPreContactor(0);
			T4CONbits.TON = 0;
			
		}
		else if (BPSsubState == 4)
		{
			//shouldn't run be here
		}
	}
	else			//not yet, just decrement the counter...
	{
		prechargeTimer--;
	}
}


//T4CONbits.TON

//newline

