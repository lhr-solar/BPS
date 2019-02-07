# MPLAB IDE generated this makefile for use with GNU make.
# Project: master.mcp
# Date: Mon Mar 04 01:10:25 2013

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

master.hex : master.cof
	$(HX) "master.cof"

master.cof : eeprom_rw.o main.o ADC.o I2C.o BLACKBOX.o RS422.o RS486master.o RTC.o registers.o misc.o SOC.o terminal.o termregnames.o I2C2.o I2Cdev.o
	$(CC) -mcpu=30F6012A "eeprom_rw.o" "main.o" "ADC.o" "I2C.o" "BLACKBOX.o" "RS422.o" "RS486master.o" "RTC.o" "registers.o" "misc.o" "SOC.o" "terminal.o" "termregnames.o" "I2C2.o" "I2Cdev.o" -o"master.cof" -Wl,--script="..\..\..\..\..\..\..\Program Files (x86)\Microchip\MPLAB C30\support\dsPIC30F\gld\p30F6012A.gld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,--defsym=__ICD2RAM=1,-Map="master.map",--report-mem

eeprom_rw.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/inc/p30f6012a.inc ../common/eeprom_rw.s
	$(CC) -mcpu=30F6012A -c -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 "..\common\eeprom_rw.s" -o"eeprom_rw.o" -Wa,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g

main.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h main.c
	$(CC) -mcpu=30F6012A -x c -c "main.c" -o"main.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

ADC.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h ADC.c
	$(CC) -mcpu=30F6012A -x c -c "ADC.c" -o"ADC.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

I2C.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h I2C.c
	$(CC) -mcpu=30F6012A -x c -c "I2C.c" -o"I2C.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

BLACKBOX.o : BLACKBOX.c
	$(CC) -mcpu=30F6012A -x c -c "BLACKBOX.c" -o"BLACKBOX.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

RS422.o : time.h ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h RS422.c
	$(CC) -mcpu=30F6012A -x c -c "RS422.c" -o"RS422.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

RS486master.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h RS486master.c
	$(CC) -mcpu=30F6012A -x c -c "RS486master.c" -o"RS486master.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

RTC.o : time.h RTC.c
	$(CC) -mcpu=30F6012A -x c -c "RTC.c" -o"RTC.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

registers.o : ../common/eeprom_rw.h ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h registers.c
	$(CC) -mcpu=30F6012A -x c -c "registers.c" -o"registers.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

misc.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h misc.c
	$(CC) -mcpu=30F6012A -x c -c "misc.c" -o"misc.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

SOC.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h SOC.c
	$(CC) -mcpu=30F6012A -x c -c "SOC.c" -o"SOC.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

terminal.o : time.h terminal.c
	$(CC) -mcpu=30F6012A -x c -c "terminal.c" -o"terminal.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

termregnames.o : termregnames.c
	$(CC) -mcpu=30F6012A -x c -c "termregnames.c" -o"termregnames.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

I2C2.o : ../../../../../../../program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h I2C2.c
	$(CC) -mcpu=30F6012A -x c -c "I2C2.c" -o"I2C2.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

I2Cdev.o : I2Cdev.c
	$(CC) -mcpu=30F6012A -x c -c "I2Cdev.c" -o"I2Cdev.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -g -Wall

clean : 
	$(RM) "eeprom_rw.o" "main.o" "ADC.o" "I2C.o" "BLACKBOX.o" "RS422.o" "RS486master.o" "RTC.o" "registers.o" "misc.o" "SOC.o" "terminal.o" "termregnames.o" "I2C2.o" "I2Cdev.o" "master.cof" "master.hex"

