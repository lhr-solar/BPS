import csv
import random
import os

"""
This module generates Voltage and Temperature
values as they would be read by the LTC6811s
"""

# path/name of file
file = "BSP/Simulator/DataGeneration/Data/SPI.csv"

# sensor values
wires = []                  # list of 31 modules (1 = connected; 0 = open)
voltage_values = []         # list of 31 modules (fixed point 0.01)
temperature_values = []     # list of 31 pairs of sensors (Celsius)


def open_wires(battery=None):
    """
    @brief generate list of wire statuses (open/closed)
    @param battery : battery object with modules
    """
    global wires
    if battery is not None:
        wires = [1 if module.connected else 0 for module in battery.modules]
    else:
        wires = [1 for i in range(31)]


def random_voltage(mode):
    """
    @brief generate randomized voltage values
           in a specified range
    @param mode : 'low', 'normal', or 'high' ranges
    """
    global voltage_values
    if mode == 'low':
        voltage_values = [random.randint(250, 270) for i in range(31)]
    elif mode == 'normal':
        voltage_values = [random.randint(270, 400) for i in range(31)]
    elif mode == 'high':
        voltage_values = [random.randint(400, 420) for i in range(31)]


def specific_voltage(battery):
    """
    @brief generate list of voltage values based on battery object
    @param battery : battery object with module voltages
    """
    global voltage_values
    voltage_values = [int(module.voltage*100) for module in battery.modules]


def random_temperature(state, mode):
    """
    @brief generate randomized temperature values in specified ranges
    @param state : 'charging' or 'discharging'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    """
    global temperature_values
    if state == 'charging':
        if mode == 'low':
            temperature_values = [(random.randint(10, 25), random.randint(10, 25)) for i in range(31)]
        elif mode == 'normal':
            temperature_values = [(random.randint(25, 48), random.randint(25, 48)) for i in range(31)]
        elif mode == 'high':
            temperature_values = [(random.randint(48, 60), random.randint(48, 60)) for i in range(31)]
    elif state == 'discharging':
        if mode == 'low':
            temperature_values = [(random.randint(10, 30), random.randint(10, 30)) for i in range(31)]
        elif mode == 'normal':
            temperature_values = [(random.randint(30, 73), random.randint(30, 73)) for i in range(31)]
        elif mode == 'high':
            temperature_values = [(random.randint(73, 90), random.randint(73, 90)) for i in range(31)]


def generate(state, mode, battery=None):
    """
    @brief create csv file with voltage and temperature data
        Function called by simulate.py
    @param state : 'charging' or 'discharging'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    @param battery : battery object with set modules
    """
    global wires, voltage_values, temperature_values
    os.makedirs(os.path.dirname(file), exist_ok=True)
    open_wires(battery)
    if battery is not None:
        specific_voltage(battery)
        random_temperature('discharging', 'normal')
    else:
        random_voltage(mode)
        random_temperature(state, mode)
    with open(file, 'w+') as csvfile:
        csvwriter = csv.writer(csvfile)
        row = []
        for i in range(len(voltage_values)):
            row.append(wires[i])
            row.append(voltage_values[i])
            row.append(temperature_values[i][0])
            row.append(temperature_values[i][1])
            csvwriter.writerow(row)
            row = []


def read():
    """
    @brief read csv file with wire, voltage, and temperature data
        Function called by simulate.py
    @return list of all data
        Format: [wire status, voltage*100, temperature sensor 1, temperature sensor 2]
            for each of 31 modules
    """
    values = []
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            for i in range(len(row)):
                row[i] = int(row[i])
            values.append(row)
    return values
