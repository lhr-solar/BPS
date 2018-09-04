/*******************************************************
/registers.c
/implements the register memory/settings system
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>
#include "eeprom_rw.h"		//needed to access the ASM functions that read/write the EEPROM
#include "registers.h"

/*******************************************************
/reg_read
/returns value from group.reg as INT16
/returns 0 for unimplemented registers
*******************************************************/
unsigned int reg_read(unsigned char group, unsigned char reg)
{
	if (group > REGS_MAX_GROUP)
		return 0;
	if (reg > REGS_PER_GROUP)
		return 0;
	group--;
	return ReadWord(group*REGS_PER_GROUP + reg);	
}

/*******************************************************
/reg_write
/saves value (int16) into group.reg
/also checks the write interlock DIP switches
/returns 0 for OK, 1 for error(such as invalid group/reg), 2 for error such as locked config
*******************************************************/
unsigned char reg_write(unsigned char group, unsigned char reg, unsigned int value)
{
	if (group > REGS_MAX_GROUP)
		return 1;
	if (reg > REGS_PER_GROUP)
		return 1;
	//TODO: check config switches
	group--;
	WriteWord(group*REGS_PER_GROUP + reg, value);
	return 0;
}
/*******************************************************
/reg_valid
/checks if register is valid
/returns 1 for unimplemented registers
/return 0 for valid registers
*******************************************************/
unsigned char reg_invalid(unsigned char group, unsigned char reg)
{
	if (group > REGS_MAX_GROUP)
		return 1;
	if (reg > REGS_PER_GROUP)
		return 1;
	//TODO: more checking
	return 0;
}


//newline

