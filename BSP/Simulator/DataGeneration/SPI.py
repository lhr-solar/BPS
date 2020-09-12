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
config_register[24] = 0

openWireOpFlag = False                  # The reading of voltages for both regular and openwire modes are
                                        # the same. The commands are different though. To measure the
                                        # regular voltage values, the ADCV command code is used. To measure
                                        # the open wire voltage values, the ADOW command code is used.
                                        # Look in the LTC6811 datasheet for more information on the difference.
                                        # The command to read the voltage values for both commands is RDCV.
                                        # When the ADOW command is called in BSP_SPI_Write, this flag should
                                        # turn true so the next time the BSP_SPI_Read function is called with
                                        # the RDCV command set, the RDCommandHandler will know to return the
                                        # appropratie open wire voltages. Any other command that's not ADOW but
                                        # still triggers and ADC conversion (e.g. ADCV), then this flag should
                                        # be set to false.

openWirePUFlag = False                  # Indicates if the pull-up voltage values should be copied or not.
                                        # This flag is set if the ADOWPU command was issued in
                                        # BSP_SPI_Write. The CopyOpenWireVoltageToByteArray function uses
                                        # this flag to determine which voltage values should be written.

temperature_mux[4] = 0

temperature_sel[4] = 0

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

mux_addresses = {
    'SIM_LTC1380_MUX1' : 0x90,        # MUX addresses on Minion boards
    'SIM_LTC1380_MUX2' : 0x92
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
currCmd = 0     #Before every BSP_SPI_Read, BSP_SPI_Write needs to be called to specify the command to handle.

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

def DetermineGroupLetter(command):
    group = 0
    if(command == command_codes['SIM_LTC6811_RDAUXA']):
        group = 0   #group A
    elif(command == command_codes('SIM_LTC6811_RDAUXB')):
        group = 1   #group B
    elif(command == command_codes['SIM_LTC6811_RDAUXC']):
        group = 2   #group C
    elif(command == command_codes['SIM_LTC6811_RDAUXD']):
        group = 3   #group D
    elif(command == command_codes['SIM_LTC6811_RDCVE']):
        group = 4   #group E
    elif(command == command_codes['SIM_LTC6811_RDCVF']):
        group = 5   #group F
    else:
        group = 0   #group A
    return group


def RDCommandHandler(buf, len):
    global currCmd
    BYTES_PER_REG = 6
    data = [0] * (NUM_MINIONS * BYTES_PER_REG)

    if currCmd == command_codes['SIM_LTC6811_RDCFGA'] :
        CreateReadPacket(config_register)
    if currCmd == command_codes['SIM_LTC6811_RDCVF']: 
        groupLetter = DetermineGroupLetter(currCmd)
        if(openWireOpFlag):
            CopyOpenWireVoltageToByteArray(data, groupLetter, openWirePUFlag)
        else:
            CopyVoltageToByteArray(data, groupLetter)
        
        CreateReadPacket(data)
    if currCmd == command_codes['SIM_LTC6811_RDAUXA']:
        groupLetter = DetermineGroupLetter(currCmd)
        CopyTemperatureToByteArray(data, groupLetter)
        CreateReadPacket(buf, data, NUM_MINIONS * BYTES_PER_REG)
    


def determineModuleIndicies(grp):
    if group == 0:
        startingIndicies = [0, 8, 16, 24]   #Group A will return 3 voltages per IC
    elif group == 1:
        startingIndicies = [3, 11, 19, 27]  #Group B will return 3 voltages per IC
    elif group == 2:
        startingIndicies = [6, 14, 22, 30]  #Group C will return 2 voltages for ICs [0:2] and 1 voltage for the last IC
    return startingIndicies

def CopyOpenWireVoltageToByteArray(data, group, pullup):
    BYTES_PER_REG = 6
    NUM_MODULES = 31
    int MAX_PINS_PER_LTC6811 = 12         # LTC6811 can only support 12 voltage modules
    pullupVoltages[NUM_MODULES] = 0       #there are 31 modules and 4 minions currently. The minions are connected to {8,8,8,7} modules
    pulldownVoltages[NUM_MODULES] = 0
    
    indicies = []
    indicies = determineModuleIndicies(group)
    
    # LTC6811 datasheet may be wrong***
    # For pins 1-11, the pullupVolt-pulldownVolt > -400mV must be true in order for the LTC6811
    # driver to recognize that the pin is an open wire.
    # If pullupVolt[1] = 0, then pin C0 is open.
    # If pulldownVolt[12] = 0, then pin C12 is open.
    # TODO: Simulator currently does not support open wire indication of pin C0 (ground pin)
    for i in range(0, NUM_MODULES):
        if open_wires[i] == 1:
            pullupVoltages[i] = 40000               # These are just dummy values. As long as
                                                    # pullup-pulldown > 4000 (-400mV)
            pulldownVoltages[i] = 30000
        
        else:
            pullupVoltages[i] = 30000
            pulldownVoltages[i] = 40000
    dataIdx = 0
    
    for i in range(NUM_MINIONS-1, -1, -1):
        sixBytes = [0,0,0,0,0,0]
        groupVoltages = [0,0,0]
        index = indicies[i] 
        if pullup:
            if group != 3:
                for s in range(0, 3):
                    groupVoltages[s] = pullupVoltages[index+s]                  #save the 3 voltages from the group for each IC
                byteIndex = 0
                for s in range(0, 3):
                    if byteIndex > 5:
                        break
                    sixBytes[byteIndex] = hex(groupVoltages[s]) & 0xFF00        #break up each 16 bit voltage into two bytes
                    sixBytes[byteIndex+1] = hex(groupVoltages[s]) & 0x00FF
                    byteIndex +=2
            else:
                for s in range(0, 2):       #group C for the 4 ICs will return {2,2,2,1} voltage values
                    groupVoltages[s] = pullupVoltages[index+s]
                byteIndex = 0
                for s in range(0, 2):
                    if byteIndex > 5:
                        break
                    sixBytes[byteIndex] = hex(groupVoltages[s]) & 0xFF00
                    sixBytes[byteIndex+1] = hex(groupVoltages[s]) & 0x00FF
                    byteIndex += 2
            for k in range(0, 6):
                data[(dataIdx * BYTES_PER_REG) + k] = sixBytes[k]           #save all 6 bytes of voltage data into array
        
        else: #pulldown
            if group != 3:
                for s in range(0, 3):
                    groupVoltages[s] = pulldownVoltages[index+s]                  #save the 3 voltages from the group for each IC
                byteIndex = 0
                for s in range(0, 3):
                    if byteIndex > 5:
                        break
                    sixBytes[byteIndex] = hex(groupVoltages[s]) & 0xFF00        #break up each 16 bit voltage into two bytes each
                    sixBytes[byteIndex+1] = hex(groupVoltages[s]) & 0x00FF
                    byteIndex +=2
            else:
                if index != 30
                    for s in range(0, 2):       #group C for the 4 ICs will return {2,2,2,1} voltage values
                        groupVoltages[s] = pulldownVoltages[index+s]
                else:   #account for Group C of the IC with only 7 modules
                    groupVoltages[s] = pulldownVoltages[index]
                
                byteIndex = 0
                for s in range(0, 2):
                    if byteIndex > 5:
                        break
                    sixBytes[byteIndex] = hex(groupVoltages[s]) & 0xFF00
                    sixBytes[byteIndex+1] = hex(groupVoltages[s]) & 0x00FF
                    byteIndex += 2
            for k in range(0, 6):
                data[(dataIdx * BYTES_PER_REG) + k] = sixBytes[k]           #save all 6 bytes of voltage data into array
        
        dataIdx += 1        
        
        return data
    

def CopyVoltageToByteArray(data, group):
    global voltage_values
    dataIdx = 0
    
    indicies = determineModuleIndicies(group)


    for i in range(NUM_MINIONS-1, -1, -1):
        groupVoltages = [0, 0, 0]
        sixBytes = [0,0,0,0,0,0]
        index = indicies[i]
        if group != 3:
            for s in range(0, 3):
                groupVoltages = voltage_values[index+s]     #save the the voltages for the group
            for s in range(0, 3)
                if byteIndex > 5:
                        break
                sixBytes[byteIndex] = hex(groupVoltages[s]) & 0xFF00        #break up each 16 bit voltage into two bytes and save them
                sixBytes[byteIndex+1] = hex(groupVoltages[s]) & 0x00FF
                byteIndex +=2
        else:
            if index != 30:
                for s in range(0, 2):       #group C for the 4 ICs will return {2,2,2,1} voltage values
                    groupVoltages[s] = voltage_values[index+s]
            else:   #account for Group C of the IC with only 7 modules
                groupVoltages[s] = voltage_values[index]
            byteIndex = 0
            for s in range(0, 2):
                if byteIndex > 5:
                    break
                sixBytes[byteIndex] = hex(groupVoltages[s]) & 0xFF00        #break up each 16 bit voltage into two bytes each
                sixBytes[byteIndex+1] = hex(groupVoltages[s]) & 0x00FF
                byteIndex += 2

        for k in range(0, 6):
            data[(dataIdx * BYTES_PER_REG) + k] = sixBytes[k]           #save all 6 bytes of voltage data into array

        dataIdx += 1


def CreateReadPacket(data):
    pkt[]
    pktIdx = 0
    BYTES_PER_REG = 6
    for currIC in range(NUM_MINIONS, 0, -1)
        for currByte in  range(0, BYTES_PER_REG, 1)
            pkt[pktIdx] = data[((currIC-1)*6)+currByte]
            pktIdx = pktIdx + 1
        dataPEC = PEC15_Calc(data[(currIC-1)*6], BYTES_PER_REG)
        pkt[pktIdx] = (dataPEC >> 8) & 0x00FF
        pkt[pktIdx + 1] = dataPEC & 0x00FF
        pktIdx = pktIdx + 2
    return pkt


def WRCommandHandler(buf, len):
    global openWireOpFlag, openWirePUFlag
    
    BYTES_PER_REG = 6
    data = [0] * (NUM_MINIONS * BYTES_PER_REG)

    if currCmd == command_codes['SIM_LTC6811_WRCFGA']:
        ExtractDataFromBuff(data, buf, len)
        config_register[] = data[]
        pass
    elif currCmd == command_codes['SIM_LTC6811_ADAX']:
        openWireOpFlag = False
    elif currCmd == command_codes['SIM_LTC6811_ADOWPU']:
        openWirePUFlag = True

    elif currCmd == command_codes['SIM_LTC6811_ADOWPD'] #ask Sijin about this on Saturday
        openWirePUFlag = False      
        openWireOpFlag = True
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
    with open(file_r, 'a') as csvfile: #open to append
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(CreateReadPacket(config_register))
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
