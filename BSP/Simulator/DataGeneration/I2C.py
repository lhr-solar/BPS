import csv
import os
import fcntl
import config

file = config.directory_path + config.files['I2C']

"""
@brief output the EEPROM error messages
    Function to be called by simulate.py
"""

def 