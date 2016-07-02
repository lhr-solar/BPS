#this program doesnt do much more than get user input that triggers certain actions from the socket connection to the server
from SocketConnection import SocketConnection

def connectToServer(host, port):
    return SocketConnection(host, port)

def main(host, port, password):
    #creates the server connection
    socket = connectToServer(host, port)

    #sends over the password
    socket.sendData(password)

    #print the inital server directory
    data = socket.recieveData()
    print(data, end= '')
    command = ""

    while command != 'wq':
        command = input('')
        socket.sendData(command)

        if command = None:
            pass
            print(data,end= '')

        #the users commands trigger different functions within the socketConnection which deals with the servers socket
        else:
            if command[:3] == "-uf": #upload file
                try:
                    socket.sendFile(command[4:])
                except:
                    pass
                
            elif command[:3] == "-df": #download file
                try:
                    socket.recieveFile(command[4:])
                except:
                    pass
                
            elif command[:4] == "-ufs": #upload files, entered as names sperated by spaces
                try:
                    files = command[5:]
                    files = files.split()
                    socket.sendFiles(files)
                except:
                    pass
                
            elif command[:4] == "-dfs": #download files, entered as names seperated by spaces
                try:
                    files = command[5:]
                    files = files.split()
                    socket.recieveFiles(files)
                 except:
                     pass
                    
            elif command[:3] == "-ud": #upload directory
                try:
                    socket.sendDir(command[4:])
                except:
                    pass
                
            elif command[:3] == "-dd": #download directory
                try:
                    socket.recieveDir(command[4:])
                except:
                    pass
                
            elif command[:3] == "-wq": #ends the connection and program
                socket.quit = True
                
            data = socket.recieveData() #keeps recieving and displaying data until the server sends the input prompt
            while '>>' not in data:
                print(data)
                data = socket.recieveData()

            print(data, end= '')

if __name__ == "__main__":

    #starts up a connection based on command line arguments and gets ready for testing
    parser = argparse.ArgumentParser()
    parser.add_argument('host', help= 'The IP of the server', )
    parser.add_argument('port', help= 'Port the server is allowing connections on', type= int)
    parser.add_argument('password', help= 'Passwords of the server', type= str)

    args = parser.parse_args()

    main(args.host, args.port, args.password)
