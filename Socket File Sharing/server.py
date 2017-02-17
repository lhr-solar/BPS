import socket, argparse, threading, os, hashlib, time
from clientHandler import ClientHandler

class Server(threading.Thread):

    def __init__(self, IP= '127.0.0.1', port= 5000, backlogs= 100, update= False):         #only needs IP, port, and number of incoming connections that are allowed to be backloged before we start rejecting people
        self.IP = IP
        self.port = port
        
        self.sock = socket.socket()     

        self.sock.bind((IP, port))          #setting up the server
        print('Server started, listening for clients..')

        self.sock.listen(backlogs)                                                                  #start up the server and set the limit on backloged connections
        self.currentCons = []                                                                       #current connections to the server, 2D stored like [IP, connectionObject]
        self.fileLocks = [['passwords.txt', threading.Lock()], ['datalog.txt', threading.Lock()]]                                      #sets up a list of currently used files to avoid different clients from accessing it at the same time
        lock = self.fileLocks[0][1]                                                                 #setting up a lock on the passwords file to avoid anyone accessing it
        lock.acquire()
        lock2 = self.fileLocks[1][1]
        lock2.acquire()
        self.clientLock = threading.Lock()                                                          #this lock is for when threads are trying to remove themselves from the client list

        self.update = update #determines whether a log of server flow is kept
        if self.update:
            self.updates("\n____________________", "SERVER START UP", "________________________\n")
        self.salt = 'scooter'.encode('utf-8')                                                       #server salt, for when passwords are being hashed and we add this in to defend against bruteforcers
        if os.path.isfile('passwords.txt'):                                                         #if there is a passwords file in the same directory, we engage passwords mode
            with open('passwords.txt', 'r') as file:
                self.passwords = [x.strip('\n') for x in file.readlines()]                          #to add passwords to the server just put them in the file as plain text and they will be hashed
                tempList = []
                for x in self.passwords:
                    if x.endswith(':h'):                                                            #:h signifies its already been hashed, otherwise its plain text so we hash it
                        tempList.append(x)
                    else:
                        tempList.append(hashlib.md5(self.salt + x.encode('utf-8')).hexdigest() + ':h') #hashing
                self.passwords = tempList

            with open('passwords.txt', 'w') as file:        #rewrite the newly hashed passwords to the file
                for x in self.passwords:
                    file.write(x + '\n')
                   
        else:
            self.passwords = None
            
        super(Server, self).__init__()

    def run(self):
        while True:                                          #the servers main job is to just sit and accept clients, then pass them off to a handler thread
            con, addr = self.sock.accept()
            print('Connection made at IP: ', str(addr[0]))
            print('Passing off client..')
            client = ClientHandler(con, addr[0], self, self.update)      #creating the client handler thread, needs the connection object and IP the client is from
            client.start()
            self.currentCons.append([client, addr[0]])      #adds in the connection to the current connections list
            self.displayClients()
            
        print('Closing server and connections..')
        self.sock.close()
        self.deleteAllClients()
        if self.update:
            self.updates("_________, SERVER SHUT DOWN", "______")
            
    def updates(self, ip, command, filename):                #just updates a text file with the client, action, and date to keep track of the flow of things
        if os.path.isfile('datalog.txt'):
            with open('datalog.txt', 'a') as file:
                file.write(ip + ',' + command + ',' + filename + ',' + time.strftime('%d/%m/%Y %H:%M:%S') + '\n')
        else:
            with open('datalog.txt', 'w') as file:
                file.write('IP ADDRESS, COMMAND, FILENAME, DATE/TIME\n')
                file.write(ip + ',' + command + ',' + filename + ',' + time.strftime('%d/%m/%Y %H:%M:%S') + '\n')   
    
    def displayClients(self):                                               #printing out all the current connections
        for num in range(len(self.currentCons)):
            print(str(num + 1) + ". " + self.currentCons[num][1])

    def deleteClient(self, index):                                          #boots a specific client off the server, starts counting from 1 <--
        self.clientLock.acquire()
        client = self.currentCons[index - 1][0]
        client.quit()
        self.currentCons.pop(index - 1)
        self.clientLock.release()
        
    def deleteAllClients(self):                                             #boot all clients off the server
        for x in range(len(self.currentCons)):
            self.deleteClient(x+1)
    
if __name__ == "__main__":  #just test code
    parser = argparse.ArgumentParser()      #adding in command line parser to
    
    parser.add_argument('-ho', '--host', help= 'The host IP the server will bind to', type= str)
    parser.add_argument('-po', '--port', help= 'The host port the server will bind to', type= int)
    parser.add_argument('-bl', '--backlogs', help= 'The number of backloged connections allowed until rejecting further traffic', type= int)
    parser.add_argument('-up', '--update', help= 'If set to true then the server will keep a log of all data flow', type = bool)
    args = parser.parse_args()

    if(args.host == None):  #setting some default values if nothing was entered into the optional parameters
        args.host = "127.0.0.1"
    if(args.port == None):
        args.port = 5000
    if(args.backlogs == None):
        args.backlogs = 100
    if (args.update == None):
        args.update = True
    s = Server(args.host, args.port, args.backlogs, args.update)
    s.start()

        
