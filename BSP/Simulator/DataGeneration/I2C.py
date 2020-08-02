import csv
import os
import fcntl
import config

TERMINATOR = ["0xff"]
file = config.directory_path + config.files["I2C"]
# read specific addresses out SIMILAR TO BSP_I2C_READ

faultDict = {
    "0x01" : "High Temp",
    "0x02" : "High Volt",
    "0x04" : "Low Volt",
    "0x08" : "High Current",
    "0x10" : "Watchdog",
    "0x20" : "Can Bus",
    "0x40" : "Misc Volt",
    "0x80" : "Charge"
}

errornames = bool

"""
@brief output the EEPROM error messages
    Function to be called by simulate.py
"""

def EEPROM_Dump(errornames):
    if os.stat(file).st_size != 0:
        os.makedirs(os.path.dirname(file), exist_ok=True)
        with open(file, 'r') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    
            csvreader = csv.reader(csvfile)
            DataList = list(csvreader)
            if errornames == 1:
                Dump = ""
                for fault in DataList:
                    if fault == TERMINATOR:
                        break
                    fault = fault[0]
                    fault = faultDict[fault]
                    Dump += fault
                    Dump += ", "
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
                return Dump
            else:
                Dump = []
                for fault in DataList:
                    if fault == TERMINATOR:
                        break
                    Dump.append(fault)
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
                return str(Dump)
    return "No EEPROM data"

def I2C_Read(startAddress, errornames):
    if os.stat(file).st_size != 0:
        os.makedirs(os.path.dirname(file), exist_ok=True)
        with open(file, 'r') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    
            csvreader = csv.reader(csvfile)
            Faults = list(csvreader)
            if errornames==1:
                ReadFaults = ""
                for i in range(startAddress, len(Faults)):
                    if Faults[i] == ["0x00"]:
                        return "This value is too high. Nothing in the EEPROM at this address yet."
                    elif Faults[i] == TERMINATOR:
                        break
                    else:
                        fault = Faults[i]
                        fault = fault[0]
                        fault = faultDict[fault]
                        ReadFaults += fault
                        ReadFaults += ", "
                return (ReadFaults)
            else:
                ReadFaults = []
                for i in range(startAddress, len(Faults)):
                    if Faults[i] == ["0x00"]:
                        return "This value is too high. Nothing in the EEPROM at this address yet."
                    elif Faults[i] == TERMINATOR:
                        break
                    else:
                        ReadFaults.append(Faults[i])
            return str(ReadFaults)
    return "No EEPROM data"


