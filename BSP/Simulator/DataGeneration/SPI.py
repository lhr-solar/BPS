import logging
import csv
import random
import os
import threading
import fcntl
import config
import ltc6811

"""
This module generates Voltage and Temperature
values as they would be read by the LTC6811s
"""

# path/name of file
# POV is of BSP_SPI, not SPI.py so that's why read_file is path of SPIW.csv
# and vice versa
read_file = config.directory_path + config.files['SPIW']
write_file = config.directory_path + config.files['SPIR']

# sensor values
wires = [1] * 31                # list of 31 modules (1 = connected; 0 = open)
voltage_values = [0]*31         # list of 31 modules (fixed point 0.0001)
temperature_values = [[0,0]]*31   # list of 31 pairs of sensors (fixed point 0.001)
DCC = [0] * 31                  # list of the DCC bits for the 31 modules (1 = discharging, 0 = not discharging)

# LTC6811 objects
minions = []

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
        voltage_values = [random.randint(25000, 27000) for i in range(31)]
    elif mode == 'normal':
        voltage_values = [random.randint(27000, 40000) for i in range(31)]
    elif mode == 'high':
        voltage_values = [random.randint(40000, 42000) for i in range(31)]


def specific_voltage(battery):
    """
    @brief generate list of voltage values based on battery object
           with +-1.2mV error
    @param battery : battery object with module voltages
    """
    global voltage_values
    voltage_values = [int((module.voltage*10000)+random.randint(-12, 12)) for module in battery.modules]


def random_temperature(state, mode):
    """
    @brief generate randomized temperature values in specified ranges
    @param state : 'charging' or 'discharging'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    """
    global temperature_values
    if state == 'charging':
        if mode == 'low':
            temperature_values = [(random.randint(10000, 25000), random.randint(10000, 25000)) for i in range(31)]
        elif mode == 'normal':
            temperature_values = [(random.randint(25000, 48000), random.randint(25000, 48000)) for i in range(31)]
        elif mode == 'high':
            temperature_values = [(random.randint(48000, 60000), random.randint(48000, 60000)) for i in range(31)]
    elif state == 'discharging':
        if mode == 'low':
            temperature_values = [(random.randint(10000, 30000), random.randint(10000, 30000)) for i in range(31)]
        elif mode == 'normal':
            temperature_values = [(random.randint(30000, 72000), random.randint(30000, 72000)) for i in range(31)]
        elif mode == 'high':
            temperature_values = [(random.randint(73000, 90000), random.randint(73000, 90000)) for i in range(31)]


def specific_temperature(battery):
    """
    @brief generate list of temperature values based on battery object
    @param battery : battery object with module voltages
    """
    global temperature_values
    temperature_values = [(int(module.temperatures_cel[0] * 1000),
                            int(module.temperatures_cel[1] * 1000))
                            for module in battery.modules]


st = 'discharging'
md = 'normal'
batt = None
def init(state='discharging', mode='normal', battery=None):
    global st, md, batt
    os.makedirs(os.path.dirname(read_file), exist_ok=True)
    os.makedirs(os.path.dirname(write_file), exist_ok=True)
    # Create files
    f_r = open(read_file, 'w')
    f_r.close()
    f_w = open(write_file, 'w')
    f_w.close()

    st = state
    md = mode
    batt = battery
    
    for i in range(4):
        minions.append(ltc6811.LTC6811(battery))

def generate(state, mode, battery=None):
    """
    @brief create csv file with voltage and temperature data
        Function called by simulate.py
    @param state : 'charging' or 'discharging'
    @param mode : 'low', 'normal', or 'high' ranges respective to the state
    @param battery : battery object with set modules
    """
    global wires, voltage_values, temperature_values
    open_wires(battery)
    if battery is not None:
        specific_voltage(battery)
        random_temperature('discharging', 'normal')
    else:
        random_voltage(mode)
        random_temperature(state, mode)

    # Scale by 10 because LTC handles returns 0.0001 fixed pt resolution
    for a,b in temperature_values:
        a *= 10
        b *= 10

    temperature_values_copy = temperature_values.copy()
    voltage_values_copy = voltage_values.copy()
    wires_copy = wires.copy()

    minion_idx = 0
    for minion in minions:
        voltage_partition = []
        temperature_partition = []
        wires_partition = []

        for i in range(8):
            voltage_partition.append(voltage_values_copy.pop(0))
            temperature_partition.append(temperature_values_copy.pop(0))
            wires_partition.append(wires_copy.pop(0))

            if len(voltage_values_copy) == 0 or len(temperature_values_copy) == 0 or len(wires_copy) == 0:
                break

        for i in range(4):
            voltage_partition.append(0)
            wires_partition.append(1)

        # Append extra
        if minion_idx == 3:
            voltage_partition.append(0)
            temperature_partition.append([0, 0])
            wires_partition.append(1)

        minion.set_temperatures(temperature_partition)
        minion.set_voltage(voltage_partition)
        minion.set_wires(wires_partition)

        minion_idx += 1

    # Record if file is not empty
    try:
        with open(read_file, 'r+') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file

            # If file is empty, then do nothing
            filesize = os.path.getsize(read_file)
            if filesize == 0:
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
                return

            # Get message
            message = []
            csvreader = csv.reader(csvfile)
            for row in csvreader:
                for i in range(len(row)):
                    message.append(int(row[i]))
                break

            ltc6811.parse_full_protocol(minions, message)
            csvfile.truncate(0)     # Delete the command to indicate simulator has resolved it
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file

        with open(write_file, 'r+') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
            csvwriter = csv.writer(csvfile)
            message = ltc6811.format_full_protocol(minions)
            if message is not None:
                # Write
                csvwriter.writerow(message)
            else:
                # Delete contents
                csvfile.truncate(0)
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
    except:    
        pass 
    balance_batteries()

