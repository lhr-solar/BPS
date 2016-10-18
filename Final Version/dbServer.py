import socket
from dbClientHandler import clientHandler
import argparse
import threading
import time
import os

class Server(threading.Thread):

    def __init__(self, IP= '127.0.0.1', port= 5000, backlogs= 100):
        self.IP = IP
        self.port = port

        
        self.sock = socket.socket()

        self.sock.bind((IP, port))
        print('Server started, listening for clients..')

        self.sock.listen(backlogs)
        self.currentCons = []
        self.fileLocks = []
        self.clientLock = threading.Lock()

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

    def updates(self, ip, command, filename):
        if os.path.isfile('datalog.txt'):
            with open('datalog.txt', 'a') as file:
                file.write(ip + ',' + command + ',' + filename + ',' + time.strftime('%d/%m/%Y %H:%M:%S') + '\n')
        else:
            with open('datalog.txt', 'w') as file:
                file.write('IP ADDRESS, COMMAND, FILENAME, DATE/TIME\n')
                file.write(ip + ',' + command + ',' + filename + ',' + time.strftime('%d/%m/%Y %H:%M:%S') + '\n')        
    
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

        

