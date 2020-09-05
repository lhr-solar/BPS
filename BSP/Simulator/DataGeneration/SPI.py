import csv
import random
import os
import fcntl
import config

"""
This module generates Voltage and Temperature
values as they would be read by the LTC6811s
"""
NUM_MINIONS = 4
simulationData[4][6] = 0
#dictionary of all command codes
command_codes = {
    'SIM_LTC6811_WRCFGA' : 0x001,
    'SIM_LTC6811_WRCFGB' : 0x024,
    'SIM_LTC6811_RDCFGA' : 0x002,
    'SIM_LTC6811_RDCFGB' : 0x026,
    'SIM_LTC6811_RDCVA'  : 0x004,
    'SIM_LTC6811_RDCVB'  : 0x006,
    'SIM_LTC6811_RDCVC'  : 0x008,
    'SIM_LTC6811_RDCVD'  : 0x00A,
    'SIM_LTC6811_RDCVE'  : 0x009,
    'SIM_LTC6811_RDCVF'  : 0x00B,
    'SIM_LTC6811_RDAUXA' : 0x00C,
    'SIM_LTC6811_RDAUXB' : 0x00E,
    'SIM_LTC6811_RDAUXC' : 0x00D,
    'SIM_LTC6811_RDAUXD' : 0x00F,
    'SIM_LTC6811_RDSTATA': 0x010,
    'SIM_LTC6811_RDSTATB': 0x012,
    'SIM_LTC6811_WRSCTRL': 0x014,
    'SIM_LTC6811_WRPWM'  : 0x020,
    'SIM_LTC6811_WRPSB'  : 0x01C,
    'SIM_LTC6811_RDSCTRL': 0x016,
    'SIM_LTC6811_RDPWM'  : 0x022,
    'SIM_LTC6811_RDPSB'  : 0x01E,
    'SIM_LTC6811_STSCTRL': 0x019,
    'SIM_LTC6811_CLRSCTRL': 0x018,
    'SIM_LTC6811_ADCV'   : 0x260,         # 0 1 MD[1] MD[0] 1 1 DCP 0 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPU' : 0x268,       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_ADOWPD' : 0x228,       # 0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
    'SIM_LTC6811_CVST'   : 0x207,         # 0 1 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADOL'   : 0x201,         # 0 1 MD[1] MD[0] 0 0 DCP 0 0 0 1
    'SIM_LTC6811_ADAX'   : 0x460,         # 1 0 MD[1] MD[0] 1 1 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_ADAXD'  : 0x400,        # 1 0 MD[1] MD[0] 0 0 0 0 CHG[2] CHG[1] CHG[0]
    'SIM_LTC6811_AXST'   : 0x407,         # 1 0 MD[1] MD[0] ST[1] ST[0] 0 0 1 1 1
    'SIM_LTC6811_ADSTAT' : 0x468,       # 1 0 MD[1] MD[0] 1 1 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_ADSTATD': 0x468,       # 1 0 MD[1] MD[0] 0 0 0 1 CHST[2] CHST[1] CHST[0]
    'SIM_LTC6811_STATST' : 0x40F,        # 1 0 MD[1] MD[0] ST[1] ST[0] 0 1 1 1 1
    'SIM_LTC6811_ADCVAX' : 0x46F,       # 1 0 MD[1] MD[0] 1 1 DCP 1 1 1 1
    'SIM_LTC6811_ADCVSC' : 0x467,        # 1 0 MD[1] MD[0] 1 1 DCP 0 1 1 1
    'SIM_LTC6811_CLRCELL': 0x711,
    'SIM_LTC6811_CLRAUX' : 0x712,
    'SIM_LTC6811_CLRSTAT': 0x713,
    'SIM_LTC6811_PLADC'  : 0x714,
    'SIM_LTC6811_DIAGN'  : 0x715,
    'SIM_LTC6811_WRCOMM' : 0x721,
    'SIM_LTC6811_RDCOMM' : 0x722,
    'SIM_LTC6811_STCOMM' : 0x723
}

# path/name of file
file_w = config.directory_path + config.files['SPIW']
file_r = config.directory_path + config.files['SPIR']
file = config.directory_path + config.files['SPI']

# sensor values
wires = []                  # list of 31 modules (1 = connected; 0 = open)
voltage_values = []         # list of 31 modules (fixed point 0.0001)
temperature_values = []     # list of 31 pairs of sensors (fixed point 0.001)

pec15Table = [0] * 256 
CRC15_POLY = 0x4599

currentRow = 1  #the first SPI_Write will be written on the second row
currCmd = 0

#erases both SPI csv files and resets their counters
def SPI_Init():
    PEC15_Table_Init()
    os.makedirs(os.path.dirname(file_r), exist_ok=True)
    with open(file_r, 'w') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(["0"])
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
    
    os.makedirs(os.path.dirname(file_w), exist_ok=True)
    with open(file_w, 'w') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(["0"])
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
    
