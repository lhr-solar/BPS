import curses
import time
import battery
import ADC
import Lights
import SPI
import Strobelight
import WDTimer
import PLL
import config

import os
import sys

# Global configurations
state = ''  # Charging/Discharging
mode = ''   # Low, Normal, High
stdscr = None   # Output screen
lights_names = ['EXTRA', 'CAN', 'WDOG', 'UVOLT', 'OVOLT', 'OTEMP', 'OCURR', 'RUN', 'FAULT']
frequency = None

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
    else:
        BeVolt = None
        configure()
    
    try:
        launch_bevolt()
    except Exception as e:
        print(repr(e))
    
    global stdscr
    stdscr = curses.initscr()
    curses.start_color()
    curses.noecho()
    curses.cbreak()
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
                print("\n\rWould you like to change 'wires', 'quit', or 'PLL'?")
                print(">>", end="")
                choice = input()
                if choice == 'wires':
                    change_wires(BeVolt)
                    stdscr = curses.initscr()
                    curses.start_color()
                elif choice == 'quit':
                    break
                elif choice == 'PLL':
                    print("Enter the frequency you would like to change the clock to in Hz.")
                    frequency = int(input())
                    PLL.Change_Frequency(frequency)
                else:
                    print("That is not a valid option. Continuing simulation...")
                    stdscr = curses.initscr()
                    curses.start_color()
            else:
                print("\n\rWould you like to change 'config' or 'quit'?")
                print(">>", end="")
                choice = input()
                if choice == 'config':
                    configure()
                    stdscr = curses.initscr()
                    curses.start_color()
                elif choice == 'quit':
                    break
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


if __name__ == '__main__':
    main()
