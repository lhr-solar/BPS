import csv



file = "BSP/Simulator/DataGeneration/Data/CAN.csv"

valid_IDs = ['002', '001', '003', '004', '005', '101', '102', '103', '104','105','106','107','108','109','10a','711','712','771','772','580','581','582','583','584','243','620']

"""
@brief send a CAN message as a specific system
    Function called by simulate.py
"""
def Send_Message(id, message):
    with open(file, 'w') as csvfile: 
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow([  "0x" + id, "0x"+message[0],"0x"+message[1],"0x"+message[2],"0x"+message[3],"0x"+message[4],"0x"+message[5],"0x"+message[6],"0x"+message[7] ])

def Get_CAN_Info():
    with open(file, 'r') as csvfile:
        csvreader = csv.reader(csvfile)
        return next(csvreader)

def Invalid_CAN_ID(id):
    ret = True
    for i in valid_IDs:
        if (i == id):
            return False    #This id is valid
    return True     #This id is invalid