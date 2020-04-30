import time
<<<<<<< HEAD
<<<<<<< HEAD
import battery
import ADC

# Global configurations
state = ''  # Charging/Discharging
mode = ''   # Low, Normal, High


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
=======
=======
import battery
>>>>>>> Create battery class to model Li-ion batteries
import ADC

# Global configurations
state = ''  # Charging/Discharging
mode = ''   # Low, Normal, High


def generate(battery=None):
    global state, mode
<<<<<<< HEAD
    # Generate ADC values
    ADC.generate(state, mode)
>>>>>>> Create script to generate and display new data every second. Tested with BPS code for ADC (high/low precision current readings)
=======
    if battery is not None:
        # Update battery's state
        battery.update()
        # Generate ADC values
        ADC.generate(state, mode, battery.current)
    else:
        # Generate ADC values
        ADC.generate(state, mode)
>>>>>>> Create battery class to model Li-ion batteries


def read():
    # Read ADC values
    adc_values = ADC.read()
    print("=============================")
    print(f"ADC:\tLow Precision: {adc_values[0][0]}\n\tHigh Precision: {adc_values[0][1]}")
    print(f"Current: {adc_values[1]} A")


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
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> Create battery class to model Li-ion batteries
    print("Welcome to the BPS Simulator")
    print("Type 'start' to start BeVolt. Otherwise, you can specify the types of data to simulate.")
    if input() == 'start':
        BeVolt = battery.Battery(30, 2950*434)
    else:
        BeVolt = None
        configure()
<<<<<<< HEAD
    while True:
        try:
            # Generate all values
            generate(BeVolt)
            read()
            print(BeVolt)
            time.sleep(1)     # one second delay
        except KeyboardInterrupt:
            if BeVolt is not None:
                break
=======
    configure()
=======
>>>>>>> Create battery class to model Li-ion batteries
    while True:
        try:
            # Generate all values
            generate(BeVolt)
            read()
            print(BeVolt)
            time.sleep(1)     # one second delay
        except KeyboardInterrupt:
<<<<<<< HEAD
>>>>>>> Create script to generate and display new data every second. Tested with BPS code for ADC (high/low precision current readings)
=======
            if BeVolt is not None:
                break
>>>>>>> Create battery class to model Li-ion batteries
            print("\n\rWould you like to change 'config' or 'quit'?")
            choice = input()
            if choice == 'config':
                configure()
            elif choice == 'quit':
                break
            else:
                print("That is not a valid option. Continuing simulation...")


if __name__ == '__main__':
    main()
