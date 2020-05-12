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
    global wires
    if battery is not None:
        wires = [1 if module.connected else 0 for module in battery.modules]
    else:
        wires = [1 for i in range(31)]


def random_voltage(mode):
    global voltage_values
    if mode == 'low':
        voltage_values = [random.randint(250, 270) for i in range(31)]
    elif mode == 'normal':
        voltage_values = [random.randint(270, 400) for i in range(31)]
    elif mode == 'high':
        voltage_values = [random.randint(400, 420) for i in range(31)]


def specific_voltage(battery):
    global voltage_values
    for i, module in enumerate(battery.modules):
        voltage_values[i] = module.voltage


def random_temperature(state, mode):
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
    global wires, voltage_values, temperature_values
    os.makedirs(os.path.dirname(file), exist_ok=True)
    random_temperature(state, mode)
    open_wires(battery)
    if battery is not None:
        specific_voltage(battery)
    else:
        random_voltage(mode)
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



if __name__ == '__main__':
    generate('discharging', 'normal')
