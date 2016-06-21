from SocketConnection import SocketConnection

    def connectToServer(host, port):
    return SocketConnection(host, port)

recvCommands = []
commandLock = threading.Lock()
Quit = False

def handleInput(con):
    global recvCommands
    global commandLock
    global Quit
    
    while not Quit: 
        while len(recvCommands) == 0:
            pass

        command = recvCommands[0]

        if command[:3] == '-df':
            con.recieveFile(command[4:])
            
        elif command[:4] == '-dfs':
            files = command[5:]
            files = files.split()
            con.recieveFiles(files)

        elif command[:3] == '-dd':
            con.recieveDir(command[4:])
            
        commandLock.acquire()
        recvCommands.remove(command)
        commandLock.release()

    con.quit = True

def main(host, port, password):
    global recvCommands
    global commandLock
    transmitSocket = connectToServer(host, port)
    recvSocket = connectToServer(host, port)

    transmitSocket.sendData(password)

    inputManager = threading.Thread(target= handleInput, args=(recvSocket))
    inputManager.start()
    
    command = ""

    while command != 'wq':
        command = input('')
        socket.sendData(command)

        if command = None:
            pass
            print('>> ')
            
        else:
            if command[:3] == "-uf":
                try:
                    transmitSocket.sendFile(command[4:])
                except:
                    print('Error sending file')
                    
            elif command[:3] == "-df":
                commandLock.acquire()
                recvCommands.append(command)
                commandLock.release()
                
            elif command[:4] == "-ufs":
                try:
                    files = command[5:]
                    files = files.split()
                    transmitSocket.sendFiles(files)
                except:
                    print('Error sending files')
                    
            elif command[:4] == "-dfs":
                commandLock.acquire()
                recvCommands.append(command)
                commandLock.release()
                
            elif command[:3] == "-ud":
                try:
                    transmitSocket.sendDir(command[4:])
                except:
                    print('Error sending directory')
                    
            elif command[:3] == "-dd":
                commandLock.acquire()
                recvCommands.append(command)
                commandLock.release()
                
            elif command[:3] == "-wq":
                transmitSocket.quit = True
                Quit = True
                
            data = transmitSocket.recieveData()
            while '>>' not in data:
                print(data)
                data = transmitSocket.recieveData()

            print(data, end= '')


    
if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('host', help= 'The IP of the server', )
    parser.add_argument('port', help= 'Port the server is allowing connections on', type= int)
    parser.add_argument('password', help= 'Passwords of the server', type= str)

    args = parser.parse_args()

    main(args.host, args.port, args.password)
