from SocketConnection import SocketConnection

def connectToServer(host, port):
    return SocketConnection(host, port)

def main(host, port, password):
    socket = connectToServer(host, port)

    socket.sendData(password)

    command = ""

    while command != 'wq':
        command = input('')
        socket.sendData(command)

        if command = None:
            pass
            print('>> ')
            
        else:
            if command[:3] == "-uf":
                socket.sendFile(command[4:])
                
            elif command[:3] == "-df":
                socket.recieveFile(command[4:])
                
            elif command[:4] == "-ufs":
                files = command[5:]
                files = files.split()
                socket.sendFiles(files)
                
            elif command[:4] == "-dfs":
                files = command[5:]
                files = files.split()
                socket.recieveFiles(files)
                
            elif command[:3] == "-ud":
                socket.sendDir(command[4:])
                
            elif command[:3] == "-dd":
                socket.recieveDir(command[4:])
                
            elif command[:3] == "-wq":
                socket.quit = True
                
            data = socket.recieveData()
            while '>>' not in data:
                print(data)
                data = socket.recieveData()

            print(data, end= '')

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('host', help= 'The IP of the server', )
    parser.add_argument('port', help= 'Port the server is allowing connections on', type= int)
    parser.add_argument('password', help= 'Passwords of the server', type= str)

    args = parser.parse_args()

    main(args.host, args.port, args.password)
