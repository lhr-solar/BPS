import csv
import os
import fcntl
import config

TERMINATOR = ["0xff"]
file = config.directory_path + config.files["I2C"]
# read specific addresses out SIMILAR TO BSP_I2C_READ

faultDict = {
    "0x01" : "FAULT_HIGH_TEMP",
    "0x02" : "FAULT_HIGH_VOLT",
    "0x04" : "FAULT_LOW_VOLT",
    "0x08" : "FAULT_HIGH_CURRENT",
    "0x10" : "FAULT_WATCHDOG",
    "0x20" : "FAULT_CAN_BUS",
    "0x40" : "FAULT_VOLT_MISC",
    "0x80" : "DATA_SOC"
}

"""
@brief output the EEPROM error messages
    Function to be called by simulate.py
"""

def EEPROM_Dump():
    dump = []
    if os.stat(file).st_size != 0:
        os.makedirs(os.path.dirname(file), exist_ok=True)
        with open(file, 'r') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    
            csvreader = csv.reader(csvfile)
            dataList = list(csvreader)
            for fault in dataList:
                if fault == TERMINATOR:
                    break
                # print faultDict[str(fault).strip('[]')]
                # I'd like to printout each error's name but cannot get this dictionary printing to work
                dump.append(fault)
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
            return str(dump)
    return "no EEPROM data"
    #Dump all faults from I2C.csv file

def I2C_Read(startAddress):
    #Read faults from I2C.csv starting at startAdddress
    pass
