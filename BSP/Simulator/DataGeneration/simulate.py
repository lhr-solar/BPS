import curses
import textwrap
import time
import battery
import ADC
import Lights
import CAN
import SPI
import Strobelight
import Fans
import WDTimer
import PLL
import I2C
import logging

import config
import Timer

import os
import sys
import traceback

# Global configurations
state = ''  # Charging/Discharging
mode = ''   # Low, Normal, High
stdscr = None   # Output screen
CANbox = None   #box that displays the CAN messages
lights_names = ['EXTRA', 'CAN', 'WDOG', 'UVOLT', 'OVOLT', 'OTEMP', 'OCURR', 'RUN', 'FAULT']
frequency = None
maxEEPROMAddress = 0x3FFF

def generate(battery=None):
    global state, mode
    # Update battery's state
    if battery is not None:
        battery.update()
    # Generate ADC values
    ADC.generate(state, mode, battery)
    # Generate SPI values
    SPI.generate(state, mode, battery)
    #Pet Watchdog
    WDTimer.Check_State()
    #Initialize Watchdog Timer
    WDTimer.WD_Enable()
    #Initialize Timer
    Timer.Enable()


def display(battery=None):  #print watchdog countdown 
    global stdscr
    # Read ADC values
    adc_values = ADC.read()
    global frequency 
    frequency = PLL.Get_Frequency()

    stdscr.addstr(0, 10, "Battery")
    stdscr.addstr(1, 0, "==============================")
    stdscr.addstr(2, 0, f"ADC:")
    stdscr.addstr(2, 10, f"Low Precision: {adc_values[0][0]}")
    stdscr.addstr(3, 10, f"High Precision: {adc_values[0][1]}")
    # Read Current values
    stdscr.addstr(4, 0, f"Current:")
    stdscr.addstr(4, 10, f"{adc_values[1]} A ")
    #Read CAN data
    CANdata = CAN.Get_CAN_Info()
    text = ' '.join(CANdata) #put elements of the list of CAN data bytes into a string 
    CANbox.erase()  #clear previous data in the box
    CANbox.addstr(4, 0, textwrap.fill(text, 40))
    CANbox.addstr(3, 2, "CAN ID and Message:")
    # Display Watchdog ticks
    ticks = WDTimer.Tick()
    stdscr.addstr(10, 0, f"                              ") #clear previous tick
    stdscr.addstr(10, 0, f"WDTimer Countdown: {ticks}")
    #Display current frequency
    stdscr.addstr(6, 0, f"                                          ") 
    stdscr.addstr(6, 0, f"Clock Frequency: {frequency} Hz")
    # Read Module values
    stdscr.addstr(0, 54, "Modules")
    stdscr.addstr(1, 40, "====================================")
    module_values = SPI.read()
    for i, module in enumerate(module_values):
        stdscr.addstr(i+2, 37, f"{i+1}")
        stdscr.addstr(i+2, 40, f"| {'X' if module[0] else ' '} | {module[1]/10000:.4f}V | {module[2]/1000:.3f}°C | {module[3]/1000:.3f}°C |")
    # Read LED values
    stdscr.addstr(0, 90, "LEDs")
    stdscr.addstr(1, 80, "=======================")
    lights = Lights.read()
    curses.init_pair(1, curses.COLOR_RED, curses.COLOR_RED)
    curses.init_pair(2, curses.COLOR_GREEN, curses.COLOR_GREEN)
    curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_BLACK)
    for i in range(9):
        stdscr.addstr(i+2, 80, lights_names[i])
        if lights & (0x1<<i):
            if i == 7:
                stdscr.addstr(i+2, 100, "[]", curses.color_pair(2))
            else:
                stdscr.addstr(i+2, 100, "[]", curses.color_pair(1))
        else:
            stdscr.addstr(i+2, 100, "[]", curses.color_pair(3))
    strobe = Strobelight.read()

    stdscr.addstr(11, 80, 'S_PULSING')
    if strobe:
        stdscr.addstr(11, 100, "[]", curses.color_pair(2))
    else:
        stdscr.addstr(11, 100, "[]", curses.color_pair(3))
    speed = [1,2,3,4]#Fans.read()
    stdscr.addstr(12, 9, "Fan Speeds")
    stdscr.addstr(13, 0, "================================")
    stdscr.addstr(14, 0, f"Fan 1: {speed[0]}/8")
    stdscr.addstr(14, 15, f"Fan 2: {speed[1]}/8")
    stdscr.addstr(15, 0, f"Fan 3: {speed[2]}/8")
    stdscr.addstr(15, 15, f"Fan 4: {speed[3]}/8")
    stdscr.refresh()


