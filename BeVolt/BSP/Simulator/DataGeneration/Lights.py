import csv

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = "Data/Lights.csv"

#returns integer where 9 LSB are status of 9 LED's
def Read_Lights():
    with open(file, 'r') as csvfile: #read file
        csvreader = csv.reader(csvfile)
        return int(csvreader) 