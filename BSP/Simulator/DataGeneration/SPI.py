import csv
import random
import os
import fcntl
import config

"""
This module generates Voltage and Temperature
values as they would be read by the LTC6811s
"""

# path/name of file
file = config.directory_path + config.files['SPI']

# sensor values
wires = []                  # list of 31 modules (1 = connected; 0 = open)
voltage_values = []         # list of 31 modules (fixed point 0.0001)
temperature_values = []     # list of 31 pairs of sensors (fixed point 0.001)

# LTC6811 objects
minions = []

# dictionary of all command codes
command_codes = {
    'SIM_LTC6811_WRCFGA'    : {'code': 0x001, 'handler': None},
    'SIM_LTC6811_WRCFGB'    : {'code': 0x024, 'handler': None},
    'SIM_LTC6811_RDCFGA'    : {'code': 0x002, 'handler': None},
    'SIM_LTC6811_RDCFGB'    : {'code': 0x026, 'handler': None},
    'SIM_LTC6811_RDCVA'     : {'code': 0x004, 'handler': None},
    'SIM_LTC6811_RDCVB'     : {'code': 0x006, 'handler': None},
    'SIM_LTC6811_RDCVC'     : {'code': 0x008, 'handler': None},
    'SIM_LTC6811_RDCVD'     : {'code': 0x00A, 'handler': None},
    'SIM_LTC6811_RDCVE'     : {'code': 0x009, 'handler': None},
    'SIM_LTC6811_RDCVF'     : {'code': 0x00B, 'handler': None},
    'SIM_LTC6811_RDAUXA'    : {'code': 0x00C, 'handler': None},
    'SIM_LTC6811_RDAUXB'    : {'code': 0x00E, 'handler': None},
    'SIM_LTC6811_RDAUXC'    : {'code': 0x00D, 'handler': None},
    'SIM_LTC6811_RDAUXD'    : {'code': 0x00F, 'handler': None},
    'SIM_LTC6811_RDSTATA'   : {'code': 0x010, 'handler': None},
    'SIM_LTC6811_RDSTATB'   : {'code': 0x012, 'handler': None},
    'SIM_LTC6811_WRSCTRL'   : {'code': 0x014, 'handler': None},
    'SIM_LTC6811_WRPWM'     : {'code': 0x020, 'handler': None},
    'SIM_LTC6811_WRPSB'     : {'code': 0x01C, 'handler': None},
    'SIM_LTC6811_RDSCTRL'   : {'code': 0x016, 'handler': None},
    'SIM_LTC6811_RDPWM'     : {'code': 0x022, 'handler': None},
    'SIM_LTC6811_RDPSB'     : {'code': 0x01E, 'handler': None},
    'SIM_LTC6811_STSCTRL'   : {'code': 0x019, 'handler': None},
    'SIM_LTC6811_CLRSCTRL'  : {'code': 0x018, 'handler': None},
    'SIM_LTC6811_ADCV'      : {'code': 0x260, 'handler': None},       # 0 1 MD[1] MD[0] 1 1 DCP 0 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPU'    : {'code': 0x268, 'handler': None},       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPD'    : {'code': 0x228, 'handler': None},       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_CVST'      : {'code': 0x207, 'handler': None},       # 0 1 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADOL'      : {'code': 0x201, 'handler': None},       # 0 1 MD[1] MD[0] 0 0 DCP 0 0 0 1
    'SIM_LTC6811_ADAX'      : {'code': 0x460, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_ADAXD'     : {'code': 0x400, 'handler': None},       # 1 0 MD[1] MD[0] 0 0 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_AXST'      : {'code': 0x407, 'handler': None},       # 1 0 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADSTAT'    : {'code': 0x468, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_ADSTATD'   : {'code': 0x468, 'handler': None},       # 1 0 MD[1] MD[0] 0 0 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_STATST'    : {'code': 0x40F, 'handler': None},       # 1 0 MD[1] MD[0] ST[1] ST[0] 0 1 1 1 1
    'SIM_LTC6811_ADCVAX'    : {'code': 0x46F, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 DCP 1 1 1 1
    'SIM_LTC6811_ADCVSC'    : {'code': 0x467, 'handler': None},       # 1 0 MD[1] MD[0] 1 1 DCP 0 1 1 1
    'SIM_LTC6811_CLRCELL'   : {'code': 0x711, 'handler': None},
    'SIM_LTC6811_CLRAUX'    : {'code': 0x712, 'handler': None},
    'SIM_LTC6811_CLRSTAT'   : {'code': 0x713, 'handler': None},
    'SIM_LTC6811_PLADC'     : {'code': 0x714, 'handler': None},
    'SIM_LTC6811_DIAGN'     : {'code': 0x715, 'handler': None},
    'SIM_LTC6811_WRCOMM'    : {'code': 0x721, 'handler': None},
    'SIM_LTC6811_RDCOMM'    : {'code': 0x722, 'handler': None},
    'SIM_LTC6811_STCOMM'    : {'code': 0x723, 'handler': None}
}

mux_addresses = {
    'SIM_LTC1380_MUX1' : 0x90,        # MUX addresses on Minion boards
    'SIM_LTC1380_MUX2' : 0x92
}

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
    temperature_values = [(int(module.temperatures_cel[0] * 1000), int(module.temperatures_cel[1] * 1000)) for module in battery.modules]


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
        specific_temperature(battery)
    else:
        random_voltage(mode)
        random_temperature(state, mode)


    # Replace with formatting
    with open(file, 'w+') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvwriter = csv.writer(csvfile)
        row = []
        for i in range(len(voltage_values)):
            row.append(wires[i])
            row.append(voltage_values[i])
            row.append(temperature_values[i][0])
            row.append(temperature_values[i][1])
            csvwriter.writerow(row)
            row = []
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file


def read():
    """
    @brief read csv file with wire, voltage, and temperature data
        Function called by simulate.py
    @return list of all data
        Format: [wire status, voltage*10000, temperature sensor 1 * 1000, temperature sensor 2 * 1000]
            for each of 31 modules
    """
    values = []
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            for i in range(len(row)):
                row[i] = int(row[i])
            values.append(row)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
    return values


class LTC6811:
    CRC15_POLY = 0x4599
    pec15_table = [0] * 256


    def __init__(self, batt_module):
        self.batt_modules = batt_modules

        # reg suffixes indicate data stored in LTC6811 registers
        self.config_reg = []    # 6B: Configuration register of LTC

        # staging buffers that will be used when formatting bus
        self.curr_cmd = 0       # 12-bits
        self.tx_data = []       # 6B: POV of simulator
        self.rx_data = []       # 6B

        # As long this is called once, pec will work
        LTC6811.pec_init()


    def __str__(self):
        print("Config: ", end='')
        for byte in self.config_reg:
            print(hex(byte), end=' ')


    def command_update(self, command, data):
        self.curr_cmd = command
        self.rx_data = data



    @staticmethod
    def format_full_protocol(ics):
        protocol = []

        for ic in ics:
            # Copy data
            for byte in ic.tx_data:
                protocol.append(byte)
            
            pec = LTC6811.calc_pec(ic.tx_data)
            protocol.append((pec >> 8) & 0x00FF)    # PEC0
            protocol.append(pec & 0x00FF)           # PEC1

        return protocol


    @staticmethod
    def parse_full_protocol(ics, protocol):
        DATA_START_IDX = 4

        # First to bytes are the command
        # The next two lines of code pops the first 4 bytes which are the cmd
        #   related bytes.
        cmd = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)
        cmd_pec = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)

        # Update all the LTCs with new cmd
        for ic in ics:
            data = [0] * 6
            for i in range(6):
                data[i] = protocol.pop(0)   # Pop the data

            # data_pec saved but not needed
            data_pec = ((protocol.pop(0) << 8) & 0xFF00) | (protocol.pop(0) & 0x00FF)

            # Update LTC
            ic.command_update(cmd, data)


    @staticmethod
    def print_protocol(protocol):
        print()
        print("Idx : Data")
        idx = 0
        for byte in protocol:
            print(" {0:2} : {1}".format(idx, hex(protocol[idx])))
            idx += 1


    @staticmethod
    def pec_init():
        for i in range(256):
            remainder = i << 7
            for bit in range(8, 0, -1):
                if remainder & 0x4000:
                    remainder = ((remainder << 1))
                    remainder = (remainder ^ LTC6811.CRC15_POLY)
                else:
                    remainder = ((remainder << 1))
            LTC6811.pec15_table[i] = (remainder&0x0000FFFF)


    @staticmethod
    def calc_pec(bytes):
        """
        @brief  calculates 16-bit PEC value of the byte array
        @param  bytes : array of bytes that the algorithm will use to calculate
                        PEC. For the LTC6811, the length of the array is either
                        2 (command) or 6 (data) bytes long
        """
        address = 0
        remainder = 16  # PEC seed
        for i in range(len(bytes)):
            address = ((remainder >> 7) ^ bytes[i]) & 0xff   #calculate PEC table address
            remainder = (remainder << 8 ) ^ LTC6811.pec15_table[address]
        return (remainder*2 & 0x0000FFFF) # The CRC15 has a 0 in the LSB so the final value must be multiplied by 2









