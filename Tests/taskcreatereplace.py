import os
import subprocess

path = str(os.getcwd())+"/Tests/"
print(path)

for files in os.listdir(path):
    outputfile = ""

    files = path + files
    with open(files, 'r') as f:
        print(f)
        
        if(files == __file__):
            continue
        
        lines = f.read()
        split = lines.split("OSTaskCreate")

        if(len(split) <= 1):
            f.close()
            print("no taskcreate found")
            continue

        outputfile += split[0]
        for i in range (1,len(split)):
            callEnd = split[i].index(";")

            callArgs = split[i][0:callEnd].split(',')
            callArgs.pop(6)
            callArgs = callArgs[0:7]
            print(callArgs)

            outputfile += "RTOS_BPS_TaskCreate"
            for j in callArgs:
                outputfile += j + ","
            outputfile = outputfile[:-1] + ")"
            outputfile += split[i][callEnd:]
        f.close()

    with open(files, 'w') as f:
        f.write(outputfile)
        f.close()


'''
#run through the compiling 
for files in os.listdir(path):
    filename = str(files)[5:-2]
    command = "make stm32f413 TEST="+filename
    command = command.split(" ")
    subprocess.run(command)
'''