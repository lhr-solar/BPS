#!/usr/bin/env python3

import serial

CLI_VOLTS = 1
CLI_AMPS = 2
CLI_TEMP = 3
CLI_CHARGE = 4
CLI_DISP = 5
CLI_WDOG = 6
CLI_EEPROM = 7
CLI_ADC = 8
CLI_OWIRE = 9
CLI_FAULT = 10
CLI_ALL = 11
CLI_CRIT = 12
CLI_CAN = 13
CLI_LTC = 14
CLI_CONTACTOR = 15
CLI_PING = 16

UART_BAUD_RATE = 115200

# Open Serial port, check board is available, flush buffers, and print interface
def cli_init():
    uart = serial.Serial("/dev/ttyUSB0", UART_BAUD_RATE)
    print(uart.name)
#    while uart.readline() != "i":
#        pass

#    uart.reset_input_buffer()
#    uart.reset_output_buffer()

    #    print("\n\r\n\r%s\n\r\n\r" % (utsvt_logo));
    print("Hello! Welcome to the BPS System! I am your worst nightmare...")
    print("Please enter a command (Type 'help' to see a list of commands)")

    return uart

def cli_help():
	print("-------------------------Help Menu-------------------------");
	print("The available commands are:");
	print("Voltage\t\t\tCurrent\t\t\tTemperature");
	print("Contactor/Switch\tCharge\t\t\tLights/LED");
	print("CAN\t\t\tEEPROM\t\t\tDisplay");
	print("LTC/Register\t\tWatchdog\t\tADC");
	print("Critical/Abort\t\topenwire\t\tAll");
	print("Keep in mind: all values are 1-indexed");
	print("-----------------------------------------------------------");

def cli_uartWrite(uartPort, data):
    uartPort.write(bytes(data, encoding="ascii"))

def cli_uartRead(uartPort):
    return uartPort.readline()

def cli_ping(uartPort):
    cli_uartWrite(uartPort, str(CLI_PING))
    print(cli_uartRead(uartPort))

def cli_voltage(toks):
    print(toks)

def cli_adc(toks):
    print(toks)


def cli_parseUserInput(uartPort):
    in_str = input(">> ")
    in_toks = in_str.split()
    
#     if in_tokens == "partytime":
# 		print("")
#         for(int i = 0; i < 100; i++):
# 			print("%s" % (party_parrot_frames[i%10]))
#             for(int j = 0; j < 18 && i < 99; j++): 
# 					print("\033[A\r")
# 	}
# 
    
    if in_toks[0] == "ping":
        cli_ping(uartPort)
    elif in_toks[0] == "help":
        cli_help()
    elif in_toks[0] == "voltage":
        cli_voltage(in_toks[1:])
    elif in_toks[0] == "current":
        cli_help()
    elif in_toks[0] == "temperature":
        cli_help()
    elif in_toks[0] == "charge":
        cli_help()
    elif in_toks[0] == "display":
        cli_help()
    elif in_toks[0] == "watchdog":
        cli_help()
    elif in_toks[0] == "eeprom":
        cli_help()
    elif in_toks[0] == "adc":
        cli_adc()
    elif in_toks[0] == "openwire":
        cli_help()
    elif in_toks[0] == "fault":
        cli_help()
    elif in_toks[0] == "all":
        cli_help()
    elif in_toks[0] == "critical" or in_toks[0] == "abort":
        cli_help()
    elif in_toks[0] == "can" or in_toks[0] == "canbus":
        cli_help()
    elif in_toks[0] == "ltc" or in_toks[0] == "register":
        cli_help()
    elif in_toks[0] == "switch" or in_toks[0] == "contactor":
        cli_help()
    elif in_toks[0] == "exit":
        exit()
    else:
        printf("Invalid command. Type 'help' or 'menu' for the help menu");

# Main function
boardUart = cli_init()
while (1):
    print(cli_uartRead(boardUart))
#    cli_parseUserInput(boardUart)
