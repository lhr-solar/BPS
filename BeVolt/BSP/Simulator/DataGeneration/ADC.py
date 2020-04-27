import csv
import random

"""
This module will generate ADC data to simulate Current readings
"""

# path/name of file
file = "Data/ADC.csv"

def amps_to_adc(current, sensor):
    """
    @brief converts current in Amperes to ADC 12-bit value
    @param current : Current value in Amperes to onvert
    @param sensor : 'h' (high precision) or 'l' (low precision)
    @return 12-bit ADC value representative of the current
    """
    if sensor == 'h':
        # Map [-50A, 50A] -> [-4V, 4V]
        hall_effect = current * 4/50
    elif sensor == 'l':
        # Map [-100A, 100A] -> [-4V, 4V]
        hall_effect = current * 4/100
    # Caps the values at +-4V
    if hall_effect > 4:
        hall_effect = 4
    elif hall_effect < -4:
        hall_effect = -4
    # Map [-4V, 4V] -> [0V, 3.3V]
    opamp_gain = 3
    opamp_offset = 4.096
    adc_voltage = (hall_effect + opamp_offset) / opamp_gain * 1000  # mV

    # Convert mV to 12-bit value
    adc_data = (int(adc_voltage) << 12) / 3300
    return int(adc_data)


def generate_adc(state, mode):
    """
    @brief generates random ADC values
    @param state : either 'charge' or 'discharge'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    @return tuple (low_precision_adc, high_precision_adc)
    """
    if state == 'charge':
        if mode == 'low':
            return (amps_to_adc(random.randint(-20, -15), 'l'), amps_to_adc(random.randint(-20, -15), 'h'))
        elif mode == 'normal':
            return (amps_to_adc(random.randint(-15, -5), 'l'), amps_to_adc(random.randint(-15, -5), 'h'))
        elif mode == 'high':
            return (amps_to_adc(random.randint(-5, 0), 'l'), amps_to_adc(random.randint(-5, 0), 'h'))
    elif state == 'discharge':
        if mode == 'low':
            return (amps_to_adc(random.randint(0, 30), 'l'), amps_to_adc(random.randint(0, 30), 'h'))
        elif mode == 'normal':
            return (amps_to_adc(random.randint(30, 50), 'l'), amps_to_adc(random.randint(30, 50), 'h'))
        elif mode == 'high':
            return (amps_to_adc(random.randint(50, 100), 'l'), amps_to_adc(random.randint(50, 100), 'h'))
    

def main():
    print(generate_adc('discharge', 'normal'))

if __name__ == '__main__':
    main()
