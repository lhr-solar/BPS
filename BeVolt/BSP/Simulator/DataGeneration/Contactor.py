import csv

#This module will read from the csv file to determine the status of the Contactor

#file path to data
file = "Data/Contactor.csv"

#Reads file and returns 1 or 0, 1 is on, 0 is off
def Read_Contactor():
    with open(file, 'r') as csv:
        Contactor_Read = csv.reader(csv)
        return Contactor_Read



if __name__ == "main":
    Read_Contactor()