def configure():
    # Get configuration settings
    global state, mode
    print("Would you like to simulate 'charging' or 'discharging'?")
    print(">>", end="")
    state = input()
    while state != 'charging' and state != 'discharging':
        print("That is not a valid option. Please enter 'charging' or 'discharging'")
        state = input()
    print("Would you like to simulate 'low', 'normal', or 'high' values?")
    print(">>", end="")
    mode = input()
    while mode != 'low' and mode != 'normal' and mode != 'high':
        print("That is not a valid option. Please enter 'low', 'normal', or 'high': ")
        mode = input()


def change_wires(battery):
    done = False
    while not done:
        print("Which module? (0 to exit)")
        print(">>", end="")
        module = int(input())
        if module:
            battery.modules[module-1].connected = not battery.modules[module-1].connected
        else:
            done = True

def launch_bevolt():
    # Suppress stdout and stderr
    null_fds = [os.open(os.devnull, os.O_RDWR) for x in range(2)]
    # save the current stdout and stderr
    save = os.dup(1), os.dup(2)
    # put /dev/null fds on 1 and 2
    os.dup2(null_fds[0], 1)
    os.dup2(null_fds[1], 2)

    # Launch the BPS code for BeVolt
    isParent = os.fork()
    if isParent == 0:   # we only want to do this in the child process
        os.execl("simulate", "simulate", "bevolt")
    
    os.dup2(save[0], 1)
    os.dup2(save[1], 2)
    
    os.close(null_fds[0])
    os.close(null_fds[1])
    os.close(save[0])
    os.close(save[1])

