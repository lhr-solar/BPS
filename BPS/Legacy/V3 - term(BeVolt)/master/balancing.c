#include "balancing.h"
#include "globals.h"
#include "RS485master.h"
#include "Mregisters.h"


static void calculate_balancing(void);

void run_balancing(void)
{
	unsigned char balancing = 0;
	unsigned char i;
	unsigned char currentSlave = 1;		//the slave we are currently interrogating, has NOTHING to do with the current sensor slave board! current here means "one we're dealing with now", not current as in amperes or V over R
	unsigned char cellsHandled = 0;
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

static void calculate_balancing(void)
{
	//#TODO: code in balancing algorithm
}

