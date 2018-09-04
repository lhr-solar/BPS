#include <p30F6012A.h>

void initialize(void);
void serialStim(void);
unsigned char GetReadings(unsigned char whichArray);
unsigned char Measure(void);
void unpackVolts(void);
void serialStim2(void);
void unpackTemps(void);
void WDT_service(void);		//feed the watchdog timer
void serviceMiscTask(void);
void serviceCommands(void);
unsigned char checkLimits(void);
void updateSOC(void);
void contactorControl(unsigned char battLimitStatus);
void send_status(unsigned char whichArray);
void send_SOC(void);
void send_voltages(unsigned char whichArray);
void send_temps(unsigned char whichArray);
void send_balancing(void);
void send_next(void);
void send_start(void);
void send_bootstrap(unsigned char whichSet);
void default_registers(void);
void run_balancing(void);
void calculate_balaing(void);
void checkWarn(void);
void serviceInput(void);

extern unsigned char RS485_command;
extern unsigned char RS485_slave;
extern unsigned char RS485_outLength;
extern unsigned char RS485_outData[];
extern unsigned char RS485_inData[];
extern unsigned char RS485_inLength;
extern unsigned char RS485_receive;
extern unsigned char RS485_status;
extern unsigned char RS485_response;

extern unsigned char SOC_percent;
extern unsigned char SOC_initState;

extern signed int ADC_current;
extern unsigned char RS422_outResponse;
extern unsigned char RS422_outData[];
extern unsigned char RS422_outLength;
extern unsigned char RS422_termMode;

unsigned int batteryVoltsA[128];
unsigned int batteryTempsA[128];
unsigned int batteryVoltsB[128];
unsigned int batteryTempsB[128];
unsigned int slaveVolts[12];
unsigned int slaveTemps[12];
//unsigned int SOCarray[128];
unsigned char balancingPeriod[128];
unsigned char arrayAgood;
unsigned char arrayBgood;
signed int currentA;
signed int currentB;
unsigned char BPSstate;		//0 = system is "off" and not measuring any cells, contactor is off
							//1 = system is measuring cell values but contactor is off
							//2 = system is on measuring cells and the contactor is on
							//3 = system is tripped, also is measuring batteries, contacotr off
							//4 = system has an error, contactor off, not measuring
							//5 = system is in initial measurement ("contactor pending")
unsigned char lastWDToutput;	//remember last WDT output pin status
unsigned char balancingOn;	//=1 for when at least one balance switch is on, 0 otherwise
unsigned char TX_waiting;	// 0 = nothing is waiting for a chance to send a packet, 1 = something is waiting to send something
unsigned char TX_count;		//number of sending loops (used to send lower-resolution data every few main cycles
unsigned char TX_step;		//which type of message we are about to send (voltage, temp, status, balance...)
unsigned char TX_sendingBatt;	//0 = not sending battery measurement data to BB, 1 = sending battery status packets
unsigned char TX_maxCycle;	//largest cycles per packet value
unsigned char TX_lastState;	//keeps track of what the last state of the send data subsystem is when there's a packet sending request from the main loop code
unsigned char TX_sendingMainline;		//flag is a 1 if we are sending something that the mainline code has requested.
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


int main(void)
{
	unsigned char battOK;
	initialize();
	//default_registers();
	//RTC_test();
	//serialStim();
	//serialStim2();
	if(reg_read(1,8))		//chek if BMS should start up in measure mode
		BPSstate = 1;
	while(1)
	{
		if(Measure());			//put BPS into the error state if there's an internal error
			//BPSstate = 4;
		battOK = checkLimits();
		checkWarn();
		contactorControl(battOK);
		read_bootstrap();
		run_balancing();
		serviceCommands();
		send_start();
		WDT_service();
		SOC_update();
		term_serviceWaitingCMD();
		serviceInput();
	}	
	for(;;);
}



