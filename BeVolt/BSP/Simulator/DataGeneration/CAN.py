import csv

#path name to file
file = "Data/CAN.csv"

"""
@brief send a CAN message as a specific system
    Function called by simulate.py
"""
def Send_Message(id, message[8], length):
    with open(file, 'w') as csvfile: #write to file
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow([ "id: "+ id, "message: 0x"+message[0],"0x"+message[1],"0x"+message[2],"0x"+message[3],"0x"+message[4],"0x"+message[5],"0x"+message[6],"0x"+message[7] ])