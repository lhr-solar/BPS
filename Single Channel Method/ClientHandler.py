import socket, sys, argparse, os, threading
import SocketConnection #what directly talks to the client

class ConnectionHandler(threading.Thread)

    def __init__(self, host, port, con, server):
        self.host = host
        self.port = port
        
        self.connection = SocketConnection(addr, port, con)

        self.server = server

        password = self.connection.recvieveData()
        privilege = server.checkPassword(password)

        self.quit = False

        if privilege == False:
            self.quit = True

        super(ConnectionHandler, self).__init__()
        
    def run(self):
        self.transmitSocket.sendData(os.getcwd() + '>>')
        
        while not self.quit:
            self.inData = self.connection.recieveData()

            if self.inData = None:
                pass
            
            elif self.inData[:3] == "-uf":
                try:
                    self.connection.recieveFile(self.inData[4:])
   
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.inData[:3] == "-df":
                try:
                    self.connection.recieveFile(self.inData[4:])
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.inData[:4] == "-ufs":
                fileNames = self.inData[5:]
                fileNames = fileNames.split()
                    
                self.connection.recieveFiles()

                
            elif self.inData[:4] == "-dfs"
                try:
                    files = self.inData[5:]
                    files = files.split()
                    self.sendFiles(files)
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')

            elif self.inData[:3] == "-ud":
                try:
                    self.connection.recieveDir(self.inData[4:])

                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')

            elif self.inData[:3] == "-dd":
                try:
                    self.sendDir(self.inData[4:])
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')

            elif self.inData[:3] == "-ce":
                data = self.inData[4:]
                data = data.split()
                self.smtp = SmtpConnection(data[0], data[1], data[2])
                
            elif self.inData[:3] == "-sm":
                data = self.inData[4:]
                data = data.split(',')
                files = data[2].split()
                self.smtp.sendMail(data[0], data[1], files)

            elif self.inData[:3] == '-wq':
                self.quit = True
                self.closeChannel()
                
            elif self.inData[:2] == '-h':
                data = "-uf\t\tUpload a file, command followed by the file name\n"
                data += "-df\t\tDownload a file, command followed by the file name\n"
                data += "-ufs\t\tUpload Files, command followed by list of file names seperated by spaces\n"
                data += "-dfs\t\tDownload Files, command followed by list of file names seperated by spaces\n"
                data += "-ud\t\tUpload Directory, command followed by the directory name\n"
                data += "-dd\t\tDownload Directory, command followed by the directory name\n"
                data += "-ce\t\tConnects to an SMTP server to send emails remotely, give it your email, password, and host seperated by spaces\n"
                data += "-sm\t\tSend the email with all built up attachments to the specified email, enter in paramters like: receiver, text , files(seperated by spaces)\n"
                data += "-wq\t\tEnds the current session\n"
                data += "*****All normal linux commands work on the server as well as the custom commands*****\n"
                data += "*****Some directories will be restricted due to your privilege level*****\n"
                try:
                    self.connection.sendData(data)
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.inData[:3] == "-cd" and self.privilege == "ADMIN":
                try:
                    os.chdir(self.inData[4:])
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.privilege == "ADMIN":
                cmd = subprocess.Popen(self.inData, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                cmd = cmd.stdout.read() + cmd.stderr.read()
                self.connection.sendData(cmd)

            else:
                self.connection.sendData("Access Not Allowed to those commands\n")
                
            self.connection.sendData(os.getcwd() + ">> ")

            
        
    #Important support functions used when the thread is apart of a server to avoid corrupting files
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


    

if __name__ == "__main__":
    

