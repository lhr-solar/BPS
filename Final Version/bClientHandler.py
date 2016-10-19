import threading
import subprocess
import os
import sys
import hashlib

class clientHandler(threading.Thread):

    def __init__(self, con, IP, server):
        self.con = con
        self.IP = IP
        self.server = server
        self.quit = False

        self.results = ''
        super(clientHandler, self).__init__()

    def run(self): 
        try:
            if self.server.passwords != None:
                message = self.recvData()
                if hashlib.md5(self.server.salt + message.encode('utf-8')).hexdigest() + ':h' in self.server.passwords:
                    self.sendData('PASSWORD ACCEPTED')
                else:
                    raise Exception('PASSWORD NOT ACCEPTED')
            
            message = ''
            while message != 'wq' and self.quit != True:
                message = self.recvData()
                if message == "wq":
                    self.quit = True
                    self.results == 'TERMINATING CONNECTION'

                elif message[:3] == "-DF":
                    fileName = message[4:]
                    if fileName == 'passwords.txt':
                        self.results = 'YOU DO NOT HAVE ACCESS TO THAT FILE'
                    else:
                        self.sendFile(fileName)
                    
                elif message[:3] == "-UF":
                    fileName = message[4:]
                    if fileName == 'passwords.txt':
                        self.results = 'YOU DO NOT HAVE ACCESS TO THAT FILE'
                    else:
                        self.recvFile(fileName)

                elif message[:2] == "cd":
                    directory = message[3:]
                    try:
                        os.chdir(directory)
                        self.results = str(os.getcwd())
                    except:
                        self.results = "Error in changing directory, directory may not exist"
                        
                else:
                    if 'passwords.txt' in message:
                        self.results = 'YOU DO NOT HAVE ACCES TO THAT FILE'
                    elif 'rmdir' in message.lower():
                        self.results = 'COMMAND NOT ALLOWED'
                    else:
                        cmd = subprocess.Popen(message, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                        self.results = (cmd.stdout.read() + cmd.stderr.read()).decode('utf-8')
                        self.results = self.results.replace('passwords.txt', 'new Text Document.txt')
                        
                self.sendData(self.results)

        except Exception as e:
            try:
                self.sendData(str(e) + '\nEnding Connection due to Errors')
            except:
                pass
        finally:
            for x in self.server.currentCons:
                if x[1] == self.IP:
                    self.server.currentCons.pop(self.server.currentCons.index(x))
                    break
                
    def recvData(self):
        message = self.con.recv(1024)
        return message.decode('utf-8')

    def recvByteData(self):
        message = self.con.recv(1024)
        return message

    def recvFile(self, fileName):
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
            else:
                file.close()
                os.remove(fileName)
                self.results = chunk.decode('utf-8').replace('$$ERROR$$', '')
            lock.release()
            
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
            raise Exception('Error: ' + str(sys.exc_info()[0]))
            return False

    def sendFile(self, fileName):
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
            return
        except Exception as e:
            self.con.recv(22)
            lock.release()
            self.con.send("$$ERROR$$".encode('utf-8'))
            self.results = str(e)

    def lockFile(self, fileName):
        noFile = True
        for x in self.server.fileLocks:
            if x[0] == fileName:
                noFile = False
                x[1].acquire()
                x[2] = self.IP
                return x[1]
        if noFile:
            lock = threading.Lock()
            lock.acquire()
            self.server.fileLocks.append([fileName, lock, self.IP])

            return lock
    
    def quit(self):
        self.quit = True
