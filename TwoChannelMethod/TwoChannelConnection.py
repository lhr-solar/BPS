import socket, os, sys, subprocess, threading, SocketConnection

class TwoChannelConnection():

    def openChannel(self, port):
        sock = SocketConnection(self.host, port)
        sock.connect((self.host, port))
        return sock
        

    def manageInput(self):
        
        while not self.quit:

            self.inData = recvSocket.recieveData()

            if self.inData = None:
                pass
            
            elif self.inData[:3] == "-uf":
                try:
                    self.recvSocket.recieveFile(self.inData[4:])
   
                except:
                    pass
                
            elif self.inData[:3] == "-df":
                try:
                    self.commandLock.acquire()
                    self.transCommands.append(self.inData)
                    self.commandLock.release()
                except:
                    pass
                
            elif self.inData[:4] == "-ufs":
                fileNames = self.inData[5:]
                fileNames = fileNames.split()
                    
                self.recvSocket.recieveFiles()

                
            elif self.inData[:4] == "-dfs"
                try:
                    self.commandLock.acquire()
                    self.transCommands.append(self.inData)
                    self.commandLock.release()
                except:
                    pass

            elif self.inData[:3] == "-ud":
                try:
                    self.recvSocket.recieveDir(self.inData[4:])

                except:
                    pass

            elif self.inData[:3] == "-dd":
                try:
                    self.commandLock.acquire()
                    self.transCommands.append(self.inData)
                    self.commandLock.release()
                except:
                    pass
                
            elif self.inData[:2] == '-h':
                data = "-uf\t\tUpload a file, command followed by the file name\n"
                data += "-df\t\tDownload a file, command followed by the file name\n"
                data += "-ufs\t\tUpload Files, command followed by list of file names seperated by spaces\n"
                data += "-dfs\t\tDownload Files, command followed by list of file names seperated by spaces\n"
                data += "-ud\t\tUpload Directory, command followed by the directory name\n"
                data += "-dd\t\tDownload Directory, command followed by the directory name\n"
                data += "*****All normal linux commands work on the server as well as the custom commands*****\n"
                data += "*****Some directories will be restricted due to your privilege level*****\n"
                try:
                    self.commandLock.acquire()
                    self.transCommands.append(self.inData + ' ' + data)
                    self.commandLock.release()
                except:
                    pass
                
            elif self.inData[:3] == "-cd" and self.privilege == "ADMIN":
                try:
                    os.chdir(self.inData[4:])
                except:
                    pass
                
            elif self.privilege == "ADMIN":
                cmd = subprocess.Popen(self.inData, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                cmd = cmd.stdout.read() + cmd.stderr.read()
                self.recvSocket.sendData(cmd)

            else:
                self.recvSocket.sendData("Access Not Allowed to those commands\n")
                
            self.recvSocket.sendData(os.getcwd() + ">> ")
            
    def manageOutput(self):
        while len(self.transCommands) == 0:
            pass

        command = self.transCommands[0]

        if command[:3] == '-df':
            self.transmitSocket.sendFile(command[4:])

        elif commamnd[:4] == '-dfs':
            fileNames = command[5:]
            fileNames = command.split()
            

            self.transmitSocket.sendFiles(fileNames)

            
        elif commamnd[:3] == '-dd':
            self.transmitSocket.sendDir(command[4:])

            
        elif command[:2] == '-h':
            self.transmitSocket.sendData(command[3:])
        
        self.commandLock.acquire()
        self.transCommands.remove(command)
        self.commandLock.release()
    
    def closeChannel(self):
        if self.quit = True:
            self.recvSocket.quit = True
            self.transmitSocket.quit = True

        sys.exit()
            

    #Important support functions used when the thread is apart of a server to avoid corrupting files, not needed for windows as files are locked when opened
##    def isFileOpen(self, path): #makes sure another thread does not have the file in question open
##        for file in self.server.openFiles:
##            if path == file:
##                return True
##
##        return False
##
##    def markFileAsOpen(self, path):
##        for file in self.server.openFiles:
##            if file == path:
##                return False
##            
##        self.server.openFiles.append(path)
##        return True
##
##    def markFileAsClosed(self, path):
##        self.server.openFiles.remove(path)



    def __init__(self, host, port, cons, privilege, server):
        self.host = host

        self.recvPort = port1
        self.sendPort = port2

        self.recvSocket = SocketConnection(host, port, cons[0])
        self.transmitSocket = SocketConnection(addr, port, cons[1])

        self.privilege = privilege

        self.server = server

        #os.chdir('//root//home//pi//codeFolder//')

        self.recvManager = threading.Thread(target= manageInput)
        self.transmitManager = threading.Thread(target= manageOutput)
        self.recvManager.start()
        self.transmitManager.start()
        
        self.commandLock = threading.Lock()
        self.transCommands = []
        
        self.quit = False


if "__name__" == __main__:
    socket = socket.socket()
    socket.bind(('127.0.0.1', 5000))
    socket2 = socket.bind()
    socket2.bind(('127.0.0.1', 5000))
    
    channel = TwoChannelConnection('127.0.0.1', socket, socket2, 'ADMIN')