def PEC15_Table_Init():
    global pec15Table
    for i in range (0, 256): 
        remainder = i << 7
        bit = 8
        while(bit > 0):
            if(remainder & 0x4000): 
                remainder = ((remainder << 1))
                remainder = (remainder ^ CRC15_POLY)
            else:
                remainder = ((remainder << 1))
            bit = bit - 1
        pec15Table[i] = remainder & 0xFFFF

def PEC15_Calc(data , len):
    #uint16_t remainder,address
    remainder = 16; #PEC seed
    for i in range (0, len):
        address = ((remainder >> 7) ^ data[i]) & 0xff #calculate PEC table address
        remainder = (remainder << 8 ) ^ pec15Table[address]
    return (remainder*2) #The CRC15 has a 0 in the LSB so the final value must be multiplied by 2

def ExtractCmdFromBuff(buf, len):
    global currentRow, currCmd
    #read the row counter that is at the very beginning of the file
    os.makedirs(os.path.dirname(file_w), exist_ok=True)
    with open(file_w, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        firstRow = csvreader.next(csvreader)
        currentRow = firstRow[0]    #update currentRow
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file

    currentWrite = getNthRow(currentRow, file_w)    #get the latest data written into SPIW
    currCmd = int((currentWrite[0] << 8) | currentWrite[1])      #set currCmd to newest command
    return int((currentWrite[0] << 8) | currentWrite[1])

def ExtractDataFromBuff(data, buf, len) {
    BYTES_PER_REG = 6
    BYTES_PER_IC = 8   # Register size (6B) + PEC (2B)

    # Extract only the data so ignore PEC and command code
    for i in range(0, NUM_MINIONS): 
        #The +4 is because bytes [0:1] holds the command code and [2:3] holds
        #the PEC for the command code.
        memcpy(&data[i*BYTES_PER_REG], &buf[i*BYTES_PER_IC+4], BYTES_PER_REG)   #COME BACK TO THIS


def ExtractMUXAddrFromBuff(buf):
    BYTES_PER_REG = 8

    buf = buf + 4

    int minionIdx = 0
    i = NUM_MINIONS - 1 
    while(i>=0):
        i = i - 1
        simulationData[minionIdx].temperature_mux = ((buf[i*BYTES_PER_REG] << 4) & 0xF0) | ((buf[i*BYTES_PER_REG+1] >> 4) & 0x0F)
        minionIdx = minionIdx + 1

def ExtractMUXSelFromBuff(buf):    #buf was a pointer in C
    BYTES_PER_REG = 8

    buf = buf + 4

    int minionIdx = 0
    i = NUM_MINIONS - 1
    while(i >= 0):
        sel = ((buf[i*BYTES_PER_REG+3] >> 4) & 0x000F) - 8;     # Check LTC1380 as to why there is an 8
        simulationData[minionIdx].temperature_sel = sel
        minionIdx = minionIdx + 1

def getNthRow(rowNum, filename):
    i=0
        with open(filename, 'r') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
            csvreader = csv.reader(filename)
            for row in csvreader:
                if i==rowNum:
                    # do nothing
                    return row
                i += 1
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file

def RDCommandHandler():

def write_SPIR():
    

def CreateReadPacket():

def WRCommandHandler(buf, len):
    BYTES_PER_REG = 6
    data = [0] * (NUM_MINIONS * BYTES_PER_REG)

    if currCmd == command_codes['SIM_LTC6811_WRCFGA']:
        ExtractDataFromBuff(data, buf, len)
        int dataIdx = 0
        for x in range(NUM_MINIONS - 1, 0, -1)
            memcpy(simulationData[x][].config, &data[dataIdx*BYTES_PER_REG], BYTES_PER_REG)
            dataIdx = dataIdx + 1
        pass
    else:
        pass
    

#Reads SPI_Write and updates the globals
def read_SPIW(): 
    global currentRow, currCmd
    #read the row counter that is at the very beginning of the file
    os.makedirs(os.path.dirname(file_w), exist_ok=True)
    with open(file_w, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        firstRow = csvreader.next(csvreader)
        currentRow = firstRow[0]    #update currentRow
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file

    currentWrite = getNthRow(currentRow, file_w)    #get the latest data written into SPIW
    currCmd = ExtractCmdFromBuff(currentWrite)      #get command code

    if(((currCmd & 0x600) == 0x200) or (currCmd & 0x700) == 0x400) :
        currCmd &= ~0x187  # Bit Mask to ignore any cmd configuration bits i.e. ignore the MD, DCP, etc. bits
    WRCommandHandler(currentWrite, #some length variable)

#Writes to SPIR
def Write_SPIR():
    RDCommandHandler()
    with open(file_r, 'a') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvwriter = csv.writer(csvfile)
        CreateReadPacket() = csvwriter.writerow(csvfile)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file

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
    with open(file_r, 'w+') as csvfile:
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
    with open(file_r, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            for i in range(len(row)):
                row[i] = int(row[i])
            values.append(row)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
    return values
