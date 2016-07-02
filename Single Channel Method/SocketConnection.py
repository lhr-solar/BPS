import socket, sys, argparse, os, threading, filelock
from Crypto.Cipher import AES
from Crypto.Hash import SHA256

class SocketConnection(): #only handles data coming in on its specified connection

    def __init__(addr, port, con= None):
        self.host = addr #ip address
        self.port = port

        if con:
            self.socket = con #Here is where we make the actual socket connection
            #self.encryptKey = self.recieveData()
            #self.IV = self.recieveData()
            #self.encryptor = AES.new(self.encryptKey, AES.MODE_CBC, self.IV)
        else:
            self.connectToHost(host, port)
    
        self.quit = False #if this is set true, than the connection will close as soon as it done handling files or anything that stopping in the middle of would be bad 
        self.quitLock = threading.Lock()#to prevent anymore processes from being opened once the quit mode is engage
        
    
    #Below is all of our functions concerning sending data through our socket
    def sendData(self, data, size= None): #The backbone of our socket connections ability to send data, all functions concerning sending data go through here
        if size is None:
            size = str(sys.getsize(data)).zfill(1024) #padds the gile size until it is a 1024 bit chunk, as that is what the recieving end expects
        try:
            self.socket.send(str(size).encode('utf-8'))
            if not binary: #b is sent to confirm the message is not intended to be in binary, a p confirms the message is intended to be bianary data
                self.socket.send((str('b').zfill(1024)).encode('utf-8'))
                #data = encryptData(data)
                self.socket.send(data.encode('utf-8')) #now the data can be sent as one full size as the other end knows what size to expect
            else:
                self.socket.send((str(1).zfill(1024)))
                #data = self.encryptData(data) #encrypts the data
                self.socket.send(data)
            return True
        except: #this message is sent to notify of an error on the other side
            self.socket.send("$ERROR$".zfill(int(size)))
            return False
        
    def sendFile(self, filePath):
            
        with FileLock(filePath): #prevents the file form being opened up by another thread        
            with open(path, 'rb') as file:
                data = file.read()
                status = self.sendData(data, True)
                
                return status
    
    def sendFiles(self, fileNames):
        statuses = []
        for name in fileNames:
            statuses.append(self.sendFile(name))

        return statuses
        
    def sendDir(self, directory, clearance = 1, topLevel = True):
        if topLevel: #on the first run through we recieve the data on how deep we are going into these directories, each level of clearance is another directory level
            clearence = self.recieveData()
            clearence = int(clearance)

        directories = []
        if 'C:' not in directory: #if there is no file path before the directory name we add it in to keep track of where we are
            directory = os.path.join(os.getpath(cwd), directory) #adds the full path to the name
        for file in os.listdir(directory): #sorting files from directories to deal with differently
           path = os.path.join(directory, file)  #adding the path name to the file so we can keep track of where it is
            if os.path.isfile(path):
                self.sendData("SENDING FILE: " + path) #let the other end know we are sending a file
                self.sendFile(path)
            elif os.path.isdir(path) && clearance != 0: #if we still have clearance then add the directory to the list of directories we will go through after all the files are done
                directories.append(path)

            else:
                pass
            
        if clearence != 0: 
            for direc in directories:
                path = os.path.join(direc, directory)
                self.socket.sendData("NEW DIRECTORY: " + direc) #let the other end know we are in a new directory
                self.sendDir(path, clearance - 1, False) #take out a level of clearence and run through the new directory

        self.sendData('__DONE__') #lets the other end know we are done

    #Below is all of our functions conerning recieving data from our socket
    def isError(data): #thuis checks if a notification that an error was encountered on the sending side was sent to us
        if "$ERROR$" in data[-6:]:
            return True
        else:
            return False
    
    def recieveData(self, fileName= None): #The backbone of our socket connection's ability to recieve data, all functions concerning recieving data use this
        try:
            size = self.socket.recv(1024).decode('utf-8').strip('0')
            binary = self.socket.recv(1024).decode('utf-8').strip('0')

            if binary == "b": #if true the data being sent is supposed to be in binary, otherwise it is assumed to be encoded text
                binary = True
            else:
                binary = False

            if self.isError(size) or self.isError(binary): #if an error is encountered then abort the process
                return False

            data = self.socket.recv(int(size))
            if self.isError(data): #one final error check before doing anything with or writing the data to a file
                return False

            #data = self.decryptData(data) 
            
            if fileName is not None:
                
                try:

                    if binary:
                        file = open(fileName, 'ab')
                    else:
                        file = open(fileName, 'a')
                        data = data.decode('utf-8')

                    file.write(data)

                except:
                    return False
            
                file.close()
        
            return data
        
        except:
            return False

    def recieveFile(self, fileName):
        with FileLock(fileName): #this prevents the file frmo being opened by other connections
            return self.recieveData(fileName)
    
    def recieveFiles(self, fileNames):
        statuses = []
        for file in fileNames:
            statuses.append(self.recieveFile(file))

        return statuses
        
    def recieveDir(self, path):
        if not os.path.exists(path) or not os.path.isdir(path):
            if "C:" not in path:
                os.path.join(os.getcwd(), path)

            os.makedirs(path)
            
        while True:
            fileName = self.recieveData() #recieves the fileName
            if fileName == "__DONE__" : #checks if the current directory is finished being sent, then below checks if an error was encountered
                break
            
            elif not fileName:
                pass
            
            elif fileName[0:12] == "SENDING FILE:": #if a file is being sent, then prepare for that
                filePath = os.path.join(path, fileName[14:])
                self.recieveData(filePath)
                
            elif fileName[0:14] == "NEW DIRECTORY:": #and if a new directory is being sent, then call this function again for some good ol recursive fun
                newDirec = fileName[16:]
                newDirecPath = os.path.join(path, newDirec)
                self.recieveDir(newDirecPath)
    

##    #Encryption functions for all of our data sending
##    def encryptData(self, data):
##        return self.encryptor.encrypt(data)
##
##    #Decryption functions for all of our data receiving
##    def decryptData(self, data):
##        return self.encryptor.decrypt(data)

    #File compression algorithms for sending files, still under construction
    #def huffminEncoding(self, file):
    

    #Managing or changing the active socket, these functions excoet for closing the connection aren't used with the additonal server code
    def closeSocket(self):
        self.socket.close()

    def connectToHost(self, host, port): #takes care of some preliminary things when connectin to a new socket

        try:
            self.socket.connect(host, port)
            #self.encryptKey = self.recieveData()
            #self.IV = self.recieveData()
            #self.encryptor = AES.new(self.encryptKey, AES.MODE_CBC, self.IV)
            return True 
        except:
            return False
      

    def changeSocket(self, host, port): #Will ask the user for input if none is given
        self.socket.close()
        return self.connectToHost(host, port):
      
    
    def checkQuit(): #if the thread is queued to close, it will end all processes as soon as it is free and not doing anything unstable
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
        
