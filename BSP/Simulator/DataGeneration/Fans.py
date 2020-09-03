import csv
import os
import fcntl
import config

#This module will hold the statuses of the Fans on the BPS.

#path name to file
file = config.directory_path + config.files['Fans']

#returns array of integers representing duty cycles of all Fans
def read():
    os.makedirs(os.path.dirname(file), exist_ok=True)   # creates directory if not exists
    with open(file, 'a') as csvfile:
        pass    # creates file if not exists
    with open(file, 'r') as csvfile: #read file
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile, delimiter = ',')
        for row in csvreader:
            duty = row
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
        if len(duty):
            return duty #return list with speed for every fan
        else:
            return 0