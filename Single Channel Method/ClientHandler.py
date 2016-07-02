import socket, sys, argparse, os, threading
import SocketConnection #what directly talks to the client

class ConnectionHandler(threading.Thread)

    def __init__(self, host, port, con, server, testing= False):
        self.host = host
        self.port = port

        #starts up a connection hadnling object, handles all data transfer        
        self.connection = SocketConnection(addr, port, con)

        self.server = server

        #recieve the client password and check if it is a known password
        password = self.connection.recvieveData()
        if not testing:
            self.privilege = server.checkPassword(password)

            self.quit = False

            if self.privilege == False:
                self.quit = True

        else:
            self.privilege = 'ADMIN'
    
        #runs the constructor for the thread class as well as this constructor
        super(ConnectionHandler, self).__init__()
        
    def run(self):
        #sends the initial command prompt
        self.transmitSocket.sendData(os.getcwd() + '>>')

        #the thread will complete on its own once a source triggers its exit flag
        while not self.quit:
            #read in user commands and execute their corresponding functions
            self.inData = self.connection.recieveData()

            if self.inData = None:
                pass
            
            elif self.inData[:3] == "-uf": #the client is uploading a file
                try:
                    self.connection.recieveFile(self.inData[4:])
   
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.inData[:3] == "-df": #the client wants to download a file
                try:
                    self.connection.recieveFile(self.inData[4:])
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.inData[:4] == "-ufs": #the client wants to upload multiple files whos names are seperated by spaces
                fileNames = self.inData[5:]
                fileNames = fileNames.split()
                    
                self.connection.recieveFiles()

                
            elif self.inData[:4] == "-dfs" #the client wants to download multiple files whos names are seperated by spaces
                try:
                    files = self.inData[5:]
                    files = files.split()
                    self.sendFiles(files)
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')

            elif self.inData[:3] == "-ud": #the client wants to upload a directory 
                try:
                    self.connection.recieveDir(self.inData[4:])

                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')

            elif self.inData[:3] == "-dd": #the client wants to download a directory
                try:
                    self.sendDir(self.inData[4:])
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')

            elif self.inData[:3] == "-ce": #connect to an email account remotely
                data = self.inData[4:]
                data = data.split()
                self.smtp = SmtpConnection(data[0], data[1], data[2])
                
            elif self.inData[:3] == "-sm": #send an email of the specified files
                data = self.inData[4:]
                data = data.split(',')
                files = data[2].split()
                self.smtp.sendMail(data[0], data[1], files)

            elif self.inData[:3] == '-wq': #close the connection between client and server
                self.quit = True
                self.closeChannel()
                
            elif self.inData[:2] == '-h': #displays all the possible commands
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
                
            elif self.inData[:3] == "-cd" and self.privilege == "ADMIN": #allows the suer to change server directories if they are an admin
                try:
                    os.chdir(self.inData[4:])
                except:
                    self.connection.sendData('Error Encountered: ' + str(sys.exc_info()[0]) + '\n')
                
            elif self.privilege == "ADMIN": #unknown commands are sent to the command line of the server if the client is an admin
                cmd = subprocess.Popen(self.inData, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                cmd = cmd.stdout.read() + cmd.stderr.read()
                self.connection.sendData(cmd)

            else:
                self.connection.sendData("Access Not Allowed to those commands\n")

            #send back the prompt to let the client know the server is ready for the next command
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
    #for testing purposes
    server = socket.socket()
    server.bind(('127.0.0.1', 5000))
    server.listen(5)

    con, addr = server.accept()
    handler = ClientHandler('127.0.0.1', 5000, con, None, True)
    handler.start()
    
