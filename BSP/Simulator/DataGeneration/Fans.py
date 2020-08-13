import csv
import os
import fcntl
import config

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = config.directory_path + config.files['Fans']

#returns integer where 9 LSB are status of 9 LED's
def read():
    duty = []
    os.makedirs(os.path.dirname(file), exist_ok=True)   # creates directory if not exists
    with open(file, 'a') as csvfile:
        pass    # creates file if not exists
    with open(file, 'r') as csvfile: #read file
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            duty.append(row)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
        if len(lights):
            return int(duty[0][0])
        else:
            return 0
            
