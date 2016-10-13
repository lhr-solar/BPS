import socket, sys, argparse, threading

class ClientSocket():

    def __init__(self, IP= '127.0.0.1', port= 5000):
        self.IP = IP
        self.port = port

        try:
            self.sock = socket.socket()
            self.sock.connect((IP, port))

            self.closed = False
        except:
            self.closed = True
            
        self.results = ''
        
    def sendCommand(self, msg):
        if self.closed == True:
            self.results = 'Connection closed, cannot send command'
            return

        self.sock.send(msg.encode('utf-8'))

        if msg[:3] == '-UF':
            fileName = msg[4:]
            self.sendData(fileName)
            self.results = 'File Sent'
            
        elif msg[:3] == '-DF':
            fileName = msg[4:]
            data = self.recvData()
            if data.decode('utf-8') != 'File Not Found' :
                try:
                    with FileLock(fileName):
                        with open(fileName, 'wb') as file:
                            file.write(data)
                    self.results = 'File Received'
                except:
                    self.results = 'Error writing to File'          
                
            else:
                self.results = data.decode('utf-8')
            
        else:
            self.results = self.recvData().decode('utf-8')
            print(self.results)
            
    def sendData(self, data, isData= False):
        if not isData:
            try:
                with FileLock(data):
                    with open(data, 'rb') as file:
                        while True: 
                            chunk = file.read(1024)

                            if not chunk:
                                self.sock.send('DONE'.encode('utf-8'))
                                break
                            elif sys.getsizeof(chunk) < 1024:
                                self.sock.send(chunk)
                                break
                            else:
                                self.sock.send(chunk)

            except FileNotFoundError:
                self.sock.send('File Not Found'.encode('utf-8'))
                
            except:
                print(str(sys.exc_info()[0]))
                self.closeCon()
            
        else:
            index = 1025
            while True:
                chunk = data[:index]

                try:
                    if not chunk:
                            self.sock.send('DONE'.encode('utf-8'))
                            break
                    elif sys.getsizeof(chunk) < 1024:
                        self.sock.send(chunk)
                        break
                    else:
                        self.sock.send(chunk)

                except:
                    self.quit()
                
                index += 1024

    def recvData(self):
        data = "".encode('utf-8')
        while True:
            chunk = self.sock.recv(1024)
            
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


    def closeCon(self):
        print('Closing Con..')
        self.sendCommand('wq')
        self.closed = True
        self.sock.close()

    def reopen(self):
        if self.closed:
            try:
                self.closed = False
                self.sock = socket.socket()
                sock.connect(self.IP, self.port)
                self.results = 'Connection opened'
            except:
                self.results = 'Problem opening connection'
                
        else:
            self.results = "Connection already opened"

    def changeIP(self, IP):
        self.IP = IP
        self.closeCon()
        self.reopen()
        
    def changePort(self, port):
        self.port = port
        self.closeCon()
        self.reopen()

    def changeConnection(self, IP, port):
        self.IP = IP
        self.port = port
        self.closeCon()
        self.reopen()
        
if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument('-ho', '--host', help= 'The host IP to connect to', type= str)
    parser.add_argument('-po', '--port', help= 'The port number to connect to', type= int)

    args = parser.parse_args()

    if not args.host:
        args.host = '127.0.0.1'
    if not args.port:
        args.port = 5000
    
    client = ClientSocket(args.host, args.port)
    msg = ""
    while msg != 'wq':
        msg = input('>>> ')
        client.sendCommand(msg)
