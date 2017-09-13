import serial

class SerialMonitor():

    def __init__(self, comPort, baudRate = 9600):
        self.comPort = comPort
        self.baudRate = baudRate
        self.serialConnection = serial.Serial(comPort, baudRate)

    def displaySerialMonitor(self):
        serialMonitorData = self.getLineFromComPort()
        print(serialMonitorData, end= '')

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

if __name__ == "__main__":
    comPort = "COM20"
    newMonitor = SerialMonitor(comPort)
    while True:
        newMonitor.displaySerialMonitor()
