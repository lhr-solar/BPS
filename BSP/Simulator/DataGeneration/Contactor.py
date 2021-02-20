# Copyright (c) 2020 UT Longhorn Racing Solar

import csv
import fcntl
import os

#This module will read from the csv file to determine the status of the Contactor

#file path to data
file = config.directory_path + config.files['Contactor']

#Reads file and returns 1 or 0, 1 is on, 0 is off
def Read_Contactor():
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file before reading
        csvreader = csv.reader(csvfile) 
        Contactor_State = next(csvreader)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file after reading
        return Contactor_State
        #print(Contactor_State) #: used this to test function

#Ran the function to test if it works
#Read_Contactor()
