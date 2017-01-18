import threading, subprocess, os, sys, hashlib, ntpath

class ClientHandler(threading.Thread):

    def __init__(self, con, IP, server, update = False):                #takes in client connection, IP address of client, server, and option paramter on whether it updates a data log for the server        
        self.con = con
        self.IP = IP
        self.server = server
        self.quit = False

        self.update = update   #determines whether the handler will update the server log
        self.results = ''
        super(ClientHandler, self).__init__()

    def run(self):             #waits for client commands
        if self.update == True:
            self.server.updates(self.IP, "CONNECTED" , '_______')
        try:
            if self.server.passwords != None:                   #if the server isnt running passwords than it doesn't expect one from the client
                message = self.recvData()
                if hashlib.md5(self.server.salt + message.encode('utf-8')).hexdigest() + ':h' in self.server.passwords: #have to hash the password before dealing with it
                    self.sendData('PASSWORD ACCEPTED')
                else:
                    raise Exception('PASSWORD NOT ACCEPTED')
            
            message = ''
            while message != 'wq' and self.quit != True:    #waits for client command to quit or server command to quit
                message = self.recvData()
                if message == "wq":
                    self.quit = True
                    self.results = 'TERMINATING CONNECTION'
                    
                elif message[:3] == "-DF":      #sends file to client
                    fileName = message[4:]
                    if 'passwords.txt' in fileName or "datalog.txt" in fileName:
                        self.results = 'YOU DO NOT HAVE ACCESS TO THAT FILE'
                    else:
                        self.sendFile(fileName)
                    
                elif message[:3] == "-UF":          #received file from client
                    fileName = message[4:]
                    if 'passwords.txt' in fileName or "datalog.txt" in fileName:
                        self.results = 'YOU DO NOT HAVE ACCESS TO THAT FILE'
                    else:
                        self.recvFile(fileName)

                elif message[:2] == "cd":   #changes current directory
                    directory = message[3:]
                    try:
                        os.chdir(directory)
                        self.results = str(os.getcwd())
                        self.server.updates(self.IP, "cd" , directory)
                    except:
                        self.results = "Error in changing directory, directory may not exist"
                        
                else:           #sends command to command line, unless it has passwords.txt in it or rmdir, no touching!!
                    if 'passwords.txt' in message or "datalog.txt" in message:
                        self.results = 'YOU DO NOT HAVE ACCESS TO THAT FILE'
                    elif 'rmdir' in message.lower():
                        self.results = 'COMMAND NOT ALLOWED'
                    else: #pipes command to the command line and returns the results
                        cmd = subprocess.Popen(message, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                        self.results = (cmd.stdout.read() + cmd.stderr.read()).decode('utf-8')
                        lines = self.results.split('\n')
                        for line in lines:
                            if 'passwords.txt' in line:
                                lines.remove(line)
                                continue
                        self.results = '\n'.join(lines)
                        #self.results = self.results.replace('passwords.txt', 'new Text Document.txt')
                        if self.update:
                            self.server.updates(self.IP, message , '___') #update server log

                self.sendData(self.results)
                
                
        except Exception as e:  #if an error with the connection the handler shuts down
            try:
                self.sendData(str(e) + '\nEnding Connection due to Errors')
            except:
                pass
        finally:
            for x in self.server.currentCons:       #it always removes itself from the connections list
                if x[1] == self.IP:
                    self.server.currentCons.pop(self.server.currentCons.index(x))
                    if self.update == True:
                        self.server.updates(self.IP, "DISCONNECTED" , '___')
                    break
                
    def recvData(self):                            #recieves byte data and turns it into strings
        message = self.con.recv(1024)
        return message.decode('utf-8')

    def recvByteData(self):                        #never really used, still here on the off chance its used somewhere else and it'll cause an error if I remove it
        message = self.con.recv(1024)
        return message

    def recvFile(self, fileName):                   ##<-- exactly the same as clients function, view that for full documentation
        fileName = ntpath.basename(fileName)
        lock = self.lockFile(fileName)
        with open(fileName, 'wb') as file:
            data = "".encode('utf-8')
            error = False
            while True:
                self.con.send('ready'.encode('utf-8'))
                chunk = self.con.recv(1024)
                
                if sys.getsizeof(chunk) < 1024:
                    if '$$ERROR$$'.encode('utf-8') in chunk:
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
                if self.update:
                    self.server.updates(self.IP, "-UF" , fileName)
            else:
                file.close()
                os.remove(fileName)
                self.results = chunk.decode('utf-8').replace('$$ERROR$$', '')
                if self.update:
                    self.server.updates(self.IP, "-UF" , fileName + " ERROR")
            lock.release()
            
    def sendData(self, data, secondTime = False):
        try:
            if data == None or len(data) == 0 or sys.getsizeof(data) == 0:  #client expects data, so we send him at least something if we dont have any so it doesnt cause him an error
                data = 'NONE'
            data = data.encode('utf-8')
            dataSize = str(sys.getsizeof(data))
            while sys.getsizeof(dataSize.encode('utf-8')) < 55:
                dataSize = '0' + dataSize
            if sys.getsizeof(dataSize.encode('utf-8')):
                dataSize = dataSize[1:]
            self.con.send(dataSize.encode('utf-8')) #let the client know the data size we are sending
            self.con.recv(22)
            self.con.send(data)
            return True
        except Exception as e:
            if not secondTime:
                self.sendData(e, True)
            return False

    def sendFile(self, fileName):  #<--- nearly the same as clients function of the same name, look at that for full documentation, only difference is client can detect bad files before sending so we have less error handling here
        try:
            lock = self.lockFile(fileName)
            with open(fileName, 'rb') as file:
                while True:
                    chunk = file.read(1024)
                    self.con.recv(22)
                    self.con.send(chunk)
                    if sys.getsizeof(chunk) < 1024:
                        break
                        
            self.results = "FILE DOWNLOADED"
            lock.release()
            if self.update:
                self.server.updates(self.IP, '-UF' , fileName)    #updates server log
            return
        
        except Exception as e:
            self.con.recv(22)
            lock.release()
            self.con.send("$$ERROR$$".encode('utf-8'))
            self.results = str(e)
            if self.update:
                self.server.updates(self.IP, "-DF" , fileName + " ERROR")
            

    def lockFile(self, fileName):           #updates the global file locking list so no other threads deal with this file
        noFile = True   
        for x in self.server.fileLocks: 
            if x[0] == fileName:
                noFile = False
                x[1].acquire()      #if the file is on the list then acquire the lock and work with it
                x[2] = self.IP
                return x[1]
        if noFile:              #if there is no file like this on the list add it to the list
            lock = threading.Lock()
            lock.acquire()
            self.server.fileLocks.append([fileName, lock, self.IP])

            return lock
    
    def quit(self):             #sets a flag to quit when no important process, like downloading a file, are being done
        self.quit = True
