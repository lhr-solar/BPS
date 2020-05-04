import csv
import os

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = "Data/Lights.csv"

#returns 
def Read_Lights():
    with open(file, 'rb') as csv: #read binary file
        Lights_Read = csv.reader(csv)
        return Lights_Read
    



if __name__ == '__main__':
    Read_Lights()
    