#!/usr/bin/env python3

import os
import serial
import readline
import keyboard

#class cli
CLI_CHAR_BASE = 33 # base ASCII value we will encode numbers from; range of encodable numbers is 0 to 94; 0 maps to 33 and 94 maps to 127

CLI_VOLTS = chr(CLI_CHAR_BASE + 0)
CLI_AMPS = chr(CLI_CHAR_BASE + 1)
CLI_TEMP = chr(CLI_CHAR_BASE + 2)
CLI_CHARGE = chr(CLI_CHAR_BASE + 3)
CLI_DISP = chr(CLI_CHAR_BASE + 4)
CLI_WDOG = chr(CLI_CHAR_BASE + 5)
CLI_EEPROM = chr(CLI_CHAR_BASE + 6)
CLI_ADC = chr(CLI_CHAR_BASE + 7)
CLI_OWIRE = chr(CLI_CHAR_BASE + 8)
CLI_FAULT = chr(CLI_CHAR_BASE + 9)
CLI_ALL = chr(CLI_CHAR_BASE + 10)
CLI_CRIT = chr(CLI_CHAR_BASE + 11)
CLI_CAN = chr(CLI_CHAR_BASE + 12)
CLI_LTC = chr(CLI_CHAR_BASE + 13)
CLI_CONTACTOR = chr(CLI_CHAR_BASE + 14)
CLI_PING = chr(CLI_CHAR_BASE + 15)
CLI_ERR = chr(CLI_CHAR_BASE + 16)
CLI_EXIT = chr(CLI_CHAR_BASE + 17)

NUM_BATT_MODULES = 31

# Open Serial port, check board is available, flush buffers, and print interface
#    def __init__(self, uartPort="/dev/ttyUSB0"):
def init(loopback):
    port = "/dev/ttyUSB0"
    if loopback:
        uart = serial.serial_for_url('loop://', timeout=1)
    else:
        uart = serial.Serial(port=port, baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=None, xonxoff=0, rtscts=0)
    print(uart.name)
#    while uart.readline() != "i":
#        pass

#    uart.reset_input_buffer()
#    uart.reset_output_buffer()

    #    print("\n\r\n\r%s\n\r\n\r" % (utsvt_logo));
    print("Hello! Welcome to the BPS System! I am your worst nightmare...")
    print("Please enter a command (Type 'help' to see a list of commands)")
    return uart

#    def __str__(self):
#        return f"{self.uart}"
    
def cli_help():
	print("-------------------------Help Menu-------------------------")
	print("The available commands are:")
	print("Voltage\t\t\tCurrent\t\t\tTemperature")
	print("Contactor/Switch\tCharge\t\t\tLights/LED")
	print("CAN\t\t\tEEPROM\t\t\tDisplay")
	print("LTC/Register\t\tWatchdog\t\tADC")
	print("Critical/Abort\t\topenwire\t\tAll")
	print("Keep in mind: all values are 1-indexed")
	print("-----------------------------------------------------------")

def cli_uartWrite(uartPort, data):
    uartPort.write(bytes(data, encoding="ascii"))

def cli_uartRead(uartPort):
    return uartPort.readline()

def cli_ping(tokens):
    return CLI_PING

def cli_volts(toks):
    volt_cmd = CLI_VOLTS

    # request list of module voltages
    if(len(toks) == 1 or toks[1] == 'all'):
        volt_cmd += chr(CLI_CHAR_BASE + 0)
    # request voltage of specific module
    elif(toks[1].isnumeric()):
        arg1 = int(toks[1])
        if(arg1 > 0 and arg1 <= NUM_BATT_MODULES):
            volt_cmd += chr(CLI_CHAR_BASE + arg1)
        else:
            print("Invalid voltage request: bad module id")
            volt_cmd = CLI_ERR
    # request total voltage of battery system
    elif(toks[1] == 'total'):
        volt_cmd += chr(CLI_CHAR_BASE + NUM_BATT_MODULES + 1)
    # request temp safety info
    elif(toks[1] == 'safe'):
        volt_cmd += chr(CLI_CHAR_BASE + NUM_BATT_MODULES + 2)
    else:
        print("Invalid voltage request")
        volt_cmd = CLI_ERR

    return volt_cmd

def cli_amps(toks):
    amps_cmd = CLI_AMPS

    # request list of module voltages

    if (len(toks) == 1 or toks[1] == 'total'):
        amps_cmd += chr(CLI_CHAR_BASE + 0)
    elif(toks[1] == 'charge'):
        amps_cmd += chr(CLI_CHAR_BASE + 1)
    elif(toks[1] == 'safe'):
        amps_cmd += chr(CLI_CHAR_BASE + 2)
    else:
        print("Invalid current request")
        amps_cmd = CLI_ERR

    return amps_cmd

