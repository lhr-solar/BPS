import threading
import sqlite3 as sql

class Database(threading.Thread):

    def __init__(self):
        self.con = sql.connect('Server.db')
        self.cur = self.con.cursor()

    def passwordStatus(self, password):
        self.cur.execute('SELECT status FROM passwords WHERE password =?', (password))
        status = cur.fetchone()

        if not status:
            return False

        return status

    def isPasswordFree(self, password):

        self.cur.execute('SELECT * FROM passwords')
        rows = self.cur.fetchall()
        
        for row in rows:
            if password == row[1]:
                return False

        return True

    def addNewPassword(self, password, status):
        self.cur.execute('INSERT INTO passwords VALUES(?, ?)', (password, status))
        return updateServer()
        
        
    def removePassword(self, password):
        self.cur.execute("DELETE FROM passwords WHERE password = ?", (password))
        return updateServer()
        
        
    def changeStatus(self, password, newStatus):
        self.cur.execute('UPDATE passwords SET status= ? WHERE password= ?', (newStatus, password))
        return updateServer()
        
    def updateServer(self):
        try:
            self.con.commit()
            return True
        except:
            self.con.rollback()
            return False
    
    def run(self):

if __name__ == '__main__':
    con = sql.connect('Server.db')
    cur = con.cursor('CREATE TABLE passwords values(password TEXT, status TEXT)')

    cur.execute('INSERT INTO passwords VALUES(corbosiny247, ADMIN)')

    con.commit()
    con.close()
    print("Database and table created")
