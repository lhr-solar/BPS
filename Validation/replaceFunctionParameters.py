'''
Used to replace a function and it's parameters with a new one. 
All files at the same directory of this script will be searched/replaced. Subdirectories are not affected
'''

#change these variables as needed before running
oldFunctionName = "OSTaskCreate" #this is the target function that you're trying to replace
keepParameters = [0,1,2,3,4,5,7] #choose which parameters of the function to keep. Start counting them at 0 cause that's how computers count
newFunctionName = "RTOS_BPS_TaskCreate" #name of the new function that will take the place of the old function. Will be set to oldFunctionName if left

import os
import subprocess

#get the directory we are working in, store in variable name path
filename = __file__
lastslash = -1
for i in range(len(filename)-1,0,-1):
    if filename[i] == '/':
        lastslash = i
        break

path = filename[0:lastslash+1]
print(path)

#if the newFunction name is not given, then set it to the old function name
if(newFunctionName == ""):
    newFunctionName = oldFunctionName

for files in os.listdir(path):
    #output file is the variable we write out to the file
    outputfile = ""

    #open the file, but only if it's not a directory or this file
    files = path + files
    if(files == __file__ or os.path.isdir(files)):
        continue

    #open the file for reading
    with open(files, 'r') as f:
        print(f)
        
        lines = f.read()
        split = lines.split(oldFunctionName)

        #skip this file if the file does not contain the method 
        if(len(split) <= 1):
            f.close()
            print("no method found")
            continue

        outputfile += split[0]
        for i in range (1,len(split)):
            callEnd = split[i].index(";") #finds the end of the function call

            callArgs = split[i][0:callEnd].split(',') #these are all the old function parameters

            outputfile += newFunctionName

            for j in keepParameters:
                outputfile += callArgs[j] + ','

            outputfile = outputfile[:-1] + ")" #get rid of extra comma, and end the function call
            outputfile += split[i][callEnd:]
        f.close()

    with open(files, 'w') as f:
        f.write(outputfile)
        f.close()

#run through the compiling 
for files in os.listdir(path):
    filename = str(files)[5:-2]
    command = "make stm32f413 TEST="+filename
    command = command.split(" ")
    subprocess.run(command)
