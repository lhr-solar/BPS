import csv
import random

"""
This module will generate ADC data to simulate Current readings
and store the simulated data in ADC.csv to be read by BSP_ADC.c
"""

# path/name of file
file = "Data/ADC.csv"

# randomized current value
current_value = 0

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


def random_adc(state, mode):
    """
    @brief generates random ADC values
    @param state : either 'charging' or 'discharging'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    @return tuple (low_precision_adc, high_precision_adc, current)
    """
    global current_value
    if state == 'charging':
        if mode == 'low':
            current_value = random.randint(-20, -15)
        elif mode == 'normal':
            current_value = random.randint(-15, -5)
        elif mode == 'high':
            current_value = random.randint(-5, 0)
    elif state == 'discharging':
        if mode == 'low':
            current_value = random.randint(0, 30)
        elif mode == 'normal':
            current_value = random.randint(30, 50)
        elif mode == 'high':
            current_value = random.randint(50, 100)
    return (amps_to_adc(current_value, 'l'), amps_to_adc(current_value, 'h'))
    

def specific_adc(current):
    """
    @brief generates specific ADC values
    @param current : current in Amperes to generate data for
    @return tuple (low_precision_adc, high_precision_adc)
    """
    return (amps_to_adc(current, 'l'), amps_to_adc(current, 'h'))


def generate(state, mode, current=None):
    """
    @brief create csv file with randomized ADC data
        Function called by simulate.py
    @param state : either 'charging' or 'discharging'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    @return correct current value
    """
    global current_value
    if current is None:
        with open(file, 'w') as csvfile:
            csvwriter = csv.writer(csvfile)
            csvwriter.writerow(list(random_adc(state, mode)))
    else:
        with open(file, 'w') as csvfile:
            csvwriter = csv.writer(csvfile)
            csvwriter.writerow(list(specific_adc(current)))
        current_value = current

def read():
    """
    @brief read csv file with ADC data
        Function called by simulate.py
    @return list of ADC data & Current data
        Format: [(low_precision_adc, high_precision_adc), current in Amps]
    """
    values = []
    with open(file, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            values.append(row)
    values.append(current_value)
    return values