void initialize(void)
{
	lastWDToutput = 0;
	arrayAgood = 0;
	arrayBgood = 0;
	BSsetAGood = 0;
	BSsetBGood = 0;
	BPSstate = 0;
	TX_step = 0;
	TX_count = 1;
	TX_waiting = 0;
	TX_sendingMainline = 0;
	TX_sendingBatt = 0;
	commErrorsSlave = 0;
	commErrorsBalance = 0;
	commErrorsBootstrap = 0;
	numCycles = 0;
	TX_maxCycle = reg_read(1,2);			//find the max value and put into TX_maxCycle
	if(reg_read(1,3) > TX_maxCycle)
		TX_maxCycle = reg_read(1,3);
	if(reg_read(1,4) > TX_maxCycle)
		TX_maxCycle = reg_read(1,4);
	if(reg_read(1,5) > TX_maxCycle)
		TX_maxCycle = reg_read(1,5);
	if(reg_read(1,6) > TX_maxCycle)
		TX_maxCycle = reg_read(1,6);
	if(reg_read(1,12) > TX_maxCycle)
		TX_maxCycle = reg_read(1,12);
	misc_initialize();
	I2C_init();
	RS485_initialize();
	RS422_initialize();
	ADC_initialize();
	SOC_initialize();
	IPC6 = 0x4433; //default is 0x4444, change U2TX and RX (RS422) interrupt from priority 4 to priority 3 (the RS485 has priority 4 on everything as well. I want to make the 422 TX lower priority)
}

//tests grabbing readings from the slave units
void serialStim2()
{
	unsigned char i;
	unsigned char commError;
	commError = 0;
	while(1)
	{
	//===voltages===//
		RS485_slave = 2;
		RS485_command = 7;
		RS485_outLength = 0;
		RS485_sendPacket(1);
		while(!RS485_ready())
		{
			//TODO: misc task
		}
		if((RS485_status != 4) || (RS485_response != 0))		//if either comms error or slave error code
		{
			for(i = 0; i < 12; i++)	//zero out readings array to guarantee a trip if there's a problem
				slaveVolts[i] = 0;   
			commError++;
		}
		else
			unpackVolts();

		//===temperatures===//
		RS485_slave = 2;
		RS485_command = 8;
		RS485_outLength = 0;
		RS485_sendPacket(1);
		while(!RS485_ready())
		{
			//TODO: misc task
		}
		if((RS485_status != 4) || (RS485_response != 0))		//if either comms error or slave error code
		{
			for(i = 0; i < 12; i++)	//zero out readings array to guarantee a trip if there's a problem
				slaveTemps[i] = 0;   
			commError++;
		}
		else
			unpackTemps();
		if(commError)
			misc_setLEDError(1);
		i++;
		i++;
		i++;
	}
}

//baseline RS485 test routine, no longer used
serialStim()
{
	unsigned char i;
	/*//while(!misc_isDebug());		//wait until DEBUG is turned on
	RS485_command = 100;
	RS485_slave = 1;
	RS485_outLength = 0;
	RS485_sendPacket(1);
	while(!RS485_ready());		//wait to send the 100 cmd
	RS485_command = 101;
	RS485_outLength = 0;
	RS485_slave = 1;
	RS485_sendPacket(1);
	//return;
	while(!RS485_ready());		//wait for a timout on resp to 101 cmd
	RS485_command = 100;
	RS485_slave = 1;
	RS485_outLength = 0;
	RS485_sendPacket(1);
	while(!RS485_ready());		//wait to send the 100 cmd
	RS485_command = 1;
	RS485_outLength = 0;
	RS485_slave = 1;
	RS485_sendPacket(1);
	while(!RS485_ready());		//wait for the ident response
*/
	



	RS485_command = 2;
	RS485_outData[0] = 1;
	RS485_outData[1] = 1;
	RS485_outData[2] = 11;
	RS485_outData[3] = 8;
	RS485_outLength = 4;
	RS485_slave = 1;
	RS485_sendPacket(1);
	while(!RS485_ready());		//wait for the reg write response

	RS485_command = 3;
	RS485_outData[0] = 1;
	RS485_outData[1] = 1;
	RS485_outLength = 2;
	RS485_slave = 1;
	RS485_sendPacket(1);
	while(!RS485_ready());		//wait for the reg read response

	while(misc_isDebug())
	{
		RS485_command = 150;
		RS485_outLength = 0;
		RS485_slave = 1;
		RS485_sendPacket(1);
		while(!RS485_ready());		//wait for a response on turning on the LED
		RS485_command = 160;
		RS485_outLength = 0;
		RS485_slave = 1;
		RS485_sendPacket(1);
		while(!RS485_ready());		//wait for a response on turning off the LED
	} 
	RS422_outLength = 255;
	for(i = 0; i < 255; i++)
		RS422_outData[i] = 0;
	RS422_sendPacket();
}

