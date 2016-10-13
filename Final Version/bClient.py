import socket

def recvFile(fileName, con):
        with open(fileName, 'wb') as file:
            data = bytes()
            while True:
                chunk = con.recv(1024)
                if chunk.decode('utf-8') == "DONE":
                    break;
                else:
                    data += chunk

            print(data)
            file.write(data)
            
def sendFile(fileName, con):
        with open(fileName, 'rb') as file:
            while True:
                data = file.read(1024)
                if not data:
                    con.send("DONE".encode('utf-8'))
                    break;
                else:
                    con.send(data)

sock = socket.socket()

print('Connecting to server..')
sock.connect(('127.0.01', 5000))

print('Connected to server')

message = ''
while message != 'wq':
    message = str(input('>>> '))
    sock.send(message.encode('utf-8'))

    if message[:3] == "-DF":
        fileName = message[4:]
        recvFile(fileName, sock)
        
    elif message[:3] == "-UF":
        fileName = message[4:]
        sendFile(fileName, sock)
    
    results = sock.recv(1024)
    results = sock.recv(int(results)).decode('utf-8')
    print(results)
    
print('Connection closed')


