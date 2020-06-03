import csv

#This module will read from the csv file to determine the status of the Contactor

#file path to data
file = "Data/Contactor.csv"

#Reads file and returns 1 or 0, 1 is on, 0 is off
def Read_Contactor():
    with open(file, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        return next(csvreader)
        #print(next(csvreader)): used this to test function

#Ran the function to test if it works
#Read_Contactor()