import serial

class SerialMonitor():

    def __init__(self, comPort, baudRate = 9600):
        self.comPort = comPort
        self.baudRate = baudRate
        self.serialConnection = serial.Serial(comPort, baudRate)

    def readComPort():
        serialMonitorData = self.serialConnection.read()
        print(serialMonitorData)


if __name__ == "__main__":
    comPort = 7
    newMonitor = SerialMonitor(comPort)
    while True:
        newMonitor.readComPort()
