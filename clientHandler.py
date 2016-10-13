import threading, sys, subprocess, os
from filelock import FileLock

class clientHandler(threading.Thread):

    def __init__(self, con, IP):
        self.con = con
        self.IP = IP

        self.quiting = False
        
        super(clientHandler, self).__init__()

    def run(self):
        msg = ""
        while msg != 'wq' or not self.quiting:
            msg = self.con.recv(1024).decode('utf-8')
  
            if msg[:3] == '-UF':
                fileName = msg[4:]
                data = self.recvData()
                if data.decode('utf-8') != 'File Not Found':
                    try:
                        with FileLock(fileName):
                            with open(fileName, 'wb') as file:
                                file.write(data)
                        print('File recieved')
                    except:
                        print('Error writing to file')
               
                else:
                    print('File Not Found')
                    
            elif msg[:3] == '-DF':
                fileName = msg[4:]
                self.sendData(fileName)
                print('File sent')

            elif msg[:2] == 'cd':
                try:
                    os.chdir(msg[3:])
                    self.sendData(os.getcwd().encode('utf-8'), True)
                except:
                    self.sendData(str(sys.exc_info()[0]).encode('utf-8'), True)
                    
            else:
                cmd = subprocess.Popen(msg, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                cmd = cmd.stdout.read() + cmd.stderr.read()
                if cmd == '':
                    cmd = 'done'.encode('utf-8')
                self.sendData(cmd, True)

        self.con.close()
        
    def sendData(self, data, isData= False):
        if not isData:
            try:
                with FileLock(data):
                    with open(data, 'rb') as file:
                        while True: 
                            chunk = file.read(1024)

                            if not chunk:
                                self.con.send('DONE'.encode('utf-8'))
                                break
                            elif sys.getsizeof(chunk) < 1024:
                                self.con.send(chunk)
                                break
                            else:
                                self.con.send(chunk)
            except FileNotFoundError:
                self.con.send('File Not Found'.decode('utf-8'))
                    
            except:
                self.quit()
                    
        else:
            index = 1025
            while True:
                chunk = data[:index]
                try:
                    if not chunk:
                            self.con.send('DONE'.encode('utf-8'))
                            break
                    elif sys.getsizeof(chunk) < 1024:
                        self.con.send(chunk)
                        break
                    else:
                        self.con.send(chunk)
                except:
                    self.quit()
                    
                index += 1024
                
    def recvData(self):
        data = "".encode('utf-8')
        while True:
            chunk = self.con.recv(1024)
            
            if sys.getsizeof(chunk) < 1024:
                data += chunk
                break
            elif chunk.decode('utf-8') == 'DONE':
                break
            elif chunk.decode('utf-8') == 'File Not Found':
                return chunk
            else:
                data += chunk
            
        return data #returns bytes

    def quit(self):
        self.quiting = True
