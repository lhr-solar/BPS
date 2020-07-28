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
    #Dump all faults from I2C.csv file
    pass

def I2C_Read(startAddress):
    #Read faults from I2C.csv starting at startAdddress
    pass
