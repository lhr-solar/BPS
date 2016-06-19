import socket, sys, argparse, os, threading
import SocketConnection #what directly talks to the client

class ConnectionHandler(threading.Thread)
{

    def __init__(self, con, addr, port, server):
        self.connection = SocketConnection(addr, port, con)
        
    def run(self):
        password = self.connection.recieveData()
        while command != "wq":
            command = self.connection.recieveData()

            
        
    #Important support functions used when the thread is apart of a server to avoid corrupting files
    def isFileOpen(self, path): #makes sure another thread does not have the file in question open
        for file in self.server.openFiles:
            if path == file:
                return True

        return False

    def markFileAsOpen(self, path):
        for file in self.server.openFiles:
            if file == path:
                return False
            
        self.server.openFiles.append(path)
        return True
    
    def markFileAsClosed(self, path):
        self.server.openFiles.remove(path)


    #Manages pausing and quitting/closing the socket connection
    def pause(self): 
        if self.pause is False:
            pass
        else:
            self.pause = True

    def unpause(self):
        if self.pause:
            self.pause = False
        else:
            pass

    def isPaused():
        return self.pause




}
