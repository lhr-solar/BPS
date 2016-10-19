import socket, argparse, threading, os, hashlib
from bClientHandler import clientHandler

class Server(threading.Thread):

    def __init__(self, IP= '127.0.0.1', port= 5000, backlogs= 100):
        self.IP = IP
        self.port = port

        
        self.sock = socket.socket()

        self.sock.bind((IP, port))
        print('Server started, listening for clients..')

        self.sock.listen(backlogs)
        self.currentCons = []
        self.fileLocks = [['passwords.txt', threading.Lock()]]
        lock = self.fileLocks[0][1]
        lock.acquire()
        self.clientLock = threading.Lock()

        self.salt = 'scooter'.encode('utf-8')
        if os.path.isfile('passwords.txt'):
            with open('passwords.txt', 'r') as file:
                self.passwords = [x.strip('\n') for x in file.readlines()]
                tempList = []
                for x in self.passwords:
                    if x.endswith(':h'):
                        tempList.append(x)
                    else:
                        tempList.append(hashlib.md5(self.salt + x.encode('utf-8')).hexdigest() + ':h')
                self.passwords = tempList

            with open('passwords.txt', 'w') as file:
                for x in self.passwords:
                    file.write(x + '\n')
                        
        else:
            self.passwords = None
            
        super(Server, self).__init__()

    def run(self):
        while True:
            con, addr = self.sock.accept()
            print('Connection made at IP: ', str(addr[0]))
            print('Passing off client..')
            client = clientHandler(con, addr[0], self)
            client.start()
            self.currentCons.append([client, addr[0]]) 
            self.displayClients()
            
        print('Closing server and connections..')
        self.sock.close()
        self.deleteAllClients()
            
    def displayClients(self):
        for num in range(len(self.currentCons)):
            print(str(num + 1) + ". " + self.currentCons[num][1])

    def deleteClient(self, index):
        self.clientLock.acquire()
        client = self.currentCons[index - 1][0]
        client.quit()
        self.currentCons.pop(index - 1)
        self.clientLock.release()
        
    def deleteAllClients(self):
        for x in range(len(self.currentCons)):
            self.deleteClient(x+1)
    
if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument('-ho', '--host', help= 'The host IP the server will bind to', type= str)
    parser.add_argument('-po', '--port', help= 'The host port the server will bind to', type= int)
    parser.add_argument('-bl', '--backlogs', help= 'The number of backloged connections allowed until rejecting further traffic', type= int)

    args = parser.parse_args()

    if(args.host == None):
        args.host = "127.0.0.1"
    if(args.port == None):
        args.port = 5000
    if(args.backlogs == None):
        args.backlogs = 100
        
    s = Server(args.host, args.port, args.backlogs)
    s.start()

        
