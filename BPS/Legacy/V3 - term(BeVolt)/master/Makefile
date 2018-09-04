# MPLAB IDE generated this makefile for use with GNU make.
# Project: master.mcp
# Date: Wed May 20 19:09:23 2015

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

output/master.hex : output/master.cof
	$(HX) "output\master.cof"

output/master.cof : output/ADC.o output/BLACKBOX.o output/I2Cdev.o output/main.o output/misc.o output/RS422.o output/RS485master.o output/RTC.o output/SOC.o output/terminal.o output/termregnames.o output/I2C.o output/eeprom_rw.o output/Mregisters.o output/send.o output/term_util.o output/balancing.o output/LCD.o output/CAN.o output/LCDutil.o
	$(CC) -mcpu=30F6012A "output\ADC.o" "output\BLACKBOX.o" "output\I2Cdev.o" "output\main.o" "output\misc.o" "output\RS422.o" "output\RS485master.o" "output\RTC.o" "output\SOC.o" "output\terminal.o" "output\termregnames.o" "output\I2C.o" "output\eeprom_rw.o" "output\Mregisters.o" "output\send.o" "output\term_util.o" "output\balancing.o" "output\LCD.o" "output\CAN.o" "output\LCDutil.o" -o"output\master.cof" -Wl,--script="..\buildFiles\p30F6012A.gld",--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=__ICD2RAM=1,-Map="output\master.map",--report-mem

output/ADC.o : Mregisters.h ADC.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h ADC.c
	$(CC) -mcpu=30F6012A -x c -c "ADC.c" -o"output\ADC.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/BLACKBOX.o : BLACKBOX.c
	$(CC) -mcpu=30F6012A -x c -c "BLACKBOX.c" -o"output\BLACKBOX.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/I2Cdev.o : I2C.h I2Cdev.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h I2Cdev.c
	$(CC) -mcpu=30F6012A -x c -c "I2Cdev.c" -o"output\I2Cdev.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/main.o : CAN.h LCD.h balancing.h Mregisters.h terminal.h send.h RS485master.h RS422.h misc.h SOC.h RTC.h I2C.h I2Cdev.h ADC.h time.h globals.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h main.c
	$(CC) -mcpu=30F6012A -x c -c "main.c" -o"output\main.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/misc.o : misc.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h misc.c
	$(CC) -mcpu=30F6012A -x c -c "misc.c" -o"output\misc.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/RS422.o : send.h misc.h RTC.h Mregisters.h terminal.h RS485master.h RS422.h globals.h time.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h RS422.c
	$(CC) -mcpu=30F6012A -x c -c "RS422.c" -o"output\RS422.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/RS485master.o : RS485master.h globals.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h RS485master.c
	$(CC) -mcpu=30F6012A -x c -c "RS485master.c" -o"output\RS485master.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/RTC.o : I2C.h I2Cdev.h RTC.h time.h RTC.c
	$(CC) -mcpu=30F6012A -x c -c "RTC.c" -o"output\RTC.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/SOC.o : Mregisters.h time.h RTC.h globals.h SOC.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h SOC.c
	$(CC) -mcpu=30F6012A -x c -c "SOC.c" -o"output\SOC.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/terminal.o : termregnames.h RS422.h Mregisters.h RS485master.h term_util.h terminal.h RTC.h misc.h SOC.h globals.h time.h terminal.c
	$(CC) -mcpu=30F6012A -x c -c "terminal.c" -o"output\terminal.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/termregnames.o : termregnames.h term_util.h termregnames.c
	$(CC) -mcpu=30F6012A -x c -c "termregnames.c" -o"output\termregnames.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/I2C.o : I2C.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h I2C.c
	$(CC) -mcpu=30F6012A -x c -c "I2C.c" -o"output\I2C.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/eeprom_rw.o : c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/inc/p30f6012a.inc ../common/eeprom_rw.s
	$(CC) -mcpu=30F6012A -c -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 "..\common\eeprom_rw.s" -o"output\eeprom_rw.o" -Wa,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g

output/Mregisters.o : Mregisters.h ../common/eeprom_rw.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h Mregisters.c
	$(CC) -mcpu=30F6012A -x c -c "Mregisters.c" -o"output\Mregisters.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/send.o : Mregisters.h RS485master.h RS422.h globals.h send.c
	$(CC) -mcpu=30F6012A -x c -c "send.c" -o"output\send.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/term_util.o : terminal.h globals.h term_util.h term_util.c
	$(CC) -mcpu=30F6012A -x c -c "term_util.c" -o"output\term_util.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/balancing.o : Mregisters.h RS485master.h globals.h balancing.h balancing.c
	$(CC) -mcpu=30F6012A -x c -c "balancing.c" -o"output\balancing.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/LCD.o : Mregisters.h RS422.h LCDutil.h globals.h LCD.h LCD.c
	$(CC) -mcpu=30F6012A -x c -c "LCD.c" -o"output\LCD.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/CAN.o : Mregisters.h RS485master.h globals.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/dsPIC30F/h/p30F6012A.h CAN.h CAN.c
	$(CC) -mcpu=30F6012A -x c -c "CAN.c" -o"output\CAN.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

output/LCDutil.o : globals.h LCDutil.h LCDutil.c
	$(CC) -mcpu=30F6012A -x c -c "LCDutil.c" -o"output\LCDutil.o" -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -g -Wall

clean : 
	$(RM) "output\ADC.o" "output\BLACKBOX.o" "output\I2Cdev.o" "output\main.o" "output\misc.o" "output\RS422.o" "output\RS485master.o" "output\RTC.o" "output\SOC.o" "output\terminal.o" "output\termregnames.o" "output\I2C.o" "output\eeprom_rw.o" "output\Mregisters.o" "output\send.o" "output\term_util.o" "output\balancing.o" "output\LCD.o" "output\CAN.o" "output\LCDutil.o" "output\master.cof" "output\master.hex"

