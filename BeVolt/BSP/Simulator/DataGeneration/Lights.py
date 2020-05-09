import csv

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = "Data/Lights.csv"

#returns integer where 9 LSB are status of 9 LED's
def read():
    lights = []
    with open(file, 'r') as csvfile: #read file
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            lights.append(row)
        return int(lights[0][0])
