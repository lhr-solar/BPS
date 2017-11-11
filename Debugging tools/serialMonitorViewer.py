from serialMonitor import SerialMonitor
from tkinter import *
import threading

class SerialMonitorViewer(threading.Thread):

    def __init__(self, comPort, baudRate = 9600):
        self.serialMonitorHandler = SerialMonitor(comPort, baudRate)
        super(SerialMonitorViewer, self).__init__()
        
    def run(self):
        self.initSerialDisplay()

        while True:
            self.displayFrame.update_idletasks()
            self.displayFrame.update()
            
    def initSerialDisplay(self):
        self.displayFrame = Tk()
        self.scrollBar = Scrollbar(self.displayFrame)
        self.scrollBar.pack(side=RIGHT, fill= Y)
        self.textBox = Text(self.displayFrame, height= 20, width= 30, yscrollcommand = self.scrollBar)
        self.scrollBar.config(command= self.textBox.yview)
        self.textBox.pack()
        self.userInputBar = Entry(self.displayFrame, width= 30)
        self.userInputBar.pack(side= LEFT)
        self.userInputButton = Button(self.displayFrame, text= "Enter", width= 10, command= self.writeUserInputToComPort)
        self.userInputButton.pack(side= RIGHT)
        
    def updateSerialMonitorDisplay(self):
        if self.serialMonitorHandler.dataIsAvailable():
            self.textBox.config(state= NORMAL)
            newData = self.serialMonitorHandler.getCharFromComPort()
            self.textBox.insert(CURRENT, newData)
            self.textBox.config(state= DISABLED)

    def writeUserInputToComPort(self):
        messageToSend = self.userInputBar.get()
        self.userInputBar.delete(0, END)
        self.serialMonitorHandler.sendStringToComPort(messageToSend)
        self.updateSerialMonitorDisplay()

if __name__ == "__main__":
    viewingPort = SerialMonitorViewer("COM20")
    viewingPort.start()
    while True:
        pass
    