//reads in stuff from the ADC and the polls the (normal) slaves for voltages and temperatures. scales it all, and combines it into the propper arrays
//returns 0 for sucess
//returns 1 for a fault (i.e. slave no response)
//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
unsigned char GetReadings(unsigned char whichArray)
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
			misc_setLEDError(1);
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
			misc_setLEDError(1);
			for(i = 0; i < 12; i++)	//zero out readings array to guarantee a trip if there's a problem
				slaveVolts[i] = 0;   
		}
		else
			unpackTemps();
		/*
		RS485_slave = currentSlave;
		RS485_command = 8;
		RS485_outLength = 0;
		RS485_sendPacket(1);
		while(!RS485_ready())
		{
			//TODO: misc task
		}
		if((RS485_status != 4) || (RS485_response != 0))		//if either comms error or slave error code
		{
			if(commErrorsSlave + 1)	//make sure it doesn't roll over
				commErrorsSlave++;
			misc_setLEDError(1);
			for(i = 0; i < 12; i++)	//zero out readings array to guarantee a trip if there's a problem
				slaveTemps[i] = 0;   
			commError++;
		}
		else
			unpackTemps();
		*/
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
unsigned char Measure()
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
void unpackVolts(void)
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
void unpackTemps(void)
{
	unsigned char i;
	for (i = 0; (2*i) < RS485_inLength; i++)
	{
		if(i > 12)
			return;
		slaveTemps[i] = RS485_combineBytes(RS485_inData[2*i], RS485_inData[(2*i)+1]); //MSB, LSB
	}
}

void WDT_service(void)		//feed the watchdog timer
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

void serviceMiscTask(void)
{
}
	
void serviceCommands(void)		//runs all RS422 commands that were put in the que during the previous cycle
{
	while(RS422_commandsWaiting())
	{	
		TX_waiting = 1;			//request the interrupt-based RS422 status update code to relinquish the bus
		RS422_dequeCMD(1);
	}

}

