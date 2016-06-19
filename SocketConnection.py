import socket, sys, argparse, os, threading
from Crypto.Cipher import AES
from Crypto.Hash import SHA256

class SocketConnection() #only handles data coming in on its specified connection
{

    def __init__(addr, port, con= None):
        self.host = addr #ip address
        self.port = port

        if con:
            self.socket = con #Here is where we make the actual socket connection   
        else:
            self.connectToHost()
    
        self.quit = False #if this is set true, than the connection will close as soon as it done handling files or anything that stopping in the middle of would be bad 
        self.quitLock = threading.Lock()#to prevent anymore processes from being opened once the quit mode is engage
        
    
    #Below is all of our functions concerning sending data through our socket
    def sendData(self, data): #The backbone of our socket connections ability to send data
        if size is None:
            size = str(sys.getsize(data)).zfill(1024) #padds the gile size until it is a 1024 bit chunk, as that is what the recieving end expects
        try:
            self.socket.send(str(size).encode('utf-8'))
            if not binary: #a 0 is ent to confirm the message is not intended to be in binary, a 1 confirms the message is intended to be bianary data
                self.socket.send((str(0).zfill(1024)).encode('utf-8'))
                data = encryptData(data)
                self.socket.send(data.encode('utf-8')) #now the data can be sent as one full size as the other end knows what size to expect
            else:
                self.socket.send((str(1).zfill(1024)).encode('utf-8')) #
                data = self.encryptData(data)
                self.socket.send(data)
            return True
        except: #this message is sent to notify of an error on the other side
            self.socket.send("$ERROR$".zfill(int(size)))
            return False
        
    def sendFile(self, filePath):
            
        if not 'C:' in path:
            path = os.path.join(path, os.getcwd())

        
        with open(path, 'rb') as file:

            if self.server is not None:
                self.markFileAsOpen(path)
                
            data = file.read()
            status = self.sendData(data, True)
            
            file.close()
            self.markFileAsClosed(path)
            return status
    
    def sendFiles(self, fileNames):
        statuses = []
        for name in fileNames:
            statuses.append(self.sendFile(name))
            
    def sendDir(self, directory, clearance = 1, topLevel = True):
        if topLevel:
            clearence = self.recieveData()
            clearence = int(clearance)

        directories = []
        if 'C:' not in directory:
            directory = os.path.join(os.getpath(cwd), directory)
        for file in os.listdir(directory):
           path = os.path.join(directory, file) 
            if os.path.isfile(path):
                self.sendData("SENDING FILE: " + path)
                self.sendFile(path)
            elif os.path.isdir(path) && clearance != 0:
                directories.append(path)

            else:
                pass
            
        if clearence != 0:
            for direc in directories:
                path = os.path.join(direc, directory)
                self.socket.send("NEW DIRECTORY: " + direc)
                self.sendDir(path, clearance - 1, False)

        self.sendData('__DONE__')

    #Below is all of our functions conerning recieving data from our socket
    def isError(data):
        if "$ERROR$" in data[-6:]:
            return True
        else:
            return False
    
    def recieveData(self, fileName= None): #The backbone of our socket connection's ability to recieve data
        size = self.socket.recv(1024).decode('utf-8')
        binary = self.socket.recv(1024).decode('utf-8')
        if binary == "1":
            binary = True
        else:
            binary = False

        data = self.socket.recv(int(size))
        data = self.decryptData(data)
        if isError(size) or (isError(binary) or isError(data)):
            return False
        
        if fileName is not None:
            
            try:
                if "C:" not in fileName:
                    fileName = os.path.join(os.getcwd(), fileName)

                if int(binary) == 1:
                    file = open(fileName, 'ab')
                else:
                    file = open(fileName, 'a')
                    data = data.decode('utf-8')
                if data[-7:] == "$ERROR$":
                    return 0
                else:
                    file.write(data)

            except:
                return False
        
            file.close()
    `       self.markFileAsClosed(fileName)
    
        return data

    def recieveFile(self.fileName):
        return self.recieveData()
    
    def recieveFiles(self, fileNames):
        for file in fileNames:
            self.recieveData(file)
        
    def recieveDir(self, path):
        if not os.path.exists(path) or not os.path.isdir(path):
            if "C:" not in path:
                os.path.join(os.getcwd(), path)

            os.makedirs(path)
        while True:
            fileName = self.recieveData()
            if fileName == "__DONE__":
                break
            
            elif fileName[0:12] == "SENDING FILE:":
                filePath = os.path.join(path, fileName[14:])
                self.recieveData(filePath)
                
            elif fileName[0:14] == "NEW DIRECTORY:":
                newDirec = fileName[16:]
                newDirecPath = os.path.join(path, newDirec)
                self.recieveDir(newDirecPath)
    

    #Encryption functions for all of our data sending
    def encryptData(self, data):
        return self.encryptor.encrypt(data)

    #Decryption functions for all of our data receiving
    def decryptData(self, data):
        return self.encryptor.decrypt(data)

    #File compression algorithms for sending files, still under construction
    def huffminEncoding(self, file):
        pass

    #Managing or changing the active socket, these functions excoet for closing the connection aren't used with the additonal server code
    def closeSocket(self):
        self.socket.close()

    def connectToHost(self, host, port):

        try:
            self.socket.connect(host, port)
            self.encryptKey = self.recieveData()
            self.IV = self.recieveData()
            self.encryptor = AES.new(self.encryptKey, AES.MODE_CBC, self.IV)
            return True #a return of 1 indicated a connection was successful
        except:
            return False #a return of zero indicates a connection was not successful
      

    def changeSocket(self, host, port): #Will ask the user for input if none is given
        self.socket.close()
        return self.connectToHost(host, port):
      
    
    def checkQuit(): #if the thread is queued to close, it will 
        if self.quit:
            self.quitLock.acquire()
            self.socket.close()
            sys.exit(0)
            
    
##Below is the testing procedures for working out any bugs in this code, goes along with the dummy server program
if __name__ == "__main__":
    parser = argparse.ArgumentParser() 

    parser.add_argument("host", help= "Target Socket IP", type= str) #Three mandatory command line arguments must be entered when running this program
    parser.add_argument("port", help= "Target Socket Port", type= int) #First is the ip to connect to, the second is the port to connect to

    args = parser.parse_args()

    try: #attempt a socket connection
        sock = socketConnection(args.host, args.port)

    except:
        print("Error: " + str(sys.exc_info()[0]))
        print("Testing aborted, target connection could not be made with given parameters:")
        print("Host IP: {0}\t\t Host Port: {1}").format(args.host, args.port)
        print("Double check that these are correct, then try again")


    userInput = ""
    while userInput is not "wq": #the tester can enter any commands they wish, and then quit using wq
        userInput = str(input(">>"))

        if(userInput)

    print("Closing Socket Connection..")
    sock.close()
    print("Test Finished, connection to the test socket has been terminated")
        
