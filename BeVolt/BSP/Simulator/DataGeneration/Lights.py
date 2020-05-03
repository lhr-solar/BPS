import csv
import os

#This module will hold the statuses of the LED's on the BPS. 

#path name to file
file = "Data/Lights.csv"

#this will define the lights
class Lights:
    status = 0 #on or off
    initialized  = 0 # initialized or not
    # @brief: This will initialize the data types for each LED
    def __init__(self, stat, init):
        self.status = stat
        self.initialized = init
    # @brief: This will change the status of the specific LED
    # @param: The Led that will be changed
    def Status_Change(self,stat):
        self.status = stat

#initialize all the lights and store them in the file
def Lights_Init():
    FAULT = Lights(0,1)
    RUN = Lights(0,1)
    OCURR = Lights(0,1)
    OTEMP = Lights(0,1)
    OVOLT = Lights(0,1)
    UVOLT = Lights(0,1)
    WDOG = Lights(0,1)
    CAN = Lights(0,1)
    EXTRA = Lights(0,1)
    Lights_Write = open(file, 'w') 
    Lights_Write.writer(file, delimeter = ',')
    #stores status and initializations of all led's in file
    Lights_Write.writerows([["FAULT", FAULT.status, FAULT.initialized], 
                            ["RUN", RUN.status, RUN.initialized],
                            ["OCURR", OCURR.status, OCURR.initialized], 
                            ["OTEMP", OTEMP.status, OTEMP.initialized], 
                            ["OVOLT", OVOLT.status, OVOLT.initialized], 
                            ["UVOLT", UVOLT.status, UVOLT.initialized],
                            ["WDOG", WDOG.status, WDOG.initialized],
                            ["CAN", CAN.status, CAN.initialized],
                            ["EXTRA", EXTRA.status, EXTRA.initialized]])


if __name__ == '__main__':
    Lights_Init()
    