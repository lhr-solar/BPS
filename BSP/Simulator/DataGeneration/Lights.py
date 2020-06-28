import csv
import os

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = "BSP/Simulator/DataGeneration/Data/Lights.csv"

#returns integer where 9 LSB are status of 9 LED's
def read():
    lights = []
    os.makedirs(os.path.dirname(file), exist_ok=True)   # creates directory if not exists
    with open(file, 'a') as csvfile:
        pass    # creates file if not exists
    with open(file, 'r') as csvfile: #read file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            lights.append(row)
        if len(lights):
            return int(lights[0][0])
        else:
            return 0
            
