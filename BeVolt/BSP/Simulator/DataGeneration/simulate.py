import time
import ADC

# Global configurations
state = ''
mode = ''


def generate():
    global state, mode
    # Generate ADC values
    ADC.generate(state, mode)


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
    configure()
    while True:
        try:
            # Generate all values
            generate()
            read()
            time.sleep(1)     # one second delay
        except KeyboardInterrupt:
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
