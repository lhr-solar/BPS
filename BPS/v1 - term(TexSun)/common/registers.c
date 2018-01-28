/*******************************************************
/registers.c
/implements the register for the temp slave
/Fred Engelkemeir, summer 2012
*******************************************************/

#include <p30F6012A.h>
#include "..\common\eeprom_rw.h"

#define REGS_PER_GROUP 128
#define REGS_MAX_GROUP 5

unsigned int reg_read(unsigned char group, unsigned char reg);
unsigned char reg_write(unsigned char group, unsigned char reg, unsigned int value);
unsigned char reg_invalid(unsigned char group, unsigned char reg);


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
	unsigned char fail = 0;
	if (group > REGS_MAX_GROUP)
		return 1;
	if (reg > REGS_PER_GROUP)
		return 1;
	if(group == 1 && !misc_isSetupUnlocked())
		return 2;
	if(group == 2 && !misc_isCalUnlocked())
		return 2;
	if(group == 3 && !misc_isCalUnlocked())
		return 2;
	if(group == 4 && !misc_isCalUnlocked())
		return 2;
	if(group == 5 && !misc_isCalUnlocked())
		return 2;
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

