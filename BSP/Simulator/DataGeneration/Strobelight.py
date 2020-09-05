import csv
import os
import fcntl
import config
#This module holds status of strobe (on/off)

#path name to file
file = config.directory_path + config.files['Strobelight']

#returns integer where 1 means strobe is on, 0 means strobe is off
def read():
    strobe = []
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'a') as csvfile:
        pass   #creates file if it doesn't exist
    with open(file, 'r') as csvfile: #read file
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    # Lock file
        csvreader = csv.reader(csvfile)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    # Unlock file
        for row in csvreader:
            strobe.append(row)
        if len(strobe):
            return int(strobe[0][0])
        else:
            return 0