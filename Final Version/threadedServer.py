import socket, clientHandler, argparse


###just a test server without the added passwords or locking of the full server, <-----------
###for documentation check out the real server, its almost identical <----------
class Server():

    def __init__(self, IP= '127.0.0.1', port= 5000, backlogs= 100):
        self.IP = IP
        self.port = port
        self.backlogs = backlogs

        self.sock = socket.socket()
        self.sock.bind((IP, port))

        self.sock.listen(backlogs)

        self.currentCons = []

    def main(self):
        while True:
            con, addr = self.sock.accept()
            connection = clientHandler.clientHandler(con, addr[0])
            connection.start()
            self.currentCons.append([connection, addr[0]])
            print('Connection made at IP:', str(addr[0]))
    

    def displayConnections(self):
        for con in self.currentCons:
            print('Connection ' + str(currentCons.index(con) + 1) + ": ",  con[1])
            
    def dropConnection(self, index):
        con = self.currentCons.pop(index)
        con.quit()

    def dropAllConnections(self):
        for x in range(len(self.currentCons)):
            self.dropConnection(x)

    def dropConnectionRange(self, indices):
        for x in range(indices[0], indices[1] + 1):
            self.dropConnection(x)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument('-ho', '--host', help= 'The host IP to bind to', type= str)
    parser.add_argument('-po', '--port', help= 'The port number to bind to', type= int)

    args = parser.parse_args()

    if not args.host:
        args.host = '127.0.0.1'
    if not args.port:
        args.port = 5000

    server = Server(args.host, args.port)
    server.main()

    