def main():
    print("Welcome to the BPS Simulator")
    print("Type 'start' to start BeVolt. Otherwise, you can specify the types of data to simulate.")
    print(">>", end="")
    if input() == 'start':
        # Initial capacity is (2500*14)/(2950*14)=0.847 i.e. 84.7% charged
        init_capacity_mah = 2500 * config.num_batt_cells_parallel_per_module

        # Amperes current draw of the electrical system
        ampere_draw = 30

        # Create state of the battery
        BeVolt = battery.Battery(ampere_draw, config.total_batt_pack_capacity_mah, init_capacity_mah)
        PLL.PLL_Init()
        SPI.init()
    else:
        BeVolt = None
        configure()
    
    try:
        launch_bevolt()
    except Exception as e:
        print(repr(e))
    
    logging.basicConfig(filename='debug.log',level=logging.DEBUG)

    global stdscr
    global CANbox
    stdscr = curses.initscr()
    curses.start_color()
    curses.noecho()
    curses.cbreak()
    #box is for CAN messages
    CANbox = curses.newwin(7, 21, 12, 78)
    CANbox.immedok(True)
    CANbox.box()
    CANbox.refresh()
    #Start background thread for timer 
    timerThread = Timer.timer_Thread
    timerThread.start()
    while True:
        try:
            # Generate all values
            generate(BeVolt)
            # Display all values
            display(BeVolt)
            time.sleep(1)     # one second delay
        except KeyboardInterrupt:
            curses.endwin()
            if BeVolt is not None:
                print("\n\rWould you like to change \n\r1. 'wires'\n\r2. 'quit'\n\r3. 'PLL'\n\r4. send a CAN message ('CAN')\n\r5. 'EEPROM'")
            else:
                print("\n\rWould you like to change\n\r1. 'config'\n\r2. 'quit'\n\r3. 'PLL'\n\r4. send a CAN message ('CAN')\n\r5. 'EEPROM'")
            print(">>", end="")
            choice = input()
            if (choice == 'wires' or choice == '1') and BeVolt is not None:
                change_wires(BeVolt)
                stdscr = curses.initscr()
                curses.start_color()
            elif (choice == 'config' or choice == '1') and BeVolt is None:
                configure()
                stdscr = curses.initscr()
            elif choice == 'quit' or choice == '2':
                break
            elif choice == 'PLL' or choice == '3':
                print("Enter the frequency you would like to change the clock to in Hz.")
                frequency = int(input())
                PLL.Change_Frequency(frequency)
            elif choice == 'CAN' or choice == '4':
                print("Enter the CAN ID for the system you wish to simulate. Leave out '0x'.")
                id = input()
                while(CAN.Invalid_CAN_ID(id) == True):
                    print("Invalid CAN ID. Try again.")
                    id = input()
                print("Enter up to 8 bytes of the CAN message that you would like to send, and separate each byte by a ','. Leave out '0x'.")
                message = input().split(',')
                CAN.Send_Message(id, message, len(message))
            elif choice == 'EEPROM' or choice == '5':
                returnErrorCodes = 0
                print("Enter 'all' for all data or 'read' to enter specific address to read.")
                print(">>", end="")
                choiceEEPROM1 = input()
                print("Enter 'raw' to read the raw hex values or 'msg' for the translated error messages.", end="\n")
                print("If invalid response is given, default is raw data.")
                print(">>", end="")
                choiceEEPROM2 = input()
                if choiceEEPROM2 == 'raw':
                    returnErrorCodes = 0
                elif choiceEEPROM2 == 'msg':
                    returnErrorCodes = 1
                else:
                    print("Invalid entry...", end="\n")
                    print("Defaulted to raw data.")
                if choiceEEPROM1 == 'all':
                    print(I2C.EEPROM_Dump(returnErrorCodes))
                    print("Enter to continue simulator:")
                    print(">>", end="")
                    choice = input()
                elif choiceEEPROM1 == 'read':
                    print("Enter address to start reading faults from (in hex format).")
                    print(">>", end="")
                    EEPROMstartAddress = input()
                    print("Enter address to stop reading faults from (in hex format).")
                    print(">>", end="")
                    EEPROMendAddress = input()
                    EEPROMstartAddress = int(EEPROMstartAddress, 16)
                    EEPROMendAddress = int(EEPROMendAddress, 16)
                    if EEPROMstartAddress >= 0 and EEPROMstartAddress <= maxEEPROMAddress and EEPROMendAddress >= 0 and EEPROMendAddress <= maxEEPROMAddress:
                        print(I2C.I2C_Read(EEPROMstartAddress,EEPROMendAddress, returnErrorCodes))
                        print("Enter to continue simulator:")
                        choiceEEPROM = input()
                    else:
                        print("Invalid address...", end="\n")
                        print("Enter to continue simulator:")
                        choiceEEPROM = input()
                else:
                    print("Invalid entry given for 1st choice (all/read)...", end="\n")
                    print("Enter to continue simulator:")
                    choiceEEPROM = input()
            else:
                print("That is not a valid option. Continuing simulation...")
                stdscr = curses.initscr()
                curses.start_color()
        except Exception as e:
            curses.echo()
            curses.nocbreak()
            curses.endwin()
            print("ERROR:", end=" ")
            print(repr(e), end="\r\n")
            exc_type, exc_obj, exc_tb = sys.exc_info()
            fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
            print(exc_type, fname, exc_tb.tb_lineno)
            print("If addwstr() returned ERR, make your terminal window bigger.")
            print("\n\rContinue? (Y/n): ", end="")
            cont = input()
            if(cont.lower() == "n" or cont.lower() == "no"):
                break
            print("Continuing...")
            main()
    curses.echo()
    curses.nocbreak()
    curses.endwin()
    Timer.terminate(True)



if __name__ == '__main__':
    try:
        main()
    except:
        traceback.print_exc()
        while True:
            pass