def cli_temp(toks):
    temp_cmd = CLI_AMPS

    # request list of module temps
    if(len(toks) == 1 or toks[1] == 'all'):
        temp_cmd += chr(CLI_CHAR_BASE + 0)
    # request temp of specific module
    elif(toks[1].isnumeric()):
        arg1 = int(toks[1])
        if(arg1 > 0 and arg1 <= NUM_BATT_MODULES):
            temp_cmd += chr(CLI_CHAR_BASE + arg1)
        else:
            print("Invalid temperature request: bad module id")
            temp_cmd = CLI_ERR
    # request total temp of battery system
    elif(toks[1] == 'total'):
        temp_cmd += chr(CLI_CHAR_BASE + NUM_BATT_MODULES + 1)
    # request temp reading from each sensor
    elif(toks[1] == 'sensors'):
        temp_cmd += chr(CLI_CHAR_BASE + NUM_BATT_MODULES + 2)
    else:
        print("Invalid temperature request")
        temp_cmd = CLI_ERR

    return temp_cmd

def cli_charge(toks):
    charge_cmd = CLI_CHARGE

    return charge_cmd

def cli_can(toks):
    can_cmd = CLI_CAN

    return can_cmd

def cli_adc(toks):
    adc_cmd = CLI_ADC

    return adc_cmd

def cli_openwire(toks):
    openwire_cmd = CLI_OWIRE

    return openwire_cmd

def cli_all(toks):
    all_cmd = CLI_ALL

    return all_cmd

def cli_exit(toks):
    return CLI_EXIT

def cli_parseUserInput():
    in_str = input(os.getlogin() + "@BPS$ ")
    if(len(in_str) == 0):
        return CLI_ERR

    in_toks = in_str.split()
    if(len(in_toks) == 0):
        in_toks = [in_str] 

    
#     if in_tokens == "partytime":
# 		print("")
#         for(int i = 0; i < 100; i++):
# 			print("%s" % (party_parrot_frames[i%10]))
#             for(int j = 0; j < 18 && i < 99; j++): 
# 					print("\033[A\r")
# 	}
# 
    cmd = CLI_ERR

    if in_toks[0] == "ping":
        return cli_ping()
    elif in_toks[0] == "help":
        cli_help()
    elif in_toks[0] == "volts":
        return cli_volts(in_toks)
    elif in_toks[0] == "amps":
        return cli_amps(in_toks)
    elif in_toks[0] == "temp":
        return cli_temp(in_toks)
    elif in_toks[0] == "charge":
        return cli_charge(in_toks)
    elif in_toks[0] == "adc":
        return cli_adc(in_toks)
    elif in_toks[0] == "openwire":
        return cli_openwire(in_toks)
    elif in_toks[0] == "all":
        return cli_all(in_toks)
    elif in_toks[0] == "can" or in_toks[0] == "canbus":
        return cli_can(in_toks)
#    elif in_toks[0] == "ltc" or in_toks[0] == "register":
#        return cli_help()
#    elif in_toks[0] == "switch" or in_toks[0] == "contactor":
#        return cli_help()
#    elif in_toks[0] == "display":
#        return cli_help()
#    elif in_toks[0] == "watchdog":
#        return cli_help()
#    elif in_toks[0] == "eeprom":
#        return cli_help()
#    elif in_toks[0] == "fault":
#        return cli_help()
#    elif in_toks[0] == "critical" or in_toks[0] == "abort":
#        return cli_help()
    elif in_toks[0] == "exit":
        return cli_exit(in_toks)
    else:
        print("Invalid command. Type 'help' or 'menu' for the help menu")
        
    return CLI_ERR

def cli_execCmd(uart, cmd):
    if (cmd == CLI_EXIT):
        uart.close()
        exit()
    
    if (cmd != CLI_ERR):
        uart.write(bytes(cmd + '\n\r', encoding="ascii"))
        result = uart.read_until('\n\r').decode("ascii")
        print(":".join("{:02x}".format(ord(c)) for c in result))

# Main function
boardUart = init(1)
#cliHistFile = os.path.expanduser("~/.bps-console-history")
#readline.add_history("cli history init")
#readline.read_history_file(cliHistFile)
#keyboard.on_press_key("up arrow", lambda _: print(cliHistFile.get_history_item(0)))

while (1):
    cmd = cli_parseUserInput()

    cli_execCmd(boardUart, cmd)

#    if (cmd == CLI_EXIT):
#        close
#        exit()
#    
#    #print(cmd)
#    if (cmd != CLI_ERR):
#        print(":".join("{:02x}".format(ord(c)) for c in cmd))
        
#    in_str = input(">> ")
#    in_toks = in_str.split()

    #if (cmd != CLI_ERR):
       # boardUart.write(bytes(cmd + '\r', encoding="ascii"))
       # rx_str = ""
       # c = boardUart.read().decode("ascii")
       # while (c != '\r'):
       #     rx_str = rx_str + c
       #     c = boardUart.read().decode("ascii")
       # 
       # print(rx_str)
        
