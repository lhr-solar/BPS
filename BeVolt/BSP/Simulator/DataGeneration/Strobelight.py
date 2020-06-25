import csv
import os
#This module holds status of strobe (on/off)

#path name to file
file = "BSP/Simulator/DataGeneration/Data/Strobe.csv"

#returns integer where 1 means strobe is on, 0 means strobe is off
def read():
    strobe = []
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'a') as csvfile:
        pass   #creates file if it doesn't exist
    with open(file, 'r') as csvfile: #read file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            strobe.append(row)
        if len(strobe):
            return int(strobe[0][0])
        else:
            return 0
print(read())