#ifndef GLOBALS_H
#define GLOBALS_H

//globals that may be acessed by more than one module get declared here (as externs). There are still instantiated in their appropriate .c file
//note that the terminal can read pretty much all of these

//=============================
//=============================
//main.c
//=============================
//=============================
extern unsigned char numCells;			//TODO: initialize this shit!!!!
extern unsigned char boardType; 		//0 = ver 1 (solorean) or 2 (texsun) boards; 1 = ver3 boards (new gen car)
extern unsigned int batteryVoltsA[128];
extern unsigned int batteryTempsA[128];
extern unsigned int batteryVoltsB[128];
extern unsigned int batteryTempsB[128];
extern unsigned int slaveVolts[12];
extern unsigned int slaveTemps[12];
extern unsigned int SOCarray[128];
extern unsigned char balancingPeriod[128];
extern unsigned char arrayAgood;
extern unsigned char arrayBgood;
extern signed int currentA;
extern signed int currentB;
extern unsigned int minVoltage;
extern unsigned int maxVoltage;
extern unsigned int minTemp;
extern unsigned int maxTemp;
extern unsigned char minVcell;		//number of the battery cell at each extreme
extern unsigned char maxVcell;
extern unsigned char minTcell;
extern unsigned char maxTcell;
extern unsigned long totalVoltage;
extern unsigned char BPSstate;		//0 = system is "off" and not measuring any cells, contactor is off
							//1 = system is measuring cell values but contactor is off
							//2 = system is on measuring cells and the contactor is on
							//3 = system is tripped, also is measuring batteries, contacotr off
							//4 = system has an error, contactor off, not measuring
							//5 = system is in initial measurement ("contactor pending")
extern unsigned char BPSsubState;	//shows which step of the precharge sequence we are currently in (i.e., we're waiting for the next step). only meaningful when BPSstate == 2
							//0 = contactor was just enabled or BPS is not in "ON" (#2) state
							//1 = only aux contactor is on
							//2 = precharge (and aux) contactor are on
							//3 = all contactors are on
							//4 = terminal state, aux and main are on, precharge is off
extern unsigned char lastWDToutput;	//remember last WDT output pin status
extern unsigned char balancingOn;	//=1 for when at least one balance switch is on, 0 otherwise
		//===bootstrap stuff===
extern unsigned char BSsetAGood;
extern unsigned char BSsetBGood;
	//set A
extern unsigned int ATcur;			//top current
extern unsigned int ATsup;			//top 24V supply
extern unsigned int ATvlt;			//top voltage
extern unsigned int ATtmp;			//top temperature
extern unsigned int ABcur;			//bottom current
extern unsigned int ABsup;			//bottom supply
extern unsigned int ABvlt;			//bottom volt
extern unsigned int ABtmp;			//bottom temp
extern unsigned char ATstatus;		//status of the top charger LEDs
extern unsigned char ABstatus;		//status of the bottom charger LEDs
	//set B
extern unsigned int BTcur;			//top current
extern unsigned int BTsup;			//top 24V supply
extern unsigned int BTvlt;			//top voltage
extern unsigned int BTtmp;			//top temperature
extern unsigned int BBcur;			//bottom current
extern unsigned int BBsup;			//bottom supply
extern unsigned int BBvlt;			//bottom volt
extern unsigned int BBtmp;			//bottom temp
extern unsigned char BTstatus;		//status of the top charger LEDs
extern unsigned char BBstatus;		//status of the bottom charger LEDs
	//system health stuff
extern unsigned long numCycles;	//counts number of main lop cycles since system boot
extern unsigned int  commErrorsSlave;		//counts the number of comm errors (getting readings from the regular slaves
extern unsigned int  commErrorsBalance;	//counts the number of comm errors (while sending balancing commands to the slaves)
extern unsigned int  commErrorsBootstrap;	//counts the number of comm errors (while getting bootstrap data)
//trip stuff
extern unsigned char tripStatus;	//0 = no trip cause
								//1= current (discharge)
								//2= current (charge)
								//3= undertemp
								//4= overtemp
								//5= undervoltage
								//6= overvoltage
								//7= other system fault/contactor malfuction
								//8= internal BPS malfunction
extern unsigned char tripCell;		//0 = no trip cell
								//any other number is the first cell to cause a tripout
extern unsigned int tripValue;

