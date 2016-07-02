import threading
import sqlite3 as sql

class Database(threading.Thread): #handles tracking all known users and their privalleges

    def __init__(self):
        #connects to the database on the server
        self.con = sql.connect('Server.db') 
        self.cur = self.con.cursor()

        self.quit = False #used to trigger the shutdown of the database connection

    def isPasswordFree(self, password):
        try:
            #checks if the input password is already taken by looking if there are any matching entries in the passwords table
            self.cur.execute('SELECT status FROM passwords WHERE password = ?', (password))
            status = cur.fetchone()

        except:
            status = False
            
        if status: #if status contains something, then the password is not free
            return False
        
        return True



    def addNewPassword(self, password, status):
        if isPasswordFree(password): #if the password isnt in use, simply insert it into the table
            self.cur.execute('INSERT INTO passwords VALUES(?, ?)', (password, status))
            return self.updateServer()

        else:
            return False
        
    def removePassword(self, password):
        self.cur.execute("DELETE FROM passwords WHERE password = ?", (password))
        return self.updateServer()
        
        
    def changeStatus(self, password, newStatus):
        self.cur.execute('UPDATE passwords SET status= ? WHERE password= ?', (newStatus, password))
        return self.updateServer()
        
    def updateServer(self): #commits any changes to the server, returns false and auto rollbacks the server if an error was encountered
        try:
            self.con.commit()
            return True
        except:
            self.con.rollback()
            return False
    
    def run(self):
        while not self.quit:
            pass #just hangout until the main server calls some funcison

        self.updateServer()
        self.con.close()
        
if __name__ == '__main__':
    #####NEVER RUN THIS UNLESS YOU ARE OK WITH DELETING THE USER TABLE#####
    #sets up the server and the table 
    con = sql.connect('Server.db')
    cur = con.cursor()

    cur.execute('DROP TABLE IF EXIST passwords') #to avoid duplicating the table during testing it deletes the old table
    cur.execute('CREATE TABLE passwords values(password TEXT, status TEXT)')
    cur.execute('INSERT INTO passwords VALUES(corbosiny247, ADMIN)') #adds in one admin

    con.commit() #commits all changes
    con.close()  #closes the server
    print("Database and table created")

    #for testing purposes
    command = ""
    while command != 'wq':
        print('-' * 25)
        print('[0]addNewPassword \n[1]Remove Password\n[2]Change Status\n')
        command = input('>>')

        if command[:3] == '[0]':
            data = command[4:].split()
            db.addNewPassword(data[0], data[1])

        elif command[:3] == '[1]':
            db.removePassword(command[4:])
            
        elif command[:3] == '[2]':
            data = command[4:]
            db.changeStatus(data[0], data[1])
    
    print('Testing done')
    print('Closing Server')
    db.quit = True
    
