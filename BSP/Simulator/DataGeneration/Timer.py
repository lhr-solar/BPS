import csv
import os
import os.path
import fcntl
import config
import threading
import time

file = config.directory_path + config.files['Timer']
ticks = 0
startFlag = False

def Tick():
    global ticks
    os.makedirs(os.path.dirname(file), exist_ok=True)
    while startFlag != True:    #do nothing until timer is enabled
        wait = 0    #filler statement to avoid traceback errors
    while True:
        reload = 2**24 #when reload value has been reached, reset for next function call. Meant to simulate 24 bit SysTick 
        ticks = 0
        while(ticks < reload):
            with open(file, 'r+') as csvfile:
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)
                csvfile.seek(0)    
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow([ticks])
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
            ticks = ticks + 1
            time.sleep(.00000001)    #this delay is here to control the tick rate
             
        
            

timer_Thread = threading.Thread(target=Tick)

def Enable():
    global startFlag
    os.makedirs(os.path.dirname(file), exist_ok=True)
    if os.path.isfile(file) == True:    #only execute the following code if the .csv file has been created 
        if ((os.stat(file).st_size != 0) and (startFlag == False)):      #check if file is empty or not
            with open(file, "r+") as csvfile:
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)
                csvreader = csv.reader(csvfile)
                check = next(csvreader)
                if(check[0] == 's'):
                    startFlag = True
                    csvfile.seek(0)
                    csvwriter = csv.writer(csvfile)
                    csvwriter.writerow([" "])   #clear startup message left by C code
                fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)
        
            

