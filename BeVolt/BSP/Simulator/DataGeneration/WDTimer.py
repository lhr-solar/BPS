import csv
import os
import fcntl

file = "BSP/Simulator/DataGeneration/Data/WDTimer.csv"

TimerEnable = False
duration = 15 #unit is in seconds
pid = 0
systemReset = False

start = '1'
reset = '2'

def Check_State():
    global duration
    global TimerEnable
    os.makedirs(os.path.dirname(file), exist_ok=True)
    if TimerEnable == True:
        with open(file, 'r+') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)  
            csvreader = csv.reader(csvfile)
            currentState = next(csvreader)
            if ((currentState[0] == reset) and (duration > 0)):
                duration = 15 #reset countdown
                csvfile.seek(0)
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow([" "]) #Kick watchdog. Clear the reset message ('2').
                csvwriter.writerow(["0"])
            elif (duration <= 0):
                csvfile.seek(0)
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow([" ", "b"]) #print system reset message. The ' ' has no significance, it's just there to keep the formatting. The 'b' notifies the C program that the system has been reset
                csvwriter.writerow(["0"])
                os.kill(int(pid), 9) #system reset - kill process
                TimerEnable = False #After system reset, watchdog can start again 
                duration = 15 
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)


def WD_Enable():
    global TimerEnable
    global pid
    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)  
        csvreader = csv.reader(csvfile)
        currentState = next(csvreader)
        if TimerEnable == False:    #check for timer startup
            if currentState[0] == start:
                    TimerEnable = True #start Watchdog timer
            secondLine = next(csvreader)
            if secondLine[0] != 0:
                pid = secondLine[0]
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)

def Tick():
    global duration
    if ((TimerEnable == True) and (duration > 0)):
        duration = duration - 1
        if(duration < 10):
            return "0" + str(duration) + " "
        else:
            return duration
    else:
        return 'Not started' #// = Watchdog hasn't been started by C code yet
