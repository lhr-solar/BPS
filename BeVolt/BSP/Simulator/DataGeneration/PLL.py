import csv

#path name to file
file = "Data/PLL.csv"

"""
@brief change the clock frequency
    Function to be called by simulate.py
"""

def Change_Frequency(newFrequency):
    with open(file, 'w') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow([newFrequency])