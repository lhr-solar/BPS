import csv
import os
import fcntl

file = "BSP/Simulator/DataGeneration/Data/WDTimer.csv"

TimerEnable = False
duration = 15
pid = 0
systemReset = False

def Check_State():
    global duration
    global TimerEnable
    #global systemReset
    if TimerEnable == True:
        os.makedirs(os.path.dirname(file), exist_ok=True)
        with open(file, 'r+') as csvfile:
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)  
            csvreader = csv.reader(csvfile)
            currentState = next(csvreader)
            if ((currentState[0] == 'aaaa') and (duration > 0)):
                duration = 15 #reset countdown
                csvfile.seek(0)
                csvwriter = csv.writer(csvfile)
                #if(systemReset == True):
                    #csvwriter.writerow(["____", "b"]) #Kick watchdog. Clear the reset message ('aaaa'), a blank .csv file crashes program
                csvwriter.writerow(["____"]) #Kick watchdog. Clear the reset message ('aaaa'), a blank .csv file crashes program
            elif (((currentState[0] != '____') and (currentState[0] != 'aaaa') and (currentState[0] != 'cccc')) or (duration <= 0)):
                csvfile.seek(0)
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow(["5555", "b"]) #print system reset message. The '5555' has no significance, it's just there to keep the formatting
                #systemReset = True
                os.kill(int(pid), 9) #system reset - kill process
                TimerEnable = False #After system reset, watchdog can start again 
                duration = 15 
            fcntl.flock(csvfile.fileno(), fcntl.LOCK_UN)

def WD_Enable():
    global TimerEnable
    global systemReset
    global pid

    os.makedirs(os.path.dirname(file), exist_ok=True)
    with open(file, 'r') as csvfile:
        fcntl.flock(csvfile.fileno(), fcntl.LOCK_EX)  
        csvreader = csv.reader(csvfile)
        currentState = next(csvreader)
        if TimerEnable == False:    #check for timer startup
            if currentState[0] == 'cccc':
                    TimerEnable = True #start Watchdog timer
            elif currentState[0] != '5555': 
                pid = currentState[0]
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
        return '//' #Watchdog hasn't been started by C code yet
