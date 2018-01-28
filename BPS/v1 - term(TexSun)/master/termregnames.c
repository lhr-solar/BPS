//termregnames.c
//Fred Engelkemeir spring 2013
//outpouts the name/description of a given register

void term_appendRegName(unsigned char type, unsigned char group, unsigned char reg);


void term_appendRegName(unsigned char type, unsigned char group, unsigned char reg)
{
	if(type == 0)		//master
	{
		if(group == 1)	//CONFIG group
		{
			if(reg == 1)
				term_appendString("1.1: number of slave boards of all types in system.");
			else if(reg == 2)
				term_appendString("1.2: number of status/current packets per update cycle.");
			else if(reg == 3)
				term_appendString("1.3: number of voltage packets per update cycle.");
			else if(reg == 4)
				term_appendString("1.4: number of temperature packets per update cycle.");
			else if(reg == 5)
				term_appendString("1.5: number of balancing packets per update cycle.");
			else if(reg == 6)
				term_appendString("1.6: number of bootstrap packets per update cycle.");
			else if(reg == 7)
				term_appendString("1.7: Address of bootstrap slave (0 = no bootstrap board).");
			else if(reg == 8)
				term_appendString("1.8: Startup AutoMeasure (1 = BPS powers up and starts measuring cells, 0 = BPS starts up in idle state).");
			else if(reg == 9)
				term_appendString("1.9: Balancing Global Enable (1 = balancing enabled, 0 = no balancing)."); 
			else if(reg == 10)
				term_appendString("1.10: Number of slave Read Retries."); 
			else if(reg == 11)
				term_appendString("1.11: \"IGNITION\" input enable (0 = serial commands, 1 = use hardware ign input)."); 
			else if(reg == 12)
				term_appendString("1.12: number of SOC packets per update cycle."); 
			return;
		}
		else if(group == 2)		//slaves group
		{
			if(reg == 1)
				term_appendString("2.1: number of cells on slave #1 (enter 0 for bootstrap).");
			else if(reg == 2)
				term_appendString("2.2: number of cells on slave #2 (enter 0 for bootstrap).");
			else if(reg == 3)
				term_appendString("2.3: number of cells on slave #3 (enter 0 for bootstrap).");
			else if(reg == 4)
				term_appendString("2.4: number of cells on slave #4 (enter 0 for bootstrap).");
			else if(reg == 5)
				term_appendString("2.5: number of cells on slave #5 (enter 0 for bootstrap).");
			else if(reg == 6)
				term_appendString("2.6: number of cells on slave #6 (enter 0 for bootstrap).");
			else if(reg == 7)
				term_appendString("2.7: number of cells on slave #7 (enter 0 for bootstrap).");
			else if(reg == 8)
				term_appendString("2.8: number of cells on slave #8 (enter 0 for bootstrap).");
			else if(reg == 9)
				term_appendString("2.9: number of cells on slave #9 (enter 0 for bootstrap).");
			else if(reg == 10)
				term_appendString("2.10: number of cells on slave #10 (enter 0 for bootstrap).");
			else if(reg == 11)
				term_appendString("2.11: number of cells on slave #11 (enter 0 for bootstrap).");
			else if(reg == 12)
				term_appendString("2.12: number of cells on slave #12 (enter 0 for bootstrap).");
			else if(reg == 13)
				term_appendString("2.13: number of cells on slave #13 (enter 0 for bootstrap).");
			else if(reg == 14)
				term_appendString("2.14: number of cells on slave #14 (enter 0 for bootstrap).");
			else if(reg == 15)
				term_appendString("2.15: number of cells on slave #15 (enter 0 for bootstrap).");
			else if(reg == 16)
				term_appendString("2.16: number of cells on slave #16 (enter 0 for bootstrap).");
			return;
		}
		else if(group == 3)		//battery group
		{
			if(reg == 1)
				term_appendString("3.1: number of cells in entire battery pack.");
			else if(reg == 2)
				term_appendString("3.2: Battery pack Amp-Hours.");
			else if(reg == 3)
				term_appendString("3.3: SOC algorithm (1 = A123)");
			else if(reg == 4)
				term_appendString("3.4: allow force recalibrations(bit field, bit0:trip UV, bit1:tripOV, bit2:warnUV, bit3:warnOV).");
			else if(reg == 5)
				term_appendString("3.5: UnderVoltage trip recal point (SOC%).");
			else if(reg == 6)
				term_appendString("3.6: OverVoltage trip recal point (SOC%).");
			else if(reg == 7)
				term_appendString("3.7: UnderVoltage warning recal point (SOC%).");
			else if(reg == 8)
				term_appendString("3.8: OverVoltage warning recal point (SOC%).");
			else if(reg == 9)
				term_appendString("3.9: default initialize value (SOC%).");
			return;
		}
		else if(group == 4)		//limits group
		{
			if(reg == 1)
				term_appendString("4.1: cell overvoltage limit (units = tenths of mv; 30000 = 3.000 volts).");
			else if(reg == 2)
				term_appendString("4.2: cell undervoltage limit (units = tenths of mv; 30000 = 3.000 volts).");
			else if(reg == 3)
				term_appendString("4.3: pack overcurrent (charge) limit (units = tenths of Amps; +1500 = 150 A charge).");
			else if(reg == 4)
				term_appendString("4.4: Pack overcurrent (discharge) limit (units = tenths of Amps; -1500 = 150A discharge <NEGATIVE SIGN MANDATORY>).");
			else if(reg == 5)
				term_appendString("4.5: cell overtemp limit on charge (units = tenths of Kelvin; 30000 = 300.0K).");
			else if(reg == 6)
				term_appendString("4.6: cell undertemp limit on charge (units = tenths of Kelvin; 30000 = 300.0K).");	
			else if(reg == 7)
				term_appendString("4.7: cell overtemp limit on discharge (units = tenths of Kelvin; 30000 = 300.0K).");
			else if(reg == 8)
				term_appendString("4.8: cell undertemp limit on discharge (units = tenths of Kelvin; 30000 = 300.0K).");
			else if(reg == 9)
				term_appendString("4.9: cell overvoltage warning (units = tenths of mv; 30000 = 3.000 volts).");
			else if(reg == 10)
				term_appendString("4.10: cell undervoltage warning (units = tenths of mv; 30000 = 3.000 volts).");
			else if(reg == 11)
				term_appendString("4.11: pack overcurrent (charge) warning (units = tenths of Amps; +1500 = 150 A charge).");
			else if(reg == 12)
				term_appendString("4.12: Pack overcurrent (discharge) warning (units = tenths of Amps; -1500 = 150A discharge <NEGATIVE SIGN MANDATORY>).");
			else if(reg == 13)
				term_appendString("4.13: cell overtemp warning on charge (units = tenths of Kelvin; 30000 = 300.0K).");
			else if(reg == 14)
				term_appendString("4.14: cell undertemp warning on charge (units = tenths of Kelvin; 30000 = 300.0K).");	
			else if(reg == 15)
				term_appendString("4.15: cell overtemp warning on discharge (units = tenths of Kelvin; 30000 = 300.0K).");
			else if(reg == 16)
				term_appendString("4.16: cell undertemp warning on discharge (units = tenths of Kelvin; 30000 = 300.0K).");;		
			return;
		}
		else if(group == 5)		//analog calibration group
		{
			if(reg == 1)
				term_appendString("5.1: low range current sensor gain.");
			else if(reg == 2)
				term_appendString("5.2: low range current sensor offset. <signed>");
			else if(reg == 3)
				term_appendString("5.3: high range current sensor gain.");
			else if(reg == 4)
				term_appendString("5.4: high range current sensor offset. <signed>");
			else if(reg == 5)
				term_appendString("5.5: high-low current transition point.");
			else if(reg == 6)
				term_appendString("5.6: X-axis Accelerometer gain.");
			else if(reg == 7)
				term_appendString("5.7: X-axis Accelerometer offset. <signed>");
			else if(reg == 8)
				term_appendString("5.8: Y-axis Accelerometer gain.");
			else if(reg == 9)
				term_appendString("5.9: Y-axis Accelerometer offset. <signed>");
			else if(reg == 10)
				term_appendString("5.10: Z-axis Accelerometer gain.");
			else if(reg == 11)
				term_appendString("5.11: Z-axis Accelerometer offset. <signed>");
			else if(reg == 12)
				term_appendString("5.12: current sensor board +15V rail-sense gain.");
			else if(reg == 13)
				term_appendString("5.13: current sensor board +15V rail-sense offset. <signed>");
			else if(reg == 14)
				term_appendString("5.14: current sensor board -15V rail-sense. gain");
			else if(reg == 15)
				term_appendString("5.15: current sensor board -15V rail-sense. <signed>");
			else if(reg == 16)
				term_appendString("5.16: current sensor board offset voltage-sense gain.");
			else if(reg == 17)
				term_appendString("5.17: current sensor board offset voltage-sense offset <signed>");
			else if(reg == 18)
				term_appendString("5.18: master's 3.3V supply-sense gain.");
			else if(reg == 19)
				term_appendString("5.19: master's 3.3V supply-sense offset. <signed>");
			else if(reg == 20)
				term_appendString("5.20: master's 5V supply-sense gain.");
			else if(reg == 21)
				term_appendString("5.21: master's 5V supply-sense offset. <signed>");
			else if(reg == 22)
				term_appendString("5.22: contactor coil voltage-sense gain.");
			else if(reg == 23)
				term_appendString("5.23: contactor coil voltage-sense offset. <signed>");
			else if(reg == 24)
				term_appendString("Current sensor polarity (0 = normal, 1 = reversed).");
			return;
		}
	}
	else if (type == 1)	//slave
	{
		if(group == 1)		//config
		{
			if(reg == 1)
				term_appendString("1.1: number of cells on this slave (voltages)");
			else if(reg == 2)
				term_appendString("1.2: number of cels on this slave (temperatures)");
			else if(reg == 3)
				term_appendString("1.3: slave ID (for programmatically assigned slave address when DIPsw swt to 0)");
			else if(reg == 4)
				term_appendString("1.4: RS485 bus inter-byte timeout <timer register value>");
			else if(reg == 5)
				term_appendString("1.5: LTC chip conversion wait time <timer register value>");
			else if(reg == 6)
				term_appendString("1.6: ADC/LTC polling time <timer register value>");
			else if(reg == 7)
				term_appendString("1.7: Balance Timeout <# of LTC cycles>");
			else if(reg == 8)
				term_appendString("1.8: LTC chip type (2 = LTC6802, 3 = LTC6803)");
			else if(reg == 9)
				term_appendString("1.9: LTC check PEC on receive (0 = ignore PEC, 1 = reject LTC data with invalid CRC)");
			else if(reg == 10)
				term_appendString("1.10: auto-start converting (1 = start converting volts/temps on boot, 0 = slave waits to be turned on)");
			return;
		}
		else if (group == 2)	//volts gain
		{
			if(reg == 1)
				term_appendString("2.1: LTC voltage gain cell 1");
			else if(reg == 2)
				term_appendString("2.2: LTC voltage gain cell 2");
			else if(reg == 3)
				term_appendString("2.3: LTC voltage gain cell 3");
			else if(reg == 4)
				term_appendString("2.4: LTC voltage gain cell 4");
			else if(reg == 5)
				term_appendString("2.5: LTC voltage gain cell 5");
			else if(reg == 6)
				term_appendString("2.6: LTC voltage gain cell 6");
			else if(reg == 7)
				term_appendString("2.7: LTC voltage gain cell 7");
			else if(reg == 8)
				term_appendString("2.8: LTC voltage gain cell 8");
			else if(reg == 9)
				term_appendString("2.9: LTC voltage gain cell 9");
			else if(reg == 10)
				term_appendString("2.10: LTC voltage gain cell 10");
			else if(reg == 11)
				term_appendString("2.11: LTC voltage gain cell 11");
			else if(reg == 12)
				term_appendString("2.13: LTC voltage gain cell 12");
			return;
		}
		else if (group == 3)		//volts offset
		{
			if(reg == 1)
				term_appendString("3.1: LTC voltage offset cell 1 <signed>");
			else if(reg == 2)
				term_appendString("3.2: LTC voltage offset cell 2 <signed>");
			else if(reg == 3)
				term_appendString("3.3: LTC voltage offset cell 3 <signed>");
			else if(reg == 4)
				term_appendString("3.4: LTC voltage offset cell 4 <signed>");
			else if(reg == 5)
				term_appendString("3.5: LTC voltage offset cell 5 <signed>");
			else if(reg == 6)
				term_appendString("3.6: LTC voltage offset cell 6 <signed>");
			else if(reg == 7)
				term_appendString("3.7: LTC voltage offset cell 7 <signed>");
			else if(reg == 8)
				term_appendString("3.8: LTC voltage offset cell 8 <signed>");
			else if(reg == 9)
				term_appendString("3.9: LTC voltage offset cell 9 <signed>");
			else if(reg == 10)
				term_appendString("3.10: LTC voltage offset cell 10 <signed>");
			else if(reg == 11)
				term_appendString("3.11: LTC voltage offset cell 11 <signed>");
			else if(reg == 12)
				term_appendString("3.13: LTC voltage offset cell 12 <signed>");
			return;
		}
		else if (group == 4)	//temp gain
		{
			if(reg == 1)
				term_appendString("4.1: temperature sensor gain cell 1");
			else if(reg == 2)
				term_appendString("4.2: temperature sensor gain cell 2");
			else if(reg == 3)
				term_appendString("4.3: temperature sensor gain cell 3");
			else if(reg == 4)
				term_appendString("4.4: temperature sensor gain cell 4");
			else if(reg == 5)
				term_appendString("4.5: temperature sensor gain cell 5");
			else if(reg == 6)
				term_appendString("4.6: temperature sensor gain cell 6");
			else if(reg == 7)
				term_appendString("4.7: temperature sensor gain cell 7");
			else if(reg == 8)
				term_appendString("4.8: temperature sensor gain cell 8");
			else if(reg == 9)
				term_appendString("4.9: temperature sensor gain cell 9");
			else if(reg == 10)
				term_appendString("4.10: temperature sensor gain cell 10");
			else if(reg == 11)
				term_appendString("4.11: temperature sensor gain cell 11");
			else if(reg == 12)
				term_appendString("4.13: temperature sensor gain cell 12");
			return;
		}
		else if (group == 5)		//temp offset
		{
			if(reg == 1)
				term_appendString("5.1: temperature sensor offset cell 1 <signed>");
			else if(reg == 2)
				term_appendString("5.2: temperature sensor offset cell 2 <signed>");
			else if(reg == 3)
				term_appendString("5.3: temperature sensor offset cell 3 <signed>");
			else if(reg == 4)
				term_appendString("5.4: temperature sensor offset cell 4 <signed>");
			else if(reg == 5)
				term_appendString("5.5: temperature sensor offset cell 5 <signed>");
			else if(reg == 6)
				term_appendString("5.6: temperature sensor offset cell 6 <signed>");
			else if(reg == 7)
				term_appendString("5.7: temperature sensor offset cell 7 <signed>");
			else if(reg == 8)
				term_appendString("5.8: temperature sensor offset cell 8 <signed>");
			else if(reg == 9)
				term_appendString("5.9: temperature sensor offset cell 9 <signed>");
			else if(reg == 10)
				term_appendString("5.10: temperature sensor offset cell 10 <signed>");
			else if(reg == 11)
				term_appendString("5.11: temperature sensor offset cell 11 <signed>");
			else if(reg == 12)
				term_appendString("5.13: temperature sensor offset cell 12 <signed>");
			return;
		}
		return;
	}
	else if (type == 2)	//bootstrap
	{
		if(group == 1)		//CONFIG (currently blank)
		{
			return;
		}
		else if(group == 2)		//sensor gain
		{
			if(reg == 1)
				term_appendString("2.1: Top bootstrap module current gain.");
			else if(reg == 2)
				term_appendString("2.2: Top charger supply voltage-sensing gain.");
			else if(reg == 3)
				term_appendString("2.3: Top bootstrap module voltage gain.");
			else if(reg == 4)
				term_appendString("2.4: Top bootstrap module temperature gain.");
			else if(reg == 5)
				term_appendString("2.5: Bottom bootstrap module current gain.");
			else if(reg == 6)
				term_appendString("2.6: Bottom charger supply voltage-sensing gain.");
			else if(reg == 7)
				term_appendString("2.7: Bottom bootstrap module voltage gain.");
			else if(reg == 8)
				term_appendString("2.8: Bottom bootstrap module temperature gain.");
			else if(reg == 9)
				term_appendString("2.9: 5V-sense gain.");
			else if(reg == 10)
				term_appendString("2.10: slave Bus Vsup-sense gain.");
			else if(reg == 11)
				term_appendString("2.11: +15V sense gain.");
			else if(reg == 12)
				term_appendString("2.12: -15V sense gain.");
			else if(reg == 13)
				term_appendString("2.13: Bootstrap slave logic board temperature sensor gain.");
			return;
		}
		else if(group == 3)  	//sensor offset
		{
			if(reg == 1)
				term_appendString("3.1: Top bootstrap module current offset. <signed>");
			else if(reg == 2)
				term_appendString("3.2: Top charger supply voltage-sensing offset. <signed>");
			else if(reg == 3)
				term_appendString("3.3: Top bootstrap module voltage offset. <signed>");
			else if(reg == 4)
				term_appendString("3.4: Top bootstrap module temperature offset. <signed>");
			else if(reg == 5)
				term_appendString("3.5: Bottom bootstrap module current offset. <signed>");
			else if(reg == 6)
				term_appendString("3.6: Bottom charger supply voltage-sensing offset. <signed>");
			else if(reg == 7)
				term_appendString("3.7: Bottom bootstrap module voltage offset. <signed>");
			else if(reg == 8)
				term_appendString("3.8: Bottom bootstrap module temperature offset. <signed>");
			else if(reg == 9)
				term_appendString("3.9: 5V-sense offset. <signed>");
			else if(reg == 10)
				term_appendString("3.10: slave Bus Vsup-sense offset. <signed>");
			else if(reg == 11)
				term_appendString("3.11: +15V sense offset. <signed>");
			else if(reg == 12)
				term_appendString("3.12: -15V sense offset. <signed>");
			else if(reg == 13)
				term_appendString("3.13: Bootstrap slave logic board temperature sensor offset. <signed>");
			return;
		}
		return;
	}
	else
		return;
}


//newline

