/*--------------------------------------------------------------------

  Title     : Header file for eeprom_rw.c  
  Filename	: eeprom_rw.h 
    
----------------------------------------------------------------------*/

#ifndef EEPROM_RW_H
#define EEPROM_RW_H

//#include "posix_types.h"

/*--- Eeprom function prototypes ---*/

void WriteWord(unsigned int address, unsigned int data);
unsigned int ReadWord(unsigned int address);

#endif

/*--- End of File ---*/
