import socket
import base64
import sys
import os

def recvFile(fileName, con):
        data = ''.encode('utf-8')
        error = False

        with open(fileName, 'wb') as file:
                while True:
                        con.send('ready'.encode('utf-8'))
                        chunk = con.recv(1024)
                        
                        if sys.getsizeof(chunk) < 1024:
                                if '$$ERROR$$'.encode('utf-8') in chunk:
                                        error = True
                                        break
                                else:
                                        data += chunk
                                        break
                        else:
                                data += chunk

                if not error:
                        file.write(data)
                else:
                        file.close()
                        os.remove(fileName)
                        
def sendFile(fileName, con):
        try:
                with open(fileName, 'rb') as file:
                        while True:
                                con.recv(22)
                                data = file.read(1024)
                                con.send(data)
                                if sys.getsizeof(data) < 1024:
                                        break
        except Exception as e:
                con.recv(22)
                con.send(('$$ERROR$$' + str(e)).encode('utf-8'))
                
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


