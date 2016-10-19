import socket, sys, os, argparse

class Client():

    def __init__(self, IP= '127.0.0.1', port= 5000, password= None):

        self.IP = IP
        self.port = port
        
        self.con = socket.socket()
        self.con.connect((IP, port))

        print(4)
        if password != None:
            self.con.send(password.encode('utf-8'))
            self.results = self.recvData()
            if 'NOT' in self.results:
                    raise Exception('PASSWORD NOT ACCEPTED')
        print(6)
        
    def recvFile(self, fileName):
        data = ''.encode('utf-8')
        error = False

        with open(fileName, 'wb') as file:
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
                        file.write(data)
                else:
                        file.close()
                        os.remove(fileName)
                        
    def sendFile(self, fileName):
            try:
                    with open(fileName, 'rb') as file:
                            while True:
                                    self.con.recv(22)
                                    data = file.read(1024)
                                    self.con.send(data)
                                    if sys.getsizeof(data) < 1024:
                                            break
            except Exception as e:
                    self.con.recv(22)
                    self.con.send(('$$ERROR$$' + str(e)).encode('utf-8'))

    def recvData(self):
            results = self.con.recv(1024)
            return self.con.recv(int(results)).decode('utf-8')


    def sendCommand(self, message):
        self.con.send(message.encode('utf-8'))
        if message[:3] == "-DF":
            fileName = message[4:]
            if fileName != 'passwords.txt':
                self.recvFile(fileName)

        elif message[:3] == "-UF":
            fileName = message[4:]
            if fileName != 'passwords.txt':
                self.sendFile(fileName)

        self.results = self.recvData()
        
        
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--ip', help= 'The IP address the client will connect to', type= str)
    parser.add_argument('-p', '--port', help= 'Port on the server machine to connect to', type= str)
    parser.add_argument('-ps', '--password', help= 'password to send to the server, leave blank if no password', type= str)

    args = parser.parse_args()

    if args.ip == None:
        args.ip = '127.0.0.1'
    if args.port == None:
        args.port = 5000

    client = Client(args.ip, args.port, args.password)
    message = ''
    while message != 'wq':
        message = str(input('>>'))
        client.sendCommand(message)
        print(client.results)
        
