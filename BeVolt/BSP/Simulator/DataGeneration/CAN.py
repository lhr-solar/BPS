import csv

#path name to file
file = "Data/CAN.csv"

"""
@brief send a CAN message as a specific system
    Function called by simulate.py
"""
def Send_Message(id, message, length):
    with open(file, 'w') as csvfile: #write to file
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow([id, message, length])