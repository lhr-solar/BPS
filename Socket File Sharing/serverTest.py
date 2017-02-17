from basicClient import clientSocket

clients = []
for i in range(20):
    client = clientSocket('127.0.0.1', 5000)
    clients.append(client)

for client in clients:
    client.sendCommand('-DF testFile.txt')

for client in clients:
    client.sendCommand('wq')


