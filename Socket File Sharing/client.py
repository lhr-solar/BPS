import socket, sys, os, argparse, subprocess, ntpath

class Client():    #it is possilbe to make a version that is threaded so it can implement with other programs without stopping them

    def __init__(self, IP= '127.0.0.1', port= 5000, password= None):        #just needs IP and Port, password is optional; check your server first

        self.IP = IP
        self.port = port
        
        self.con = socket.socket()
        self.con.connect((IP, port))

        if password != None:                     #if a password was entered it sends it to the server, if wrong the client closes
            self.con.send(password.encode('utf-8'))
            self.results = self.recvData()
            if 'NOT' in self.results:
                    raise Exception('PASSWORD NOT ACCEPTED')
        
    def recvFile(self, fileName):                #receiving a file in predetermined chunks from the server
        data = ''.encode('utf-8')                #setting up a string of bits
        error = False                            #flag for if anything went wrong server side

        fileName = ntpath.basename(fileName)
        with open(fileName, 'wb') as file:
                while True:
                        self.con.send('ready'.encode('utf-8'))  #used to make sure both ends are in synch
                        chunk = self.con.recv(1024)             #predetermined chunck size is 1024
                        
                        if sys.getsizeof(chunk) < 1024:                 #if the chunk size is less than 1024 the server hit an error is out of stuff to send
                                if '$$ERROR$$'.encode('utf-8') in chunk: #looking for the error flag in the message
                                        error = True
                                        break
                                else:                                   #if no error than that means we must be done
                                        data += chunk
                                        break
                        else:                               
                                data += chunk

                if not error:
                        file.write(data)
                else:                                               #if there was an error just scrap the file we set up to hold the data
                        file.close()
                        os.remove(fileName)
                        
    def sendFile(self, fileName):                                   #uploads a file to the server in predetermined chunk sizes
            try:
                    with open(fileName, 'rb') as file: 
                            while True:
                                    self.con.recv(22)               #wait for the ready message
                                    data = file.read(1024)
                                    self.con.send(data)
                                    if sys.getsizeof(data) < 1024:  #if the data is less than 1024 we are out of things to send
                                            break
            except Exception as e:                                  #if there was an error, send the message and error flag over
                    self.con.recv(22)
                    self.con.send(('$$ERROR$$' + str(e)).encode('utf-8'))

    def recvData(self):                                             #used to avoid writing the decode line when just receiving strings
        results = self.con.recv(55)
        self.con.send('ready'.encode('utf-8'))
        return self.con.recv(int(results)).decode('utf-8')

    def sendCommand(self, message):                     #sends a command over and triggers its own response to whichever command it is
        if message[0:3] == "lc:":
            command = message[3:]
            if command[0:2] == "cd":
                directory = command[3:]
                try:
                    os.chdir(directory)
                    self.results = str(os.getcwd())
                except:
                    self.results = "Error in changing directory, directory may not exist"
            else:
                cmd = subprocess.Popen(command, shell= True, stdout= subprocess.PIPE, stderr= subprocess.PIPE)
                self.results = (cmd.stdout.read() + cmd.stderr.read()).decode('utf-8')
                
        else:
            self.con.send(message.encode('utf-8'))
            if message[:3] == "-DF":    #download file from server
                fileName = message[4:]
                if "passwords.txt" not in fileName and "datalog.txt" not in fileName:
                    self.recvFile(fileName)

            elif message[:3] == "-UF":  #upload file to server
                fileName = message[4:]
                if "passwords.txt" not in fileName and "datalog.txt" not in fileName:
                    self.sendFile(fileName)
            
            self.results = self.recvData() #server will send back the results of the command
            if "Ending Connection due to Errors" == self.results:
                raise Exception("Ending Connection due to Errors")
        
if __name__ == '__main__': #below is mostly test code
    parser = argparse.ArgumentParser()      #adding in an optional command line parser, allows you to start the program from the command line
    parser.add_argument('-i', '--ip', help= 'The IP address the client will connect to', type= str)
    parser.add_argument('-p', '--port', help= 'Port on the server machine to connect to', type= str)
    parser.add_argument('-ps', '--password', help= 'password to send to the server, leave blank if no password', type= str)

    args = parser.parse_args()

    if args.ip == None:     #just sets the arguments to default values if nothing was entered
        args.ip = '127.0.0.1'
    if args.port == None:
        args.port = 5000

    client = Client(args.ip, args.port, args.password)
    message = ''
    while message != "wq":      #starts up the client and waits for commands
        message = str(input('>>'))
        client.sendCommand(message)
        print(client.results)
