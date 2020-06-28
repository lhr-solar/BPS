import csv
import os
import fcntl

file = "BSP/Simulator/DataGeneration/Data/PLL.csv"

"""
@brief change the clock frequency
    Function to be called by simulate.py
"""
CurrentFrequency = 16000000


def Change_Frequency(newFrequency):
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'w') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow([newFrequency])
        global CurrentFrequency 
        CurrentFrequency = int(newFrequency)
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    

def Get_Frequency():
    if os.stat(file).st_size != 0:      #check if file is empty
        os.makedirs(os.path.dirname(file), exist_ok=True)
        with open(file, 'r') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)    
            csvreader = csv.reader(csvfile)
            data = next(csvreader)
            global CurrentFrequency 
            CurrentFrequency = data[0]
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)    
    return int(CurrentFrequency)

