import curses
import time
import battery
import ADC
import Lights
import SPI

# Global configurations
state = ''  # Charging/Discharging
mode = ''   # Low, Normal, High
stdscr = None   # Output screen
lights_names = ['EXTRA', 'CAN', 'WDOG', 'UVOLT', 'OVOLT', 'OTEMP', 'OCURR', 'RUN', 'FAULT']


def generate(battery=None):
    global state, mode
    # Update battery's state
    if battery is not None:
        battery.update()
    # Generate ADC values
    ADC.generate(state, mode, battery)
    # Generate SPI values
    SPI.generate(state, mode, battery)


def display(battery=None):
    global stdscr
    # Read ADC values
    adc_values = ADC.read()
    stdscr.addstr(0, 10, "Battery")
    stdscr.addstr(1, 0, "==============================")
    stdscr.addstr(2, 0, f"ADC:")
    stdscr.addstr(2, 10, f"Low Precision: {adc_values[0][0]}")
    stdscr.addstr(3, 10, f"High Precision: {adc_values[0][1]}")
    # Read Current values
    stdscr.addstr(4, 0, f"Current:")
    stdscr.addstr(4, 10, f"{adc_values[1]} A ")
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
    stdscr.refresh()



def configure():
    # Get configuration settings
    global state, mode
    print("Would you like to simulate 'charging' or 'discharging'?")
    print(">>", end="")
    state = input()
    while state != 'charging' and state != 'discharging':
        print("That is not a valid option. Please enter 'charging' or 'discharging': ")
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


def main():
    print("Welcome to the BPS Simulator")
    print("Type 'start' to start BeVolt. Otherwise, you can specify the types of data to simulate.")
    print(">>", end="")
    if input() == 'start':
        BeVolt = battery.Battery(30, 2950*434, 2900*434)
    else:
        BeVolt = None
        configure()
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
                print("\n\rWould you like to change 'wires' or 'quit'?")
                print(">>", end="")
                choice = input()
                if choice == 'wires':
                    change_wires(BeVolt)
                    stdscr = curses.initscr()
                    curses.start_color()
                elif choice == 'quit':
                    break
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
            print("ERROR:", end=" ")
            print(e, end="\r\n")
            print("If addwstr() returned ERR, make your terminal window bigger.")
            break
    curses.echo()
    curses.nocbreak()
    curses.endwin()


if __name__ == '__main__':
    main()
