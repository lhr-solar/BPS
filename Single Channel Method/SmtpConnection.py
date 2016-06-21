import os
import sys
from smtplib import *

from email import encoders
from email.message import Message
from email.mime.audio import MIMEAudio
from email.mime.base import MIMEBase
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

class smtpConnection(threading.Thread):

    def __init__(self, username, password, smtpServer):
        self.username = username
        self.password = password
        if not smtpServer.startswith("smtp."):
            self.smtpServer = "smtp." + "smtpServer"
            
        else:
            self.smtpServer = smtpServer

        try:
            print("Connecting with smtp server..")
            self.mail = SMTP(smtpServer, 587)
            self.mail.ehlo()
            self.mail.starttls()
            print("Connected to smtp server", self.smtpServer)
            self.active = True
            
        except:
            print("Error occurred while connecting to smtp server ", self.smtpServer, "with error of:", sys.exc_info()[0])
            self.active = False

        self.quit = False

        if self.active:
            self.quit = True
            try:
                print("Logging in to email account..")
                self.mail.login(self.username, self.password)
                print("Successfuly logged into email account", self.username)
                
            except:
                print("Print error logging into email account:", sys.exc_info()[0])
                self.active = False
            
        
        
    def run(self):
        while self.quit = False:
            pass

    def encodeFileBase64(self, file, filesName):
        print("Encoding", filesName + "...")
        attachment = MIMEBase('application', 'octet-stream')
        attachment.set_payload(file.read())
        encoders.encode_base64(attachment)

        print("Attaching", filesName + "...")
        attachment.add_header('Content-Disposition', 'attachment', filename = filesName)
        print(filesName, "was successfully attached to email")
        file.close()
        
        return attachment 

    def attachFile(self, file, email, triedText = False):

        try:
            print("Opening", file + " to attach...")
            with open(file, 'rb') as f:
                email.attach(encodeFileBase64(f, file))

        except:
            print("Error occurred when attaching file", file, ":", sys.exc_info()[0])   
            if not triedText:
                print("Trying to convert to text file..")
                try:
                    with open(file, 'rb') as f:
                        dotIndex = file.index('.')
                        if dotIndex != -1:
                            file = file[:dotIndex] + "To_txt.txt"
                        else:
                            file += "To_txt.txt"

                        print("Reading binary text data from file", file[:len(file) - 10], "..")
                        with open(file, 'wb') as fNew:
                            print("Writing binary data to new text file", fNew, "..")
                            while True:
                                data = f.read(1024)
                                if data:
                                    fNew.write(data)
                                    continue
                                break
                            print("Binary data successfully written to text file")
                            
                        email = attachFile(file, email, True)
                    
                except:
                    print("Could not convert file binary data to a text file:", sys.exc_info()[0])

            else:
                print("New text file could also not be attached, aborting attachment of this file")
                    
        return email

    def attachDirectory(self, directory, email, clearance = 0): #clearance of zero only attaches all files from the chosen directory, ever level of clearance declares how deep it will add directories in side this directory
        
        for file in os.listdir(directory):
            path = os.path.join(directory, file)
            print("Determining type of", file + "...")
            
            if os.path.isfile(path):
                print("Attempting to attach", file + "...")
                email = attachFile(path, email)

            elif clearance > 0:
                if os.path.isdir(path):
                    print("Attempting to attach contents of", file + " directory..." )
                    email = attachDirectory(path, email, clearance - 1)

            elif clearance < 0:
                if os.path.isdir(path):
                    email = attachDirectory(path, email, clearance)

            else:
                print(file, "no clearance for this directory")

        print("End of " + directory + " directory's contents reached")
        
        return email

    def sendMail(self, sendTo, subject, text, files = None):
        
        msg = MIMEMultipart()
        msg["From"] = self.username
        if isinstance(sendTo, list):
            msg["To"] = ", ".join(sendTo)
        else:
            msg["To"] = sendTo
        msg["Subject"] = subject
        msg.attach(MIMEText(text))
        
        if files:
            if isinstance(files, list):
                for file in files:
                    if os.path.isdir(file):
                        print("Attempting to attach contents of", file + " directory..." )
                        msg = attachDirectory(file, msg, 1)
                        
                    elif os.path.isfile(file):
                        print("Attempting to attach", file + "...")
                        msg = attachFile(file, msg)

            else:
                if os.path.isdir(files):
                    print("Attempting to attach contents of", file + " directory..." )
                    msg = attachDirectory(files, msg, 1)
                        
                elif os.path.isfile(files):
                    print("Attempting to attach", file + "...")
                    msg = attachFile(files, msg)
            
        try:
            print("Sending mail..")
            self.mail.sendmail(self.username, sendTo, msg.as_string())
            print("Mail Successfully sent")
            return True
        
        except:
            print("Error sending mail:", sys.exc_info()[0])    
            return False

        
if __name__ == "__main__":
    sendMail('coreyohulse@gmail.com', ['cohulse@asu.edu', 'coreyohulse@gmail.com'], 'testing', 'Hope this works!', ['serverDirectory', 'serverFile.txt'])
    print("Sent Successfully!")