//=========================
//==========================
//SOC.c
//==========================
//==========================
extern unsigned int SOC_cycletime;				//units = timer ticks (divide by 176 to get miliseconds)
extern unsigned long int SOC_accumulator;		//units = "amp-milliseconds"
extern unsigned char SOC_percent;				//units = percent 0-100
extern unsigned char SOC_initState;			//holds how the SOC was initialized
										//0 = not initialized
										//1 = acc1 i SRAM good and loaded
										//2 = acc2 in SRAM good and loaded
										//3 = acc1 and acc2 are good, acc1 loaded
										//4 = no good SRAM data, default initialization
//=========================
//=========================
//ADC.c
//=========================
//=========================
extern signed int ADC_Xacc;
extern signed int ADC_Yacc;
extern signed int ADC_Zacc;
extern unsigned int ADC_3V3;
extern unsigned int ADC_5V;
extern signed int ADC_currHigh;
extern signed int ADC_currLow;
extern signed int ADC_current;
extern unsigned int ADC_currOffset;
extern unsigned int ADC_curr15Vp;
extern unsigned int ADC_curr15Vn;
extern unsigned int ADC_contactorVoltage;


//=========================
//=========================
//RS485master.c
//=========================
//=========================
extern unsigned char RS485_outData[255];					//data to slaves
extern unsigned char RS485_inData[255];						//data from slaves
extern unsigned char RS485_command;							//master's command
extern unsigned char RS485_slave;							//slave we're talking to
extern unsigned char RS485_response;						//response from slaves
extern unsigned char RS485_outLength;						//DATA length on commands
extern unsigned char RS485_inLength;						//DATA length on response DATA
extern unsigned char RS485_status;					//0 = idle
													//1 = transmitting
													//2 = waiting on reception ("slave is thinking")
													//3 = receiving
													//4 = got good response
													//5 = inter-byte receive timeout
													//6 = corrupt packet received 
													//7 = timed out waiting for slave response	


//===========================
//===========================
//RS422.c
//===========================
//===========================
//terminal mode
extern unsigned char RS422_termMode;					//0 = normal (packet) mode, 1 = interactive terminal mode
extern unsigned char RS422_termInPointer;				//counts the characters received
extern unsigned char RS422_termInBuff[255];				//holds one line of user input
extern unsigned char RS422_termOutBuff[255];			//holds the output to the user
extern unsigned char RS422_termOutLength;
//packet mode
extern unsigned char RS422_outData[255];				//data to controller
extern unsigned char RS422_inData[255];					//data from controller
extern unsigned char RS422_inDevID;						//device ID from Master
extern unsigned char RS422_inCommand;					//command from the controller
extern unsigned char RS422_outResponse;					//response/data-ID to the controller
extern unsigned char RS422_outLength;					//DATA length on output packets
extern unsigned char RS422_inLength;					//DATA length on input packets
//LCD stuff
extern unsigned char RS422_LCDsending;		//1 when LCD is displaying stuff
extern unsigned char RS422_LCDready;		//1 = LCD_buffer is full and ready to send
extern unsigned char RS422_LCDdone;		//1 = LCD has finished a transmission (mainline is GO to put new stuff in LCD_buffer)



//===========================
//===========================
//send.c
//===========================
//===========================
extern unsigned char TX_waiting;	// 0 = nothing is waiting for a chance to send a packet, 1 = something is waiting to send something
extern unsigned char TX_count;		//number of sending loops (used to send lower-resolution data every few main cycles
extern unsigned char TX_step;		//which type of message we are about to send (voltage, temp, status, balance...)
extern unsigned char TX_sendingBatt;	//0 = not sending battery measurement data to BB, 1 = sending battery status packets
extern unsigned char TX_maxCycle;	//largest cycles per packet value
extern unsigned char TX_lastState;	//keeps track of what the last state of the send data subsystem is when there's a packet sending request from the main loop code
extern unsigned char TX_sendingMainline;		//flag is a 1 if we are sending something that the mainline code has requested.

//===========================
//===========================
//LCD.c
//===========================
//===========================
extern unsigned char LCD_buffer[255];
extern unsigned char LCD_outLength;
extern unsigned char LCD_TXreqeust;
extern unsigned char LCD_outPointer;
extern unsigned char LCD_updateEnabled;
#endif
