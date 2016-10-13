import sys
with open('testFile.txt', 'w') as file:
    file.write('testing testing testing testing testing testing')
with open('testFile.txt', 'r') as file:
    data = "asdf".encode('utf-8')
    while data:
        data = file.read(5)
        print(data)