def balance_batteries():
    global DCC, minions
    dcc_idx = 0
    for i in range(0, 4):
        #read each minion's tx_data field
        for k in range(0, 8):
            #store dcc bits into the global list
            DCC[dcc_idx] = (minions[i].tx_data[4] & (1 << k)) >> k
            if dcc_idx == 30:   #there are only 31 modules
                break
            else:
                dcc_idx += 1


def spi_task():
    logging.info('Starting SPI Task')
    while True:
        generate('discharging', 'normal', None)

spi_thread = threading.Thread(target=spi_task)


def read():
    """
    @brief read csv file with wire, voltage, and temperature data
        Function called by simulate.py
    @return list of all data
        Format: [wire status, voltage*10000, temperature sensor 1 * 1000, temperature sensor 2 * 1000]
            for each of 31 modules
    """
    values = []
    row = []
    for i in range(len(voltage_values)):
        row.append(wires[i])
        row.append(voltage_values[i])
        row.append(temperature_values[i][0])
        row.append(temperature_values[i][1])
        row.append(DCC[i])
        values.append(row)
        row = []
    return values










# TEST
UNIT_TEST = 0
if __name__ == "__main__":


    if UNIT_TEST:
        # Test SPI functions
        import battery
        BeVolt = battery.Battery(30, config.total_batt_pack_capacity_mah, 2500 * config.num_batt_cells_parallel_per_module)
        module_values = read()
        for i, module in enumerate(module_values):
            print("| {0} | {1:.4f}V | {2:.3f}°C | {3:.3f}°C |".format('X' if module[0] else ' ', module[1]/10000, module[2]/1000, module[3]/1000))

        # Test LTC6811 instantiation
        for ic in range(4):
            batt_modules = []
            lim = 8

            # Last ic in chain only monitors 7 battery modules
            if ic == 3:
                lim = 7

            for batt in range(lim):
                batt_modules.append(BeVolt.modules[ic * 8 + batt])

            minions.append(ltc6811.LTC6811(batt_modules))
        print()

        # Test PEC
        print("PEC Test")
        ltc6811.pec_init()
        print(hex(ltc6811.calc_pec([0x00, 0x01])))
        print()

        # Test LTC6811 static methods
        print("LTC6811 Static Methods Test")
        minions[0].tx_data = [0, 1, 2, 3, 4, 5]
        minions[1].tx_data = [6, 7, 8, 9, 10, 11]
        minions[2].tx_data = [12, 13, 14, 15, 16, 17]
        minions[3].tx_data = [18, 19, 20, 21, 22, 23]
        for minion in minions:
            minion.tx_available = 1
        protocol = ltc6811.format_full_protocol(minions)
        ltc6811.print_protocol(protocol)

        # Add command at the front + cmd_pec
        pec = ltc6811.calc_pec([0x00, 0x01])
        protocol.insert(0, 0x00)
        protocol.insert(1, 0x01)
        protocol.insert(2, (pec >> 8) & 0x00FF)    # PEC0
        protocol.insert(3, pec & 0x00FF)           # PEC1
        ltc6811.parse_full_protocol(minions, protocol)

        protocol.clear()
        pec = ltc6811.calc_pec([0x00, 0x02])
        protocol.insert(0, 0x00)
        protocol.insert(1, 0x02)
        protocol.insert(2, (pec >> 8) & 0x00FF)    # PEC0
        protocol.insert(3, pec & 0x00FF)           # PEC1
        ltc6811.parse_full_protocol(minions, protocol)
        # 
        print(minions[0])
        print(minions[1])
        print(minions[2])
        print(minions[3])

        generate('discharging', 'normal', BeVolt)

    else:
        import time
        init()

        while True:
            generate('discharging', 'normal', None)
            