import csv
import os
import fcntl
import config

file = config.directory_path + config.files['I2C']
# dump EEPROM all faults LIKE serialprintdata in eeprom file
# read specific addresses out SIMILAR TO BSP_I2C_READ

"""
@brief output the EEPROM error messages
    Function to be called by simulate.py
"""

def EEPROM_Dump():
    dump = []
    if os.stat(file).st_size != 0:
        os.makedirs(os.path.dirname(file), exist_ok=True)
        with open(file, 'r') as csv file:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    
            csvreader = csv.reader(csvfile)
            data = next(csvreader)
            while data is not 0xff:
                dump.append(data)
                data = next(csvreader)
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
            return str(dump)
    return "no EEPROM data"
    #Dump all faults from I2C.csv file
    # pass

def I2C_Read(startAddress):
    #Read faults from I2C.csv starting at startAdddress
    pass