# TEST
if __name__ == "__main__":
    # Test SPI functions
    import battery
    BeVolt = battery.Battery(30, config.total_batt_pack_capacity_mah, 2500 * config.num_batt_cells_parallel_per_module)
    generate('discharging', 'normal', BeVolt)
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

        minions.append(LTC6811(batt_modules))

    # Test PEC
    LTC6811.pec_init()
    print(hex(LTC6811.calc_pec([0x00, 0x01])))

    # Test LTC6811 static methods
    minions[0].tx_data = [0, 1, 2, 3, 4, 5]
    minions[1].tx_data = [6, 7, 8, 9, 10, 11]
    minions[2].tx_data = [12, 13, 14, 15, 16, 17]
    minions[3].tx_data = [18, 19, 20, 21, 22, 23]
    protocol = LTC6811.format_full_protocol(minions)
    LTC6811.print_protocol(protocol)

    # Add command at the front + cmd_pec
    pec = LTC6811.calc_pec([0x00, 0x01])
    protocol.insert(0, 0x00)
    protocol.insert(1, 0x01)
    protocol.insert(2, (pec >> 8) & 0x00FF)    # PEC0
    protocol.insert(3, pec & 0x00FF)           # PEC1

    LTC6811.parse_full_protocol(minions, protocol)

    for minion in minions:
        print(minion.rx_data)