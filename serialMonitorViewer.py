from serialMonitor import SerialMonitor
from tkinter import *
import threading

class SerialMonitorViewer(threading.Thread):

    def __init__(self, comPort, baudRate = 9600):
        self.serialMonitorHandler = #SerialMonitor(comPort, baudRate)
        super(SerialMonitorViewer, self).__init__()
        
    def run(self):
        self.initSerialDisplay()

        while True:
            self.updateSerialMonitorDisplay()
        
    def initSerialDisplay(self):
        self.displayFrame = Tk()
        self.textBox = Text(self.displayFrame, height= 100, width= 100)
        self.textBox.pack(fill= BOTH, side= TOP)

        self.userInputBar = Entry(self.displayFrame)
        self.userInputBar.pack(fill= BOTH, side= LEFT)
        self.userInputButton = Button(self.displayFrame, width= 10, command= writeUserInputToComPort)
        self.userInputButton.pack(side= RIGHT)
        
    def updateSerialMonitorDisplay(self):
        self.textBox.config(state= NORMAL)
        newData = self.serialMonitorHandler.getLineFromComPort()
        self.textBox.insert(CURRENT, newData)
        self.textBox.config(state= DISABLED)

    def writeUserInputToComPort():
        messageToSend = self.userInputBar.get()
        self.userInputBar.delete(0, END)
        self.serialMonitorHandler.sendStringToComPort(messageToSend)

if __name__ == "__main__":
    viewingPort = SerialMonitorViewer("COM20")
    viewingPort.start()
    
