import csv
import fcntl
import os


file = "BSP/Simulator/DataGeneration/Data/CAN.csv"

valid_IDs = [
 '001', #DASH_KILL_SWITCH
 '003', #ANY_SYSTEM_FAILURES
 '004', #IGNITION
 '005', #ANY_SYSTEM_SHUTOFF
 #BPS
 '002', #BPS_TRIP
 '101', #BPS_ALL_CLEAR
 '102', #BPS_CONTACTOR_STATE
 '103', #CURRENT_DATA
 '104', #VOLTAGE_DATA_ARRAY
 '105', #TEMPERATURE_DATA_ARRAY
 '106', #SOC_DATA
 '107', #WDOG_TRIGGERED
 '108', #CAN_ERROR
 '109', #BPS_COMMAND_MSG
 '10a', #MOTOR_DISABLE
 #MPPT
 '711', #MASTER_REQUEST1
 '712', #MASTER_REQUEST2
 '771', #MPPT_MASTER_RECIEVE1
 '772', #MPPT_MASTER_RECIEVE2
 #Dashboard
 '580', #FORWARD
 '581', #CRUISE_SET
 '582', #CRUISE
 '583', #REGEN
 '584', #CRUISE_CONTROL_STATE
 '243', #SPEED
 #Array
 '620' #RTDS
 ]

"""
@brief send a CAN message as a specific system
    Function called by simulate.py
"""
def Send_Message(id, message, length):
    CANString = []
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'w') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)  
        csvwriter = csv.writer(csvfile)
        CANString.append("0x" + id)
        i = 1
        for i in range (length):
            CANString.append("0x" + message[i])      
        csvwriter.writerow(CANString)   #"0x"+message[0],"0x"+message[1],"0x"+message[2],"0x"+message[3],"0x"+message[4],"0x"+message[5],"0x"+message[6],"0x"+message[7] ])
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)

def Get_CAN_Info():
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)
        csvreader = csv.reader(csvfile)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
        return next(csvreader)

def Invalid_CAN_ID(id):
    return not id in valid_IDs

#def Invalid_CAN_ID(id):
    #for i in valid_IDs:
        #if (i == id):
            #return False    #This id is valid
    #return True     #This id is invalid