import curses
import time
import battery
import ADC
import Lights

# Global configurations
state = ''  # Charging/Discharging
mode = ''   # Low, Normal, High
stdscr = None   # Output screen
lights_names = ['EXTRA', 'CAN', 'WDOG', 'UVOLT', 'OVOLT', 'OTEMP', 'OCURR', 'RUN', 'FAULT']


def generate(battery=None):
    global state, mode
    if battery is not None:
        # Update battery's state
        battery.update()
        # Generate ADC values
        ADC.generate(state, mode, battery.current)
    else:
        # Generate ADC values
        ADC.generate(state, mode)


def display(battery=None):
    global stdscr
    if battery is not None:
        stdscr.addstr(5, 0, battery.__str__())
    # Read ADC values
    adc_values = ADC.read()
    stdscr.addstr(0, 0, "Simulator")
    stdscr.addstr(1, 0, "=============================")
    stdscr.addstr(2, 0, f"ADC:")
    stdscr.addstr(2, 10, f"Low Precision: {adc_values[0][0]}")
    stdscr.addstr(3, 10, f"High Precision: {adc_values[0][1]}")
    # Read Current values
    stdscr.addstr(4, 0, f"Current:")
    stdscr.addstr(4, 10, f"{adc_values[1]} A ")
    # Read LED values
    lights = Lights.read()
    for i in range(0, 9):
        stdscr.addstr(i+2, 40, lights_names[i])
        if lights & (0x1<<i):
            stdscr.addstr(i+2, 50, "[X]")
        else:
            stdscr.addstr(i+2, 50, "[ ]")
    stdscr.refresh()



def configure():
    # Get configuration settings
    global state, mode
    print("Welcome to the BPS Simulator")
    print("Would you like to simulate 'charging' or 'discharging'?")
    state = input()
    while state != 'charging' and state != 'discharging':
        print("That is not a valid option. Please enter 'charging' or 'discharging': ")
        state = input()
    print("Would you like to simulate 'low', 'normal', or 'high' values?")
    mode = input()
    while mode != 'low' and mode != 'normal' and mode != 'high':
        print("That is not a valid option. Please enter 'low', 'normal', or 'high': ")
        mode = input()


def main():
    print("Welcome to the BPS Simulator")
    print("Type 'start' to start BeVolt. Otherwise, you can specify the types of data to simulate.")
    if input() == 'start':
        BeVolt = battery.Battery(30, 2950*434)
    else:
        BeVolt = None
        configure()
    global stdscr
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()
    while True:
        try:
            # Generate all values
            generate(BeVolt)
            display(BeVolt)
            time.sleep(1)     # one second delay
        except KeyboardInterrupt:
            curses.endwin()
            if BeVolt is not None:
                break
            print("\n\rWould you like to change 'config' or 'quit'?")
            choice = input()
            if choice == 'config':
                configure()
                stdscr = curses.initscr()
            elif choice == 'quit':
                break
            else:
                print("That is not a valid option. Continuing simulation...")
        except Exception:
            break
    curses.echo()
    curses.nocbreak()
    curses.endwin()


if __name__ == '__main__':
    main()
