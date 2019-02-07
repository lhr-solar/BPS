#ifndef REGISTERS_H
#define REGISTERS_H

#define REGS_PER_GROUP 128
#define REGS_MAX_GROUP 5

unsigned int reg_read(unsigned char group, unsigned char reg);
unsigned char reg_write(unsigned char group, unsigned char reg, unsigned int value);
unsigned char reg_invalid(unsigned char group, unsigned char reg);

#endif
