import socket, sys, os

def recvFile(fileName, con):   #downloads a file from the server, sent in predetermined chunk sizes
        data = ''.encode('utf-8')                                                       #initialize an empty object that holds bits
        error = False                                                                   #signals an error occured during the file transfer

        with open(fileName, 'wb') as file:                                              #here we are opening the file we will dump into the bytes transfered   
                while True:
                        con.send('ready'.encode('utf-8'))                                #signals for the sending of a byte of data, this is to make sure they are in time
                        chunk = con.recv(1024)                  
                        
                        if sys.getsizeof(chunk) < 1024:                         #1024 is the agreed upon size, if its less than 1024 either the server sent an error message or ran out of stuff to send
                                if '$$ERROR$$'.encode('utf-8') in chunk:        #checking for the error message
                                        error = True
                                        break
                                else:                                           #otherwise we know its just the end of the file
                                        data += chunk
                                        break
                        else:                                                   #normal chunk recieved
                                data += chunk

                if not error:                                                   #no errors === no problem, download to the file
                        file.write(data)
                else:                                                           #error? well then no deal, scrap the file, try again later homie
                        file.close()
                        os.remove(fileName)
                        
def sendFile(fileName, con):         #uploads a file to the server in predetermined chunk sizes
        try:                                                    #put this bad boy in a try except incase we read a bad file
                with open(fileName, 'rb') as file:              
                        while True:
                                con.recv(22)                    #wait for the ok message
                                data = file.read(1024)          #read then send the chunk over
                                con.send(data)
                                if sys.getsizeof(data) < 1024:  #if our chunk is left than 1024 than we know we are done with this punk file
                                        break            
        except Exception as e:                                  #got yourself a bad file? That's cool wait for the ok, then send over the error message!
                con.recv(22)
                con.send(('$$ERROR$$' + str(e)).encode('utf-8'))

def recvData():                                #simple wrapper that automatically decodes are data, useful when we are trying to receive actualy strings
        results = sock.recv(1024)
        return sock.recv(int(results)).decode('utf-8')


if __name__ == "__main__":              #just some test code that takes in special commands and sends them to the server
        sock = socket.socket()

        print('Connecting to server..')
        sock.connect(('127.0.0.1', 5000))

        print('Connected to server')

        password = str(input('PASSWORD(put in nothing if no password): '))
        if len(password) > 0:
                sock.send(password.encode('utf-8'))     #if password entered send it, if the server says NOPE than bye bye
                results = recvData()
                print(results)
                if 'NOT' in results:
                        sys.exit()
        

        message = ''
        while message != 'wq':                                  #just test file uploading/downloading and command line commands, when done hit wq
                message = str(input('>>> '))
                sock.send(message.encode('utf-8'))
        
                if message[:3] == "-DF":
                        fileName = message[4:]
                        if fileName != 'passwords.txt':
                                recvFile(fileName, sock)

                elif message[:3] == "-UF":
                        fileName = message[4:]
                        if fileName != 'passwords.txt':
                                sendFile(fileName, sock)

                results = recvData()
                print(results)

        print('Connection closed')


