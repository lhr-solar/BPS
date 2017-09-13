import serial
import threading

class SerialMonitor(threading.Thread):

    def __init__(self, comPort, baudRate = 9600):
        self.comPort = comPort
        self.baudRate = baudRate
        self.serialConnection = serial.Serial(comPort, baudRate)
        super(SerialMonitor, self).__init__()

    def run(self):
        while True:
            self.displaySerialMonitor()

    def displaySerialMonitor(self):
        serialMonitorData = self.getLineFromComPort()
        print(serialMonitorData, end= '')
            
    def getIntFromComPort(self):
        serialData = self.getLineFromComPort()
        return int(serialData)

    def getFloatFromComPort(self):
        serialData = self.getLineFromComPort()
        return float(serialData)


    def getLineFromComPort(self):
        line = ''
        lastCharReceived = ''
        while '\n' not in lastCharReceived:
            lastCharReceived = self.getCharFromComPort()
            line += lastCharReceived

        return line

    def getCharFromComPort(self):
        return self.getByteFromComPort().decode('utf-8')

    def getByteFromComPort(self):
        return self.serialConnection.read()



    def sendNumToComPort(self, numToSend):
        numAsString = str(numToSend)
        self.sendStringToComPort(numAsString)

    def sendStringToComPort(self, message):
        self.sendBytesToComPort(message.encode('utf-8'))

    def sendBytesToComPort(self, bytesToSend):
        self.serialConnection.write(bytesToSend)


if __name__ == "__main__":
    comPort = "COM20"
    newMonitor = SerialMonitor(comPort)
    newMonitor.start()
    while True:
        userInput = str(input(">>"))
        newMonitor.sendStringToComPort(userInput)