//returns 1 if the batteries are within limits, 0 otherwise.	
unsigned char checkLimits(void)
{
	unsigned char checkingA;
	unsigned int hightemp;
	unsigned int lowtemp;
	unsigned char i;
	if(BPSstate == 0 || BPSstate == 1 || BPSstate == 3 || BPSstate == 4)				//don't bother checking the limits if we aren't in the ready state
		return 0;
	if(!(arrayAgood || arrayBgood)) //it's bad if neither array is OK, this means that the system did not yet read the cells
		return 0;
	if(arrayAgood)
		checkingA = 1;
	else if(arrayBgood)
		checkingA = 0;
	else
		return 0;
	if(checkingA)
	{
		if(currentA > reg_read(4,3))  //charging current
			return 0;
		if(currentA < reg_read(4,4)) //discharging current
			return 0;	
	}
	else
	{
		if(currentB > reg_read(4,3))
			return 0;
		if(currentB < reg_read(4,4))
			return 0;	
	}				
	
	if(checkingA)				//get the high and low temp set points based on if battery is charging or not
	{
		if(currentA > 0)
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
	{
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
	
	if(checkingA) 
	{
		for(i = 0; i < reg_read(3,1); i++)
		{
			if(batteryVoltsA[i] > reg_read(4,1))
				return 0;
			if(batteryVoltsA[i] < reg_read(4,2))
				return 0;
			if(batteryTempsA[i] > hightemp)
				return 0;
			if(batteryTempsA[i] < lowtemp)
				return 0;
		}	
	}
	else
	{
		for(i = 0; i < reg_read(3,1); i++)
		{
			if(batteryVoltsB[i] > reg_read(4,1))
				return 0;
			if(batteryVoltsB[i] < reg_read(4,2))
				return 0;
			if(batteryTempsB[i] > hightemp)
				return 0;
			if(batteryTempsB[i] < lowtemp)
				return 0;
		}	
	}			
	return 1; //hey, no bad readings!
}	


void contactorControl(unsigned char battLimitStatus)
{
//	unsigned char BPSstate;		//0 = system is "off" and not measuring any cells, contactor is off
							//1 = system is measuring cell values but contactor is off
							//2 = system is on measuring cells and the contactor is on
							//3 = system is tripped, also is measuring batteries, contacotr off
							//4 = system has an error, contactor off, not measuring
							//5 = system commanded on, check limits and then turn it on
	
	if(((BPSstate == 2)||(BPSstate == 5)) && battLimitStatus)
	{		//contactor is on
		misc_setContactor(1);
		misc_setOutBPSON(1);
	}	
	else
	{		//contactor is off
		misc_setContactor(0);
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

//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
//sends the BPS status packet (and the system current)
void send_status(unsigned char whichArray)
{
	RS422_outResponse = 1;
	RS422_outLength = 3;
	RS422_outData[0] = BPSstate;
	if(balancingOn)
		RS422_outData[0] += 0b10000000;
	if(whichArray)
	{
		RS422_outData[1] = RS485_splitMSB(currentB);
		RS422_outData[2] = RS485_splitLSB(currentB);
	}
	else
	{
		RS422_outData[1] = RS485_splitMSB(currentA);
		RS422_outData[2] = RS485_splitLSB(currentA);
	}
	RS422_sendPacket();
}


//sends the State of Charge value
void send_SOC(void)
{
	RS422_outResponse = 11;
	RS422_outLength = 1;
	RS422_outData[0] = SOC_percent;
	if(SOC_initState == 4 || SOC_initState == 0)	//if SoC value was corrupt or SoC has not run yet.
		RS422_outData[0] = 255;						//then send 255 (real vlaues go from 0 to 100)
	RS422_sendPacket();
}

//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
//sends a voltage status packet	
void send_voltages(unsigned char whichArray)
{
	unsigned char numCells;
	unsigned char i;
	numCells = reg_read(3,1);
	RS422_outResponse = 2;
	RS422_outLength = 2*numCells;
	for(i = 0; i < numCells; i++)
	{
		if(whichArray)
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryVoltsB[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryVoltsB[i]);
		}
		else
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryVoltsA[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryVoltsA[i]);
		}
	}
	RS422_sendPacket();	
}
	
//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B
//sends a temperature status packet
void send_temps(unsigned char whichArray)
{
	unsigned char numCells;
	unsigned char i;
	numCells = reg_read(3,1);
	RS422_outResponse = 3;
	RS422_outLength = 2*numCells;
	for(i = 0; i < numCells; i++)
	{
		if(whichArray)
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryTempsB[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryTempsB[i]);
		}
		else
		{
			RS422_outData[2*i] = RS485_splitMSB(batteryTempsA[i]);
			RS422_outData[2*i+1] = RS485_splitLSB(batteryTempsA[i]);
		}
	}
	RS422_sendPacket();	
}	

//sends the balancing status
void send_balancing(void)
{
	//TODO: add code!
	RS422_outResponse = 4;
	RS422_sendPacket();	
}

//whichArray is a 0 for making it fill into arrays A, and a 1 for filling array set B (note that setA and setB good are the ones for bootstrap, not the ones for the main readings!)
//sends a bootstrap update packet
void send_bootstrap(unsigned char whichSet)
{
	//RS485_response = 0;
	if(whichSet)
	{
		RS422_outData[0] = RS485_splitMSB(BTvlt);
		RS422_outData[1] = RS485_splitLSB(BTvlt);
		RS422_outData[2] = RS485_splitMSB(BTcur);
		RS422_outData[3] = RS485_splitLSB(BTcur);
		RS422_outData[4] = RS485_splitMSB(BTtmp);
		RS422_outData[5] = RS485_splitLSB(BTtmp);
		RS422_outData[6] = RS485_splitMSB(BTsup);
		RS422_outData[7] = RS485_splitLSB(BTsup);
		RS422_outData[8] = BTstatus;
		RS422_outData[9] = RS485_splitMSB(BBvlt);
		RS422_outData[10] = RS485_splitLSB(BBvlt);
		RS422_outData[11] = RS485_splitMSB(BBcur);
		RS422_outData[12] = RS485_splitLSB(BBcur);
		RS422_outData[13] = RS485_splitMSB(BBtmp);
		RS422_outData[14] = RS485_splitLSB(BBtmp);
		RS422_outData[15] = RS485_splitMSB(BBsup);
		RS422_outData[16] = RS485_splitLSB(BBsup);
		RS422_outData[17] = BBstatus;
	}
	else
	{
		RS422_outData[0] = RS485_splitMSB(ATvlt);
		RS422_outData[1] = RS485_splitLSB(ATvlt);
		RS422_outData[2] = RS485_splitMSB(ATcur);
		RS422_outData[3] = RS485_splitLSB(ATcur);
		RS422_outData[4] = RS485_splitMSB(ATtmp);
		RS422_outData[5] = RS485_splitLSB(ATtmp);
		RS422_outData[6] = RS485_splitMSB(ATsup);
		RS422_outData[7] = RS485_splitLSB(ATsup);
		RS422_outData[8] = ATstatus;
		RS422_outData[9] = RS485_splitMSB(ABvlt);
		RS422_outData[10] = RS485_splitLSB(ABvlt);
		RS422_outData[11] = RS485_splitMSB(ABcur);
		RS422_outData[12] = RS485_splitLSB(ABcur);
		RS422_outData[13] = RS485_splitMSB(ABtmp);
		RS422_outData[14] = RS485_splitLSB(ABtmp);
		RS422_outData[15] = RS485_splitMSB(ABsup);
		RS422_outData[16] = RS485_splitLSB(ABsup);
		RS422_outData[17] = ABstatus;
	}
	RS422_outLength = 18;
	RS422_outResponse = 5;
	RS422_sendPacket();	
}	

// called by the RS422 TX interrupt handler whenever it has completed sending a packet
// this code will figure out which packet to send net.
void send_next(void)
{
	unsigned char array = 0;
	unsigned char didntsend = 1;
	unsigned char stuckloopctr = 0;
	if (TX_waiting)		//if something from the main loop code is wanting to send a packet, we'l relinquish the bus to it.
	{
		TX_lastState = TX_sendingBatt;
		TX_sendingBatt = 0;
		TX_sendingMainline = 1;
		TX_waiting = 0;
		return;
	}
	if(TX_sendingMainline)	//called by the 422 tx interrupt handler at the end of sending a packet requested from mainloop
	{						//so keep sending packets before the mainloop code jutted in.
		TX_sendingMainline = 0;
		TX_sendingBatt = TX_lastState;
	}
	if((BPSstate == 0) || (BPSstate == 4))	//if BPS is not measuring cells, don't send anything
	{
		TX_sendingBatt = 0;
		return;
	}
	if(arrayAgood)
		array = 0;
	else if(arrayBgood)
		array = 1;
	else					//no good cell readings to send 
	{
		TX_sendingBatt = 0;
		return;	
	}
	while(didntsend && (stuckloopctr < 255))						//loops around until we have something to send!
	{																//stuck loop counter is for making sure we don't get stuck in an infinite loop here
		if((TX_step == 0) && (TX_count >= reg_read(1,2)))
		{
			if(reg_read(1,2))
			{
				didntsend = 0;
				send_status(array);
			}
			else
				TX_step++;
		}
		if((TX_step == 1) && (TX_count >= reg_read(1,3)))
		{
			if(reg_read(1,3))
			{
				didntsend = 0;
				send_voltages(array);
			}
			else
				TX_step++;
		}
		if((TX_step == 2) && (TX_count >= reg_read(1,4)))
		{
			if(reg_read(1,4))
			{
				didntsend = 0;
				send_temps(array);
			}
			else
				TX_step++;
		}
		if((TX_step == 3) && (TX_count >= reg_read(1,5)))
		{
			if(reg_read(1,5))
			{
				didntsend = 0;
				send_balancing();
			}
			else
				TX_step++;
		}
		if((TX_step == 4) && (TX_count >= reg_read(1,6)))
		{
			if(reg_read(1,6))
			{
				if(BSsetAGood)
				{
					didntsend = 0;
					send_bootstrap(0);
				}
				else if(BSsetBGood)
				{
					didntsend = 0;
					send_bootstrap(1);
				}
				else
					TX_step++;
			}
			else
				TX_step++;
		}
		if((TX_step == 5) && (TX_count >= reg_read(1,12)))
		{
			if(reg_read(1,12))
			{
				didntsend = 0;
				send_SOC();
			}
			else
				TX_step++;
		}
		TX_step++;
		if(TX_step >= 6)		//looked at each type of packet to send, so go to the next cycle
		{
			TX_step = 0;
			TX_count++;
			if(TX_count > TX_maxCycle)	//loop back around
				TX_count = 0;
		}
		stuckloopctr++;
	}
	if(didntsend)			//make sure to flag if we didn't send aything
		TX_sendingBatt = 0;
	return; 
}

//this function is used to kick off sending a packet.
void send_start(void)
{
	if(TX_sendingBatt)	//we're already sending battery data
		return;
	if(arrayAgood)
	{
		while(!RS422_ready());
		send_status(0);
		TX_sendingBatt = 1;
		return;
	}
	else if(arrayBgood)
	{
		while(!RS422_ready());
		send_status(1);
		TX_sendingBatt = 1;
		return;
	}
	else
	{
		TX_sendingBatt = 0;
		return;	
	}
}

//reads parameters from the bootstrap board (if present)
void read_bootstrap(void)
{
	unsigned char retryCntr;
	unsigned char badRead;
	unsigned char BSaddy;
	if ((BPSstate == 0) || (BPSstate == 4))				//if not measuring, just skip the measurements
	{
		serviceMiscTask();
		BSsetAGood = 0;
		BSsetBGood = 0;
		return 0;
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
		misc_setLEDError(1);
		return;
	}
	else
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

void checkWarn(void)
{
//TODO: code this to check for the warning limits
}
void serviceInput(void)
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

void run_balancing(void)
{
	unsigned char balancing = 0;
	unsigned char i;
	unsigned char currentSlave = 1;		//the slave we are currently interrogating, has NOTHING to do with the current sensor slave board! current here means "one we're dealing with now", not current as in amperes or V over R
	unsigned char cellsHandled = 0;
	unsigned char slaveCellCtr;
	if(BPSstate == 2 && reg_read(1,9))		//running mode
	{
		calculate_balancing();
		balancing = 1;
	}
	else			//balancing disabled or BPS not in the RUN mode
	{
		balancing = 0;
	}
	while((currentSlave < 16) && (cellsHandled < reg_read(3,1))) //loop through all slaves untill all cells are accounted for (or we run out of slaves)
	{	
		if(reg_read(2, currentSlave) > 12 || reg_read(2, currentSlave) == 0) //skip this loop altogether if no cells on this slave, or too many
		{
			currentSlave++;
			continue;
		}	
		RS485_slave = currentSlave;
		RS485_command = 9;		//set balancing
		RS485_outLength = reg_read(2, currentSlave);	//issue balancing period update for each cell on the slave.
		if(balancing)
		{
			for(i = 0; i < reg_read(2, currentSlave); i++)
				RS485_outData[i] = balancingPeriod[i + cellsHandled];
		}
		else
		{
			for(i = 0; i < reg_read(2, currentSlave); i++)
				RS485_outData[i] = 0;
		}
		RS485_sendPacket(1);
		while(!RS485_ready())
		{
			//TODO: misc task
		}
		/*if((RS485_status != 4) || (RS485_response != 0))		//if either comms error or slave error code
		{
			if(commErrorsBalance + 1)	//make sure it doesn't roll over
				commErrorsBalance++;
			misc_setLEDError(1);
			//#TODO: handle a comms error here (or slave returning a balancing disabled message/balancing error...)
		}*/
		cellsHandled += reg_read(2, currentSlave); 		//OK, we are done with the curent slave, so we incremen the counter as these cells have been dealt with already
		currentSlave++;
	}
	
}

void calculate_balancing(void)
{
	//#TODO: code in balancing algorithm
}

void default_registers(void)
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
	reg_write(1,7,5);	//bootstrap board at address 5.
	reg_write(1,8,1);	//startup in state 1
	reg_write(1,9,0);	//no balancing
	
	reg_write(4,1,36000);	//overvoltage = 3.6V
	reg_write(4,2,25000);	//undervoltage = 2.5V
	reg_write(4,3,5000);	//chaging overcurrent = 50A
	reg_write(4,4,-15000);	//discharge limit = 150A
	reg_write(4,5,3330);	//charge overtemp = 60C
	reg_write(4,6,2730);	//charge undertemp = 0C
	reg_write(4,7,3330);	//discharge overtemp = 60C
	reg_write(4,8,2440);	//discharge undertemp = -30C

	reg_write(5,1,5);			//low range gain is 2.5 (/2is hardcoded)
	reg_write(5,2,0);			//analog zero-shift is hard-coded
	reg_write(5,3,10);			//high range scale factor is 10 
	reg_write(5,4,0);			//analog zero-shift offset ishard-coded, this complements it
	reg_write(5,5,5000);		//high/low transition point is 50A
	//BPS uses an unsigned int for temp, in tenths of a degree kelvin
	//BPS uses unsigned int for the cell voltages, in tenths of a milivolt
	//BPS uses signed int for the pack current, positive is charge, and is in hundredths of amps
}



//newline

