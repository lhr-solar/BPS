import socket
import clientHandler

sock = socket.socket()

print('Attempting to bind server..')
sock.bind(('127.0.0.1', 5000))
sock.listen(100)

print('Server waiting for client..')


while True:
    con, addr = sock.accept()
    connection = clientHandler.clientHandler(con, addr[0])
    connection.start()
    print('Connection made at IP:', str(addr[0]))
    

print('Closing server')
sock.close()
