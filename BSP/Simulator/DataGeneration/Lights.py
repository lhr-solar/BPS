# Copyright (c) 2020 UT Longhorn Racing Solar

import csv
import os
import fcntl
import config

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = config.directory_path + config.files['Lights']

#returns integer where 9 LSB are status of 9 LED's
def read():
    lights = []
    os.makedirs(os.path.dirname(file), exist_ok=True)   # creates directory if not exists
    with open(file, 'a') as csvfile:
        pass    # creates file if not exists
    with open(file, 'r') as csvfile: #read file
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            lights.append(row)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
        if len(lights):
            return int(lights[0][0])
        else:
            return 0
            
